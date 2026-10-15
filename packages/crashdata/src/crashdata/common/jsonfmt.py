"""A JSON file written back in its own layout (#1167, docs/editors.md "The editor's backend").

The data files are laid out by hand or by the tool that made them (one
entity per line, a level's values on one line, a cutscene's slides
under it), and the layouts differ from file to file. An editor that
changes one value must not rewrite the rest, and one that changes
nothing must give back the same bytes. dumps_like(text, value) writes
`value` in `text`'s layout:

- a value equal to the one the text had (the same types, the same keys
  in the same order) is the text's own, byte for byte, so an unchanged
  file is the same file;
- a changed object or array keeps its brackets, the whitespace between
  its items and its keys' text, and only the changed items are written
  again (an item moved, added or removed is matched with difflib);
- a new value is written like the file's other values at the same place
  (the same path with any array index: another entity of the room,
  another slide), and with no such value, like json.dumps with the
  file's indentation and separators.

crashdata.editing picks a tool's own writer instead where the file is
in that writer's layout (room.json, levels.json).
"""
import difflib
import json

_WS = " \t\n\r"
_decoder = json.JSONDecoder()


class _Node:
    """A value of the text: its kind ("obj", "arr" or "val"), where it is,
    its value; a container's items (an object's (key text, colon text,
    node)), and the text between them (gaps[0] after the opening bracket,
    gaps[i] before item i with its comma, gaps[-1] before the closing one)."""
    __slots__ = ("kind", "start", "end", "value", "items", "gaps", "keys")

    def __init__(self, kind, start):
        self.kind, self.start, self.end = kind, start, start
        self.value, self.items, self.gaps, self.keys = None, [], [], []


class _Text:
    def __init__(self, text):
        self.text = text
        self.by_path = {}  # (keys, with "*" for an array index) -> [node]
        i = self._ws(0)
        self.root = self._value(i, ())
        self.prefix, self.suffix = text[:self.root.start], text[self.root.end:]
        if self._ws(self.root.end) != len(text):
            raise ValueError("extra data after the JSON value")
        self.style = self._style()

    def _ws(self, i):
        n = len(self.text)
        while i < n and self.text[i] in _WS:
            i += 1
        return i

    def _value(self, i, path):
        t = self.text
        if i >= len(t):
            raise ValueError("unexpected end of the JSON text")
        c = t[i]
        if c == "{" or c == "[":
            node = _Node("obj" if c == "{" else "arr", i)
            close = "}" if c == "{" else "]"
            gap = i + 1
            j = self._ws(gap)
            if t[j:j + 1] == close:
                node.gaps.append(t[gap:j])
            else:
                while True:
                    if node.kind == "obj":
                        key, kend = _decoder.raw_decode(t, j)
                        if not isinstance(key, str):
                            raise ValueError("an object's key isn't a string")
                        k2 = self._ws(kend)
                        if t[k2:k2 + 1] != ":":
                            raise ValueError("':' expected at %d" % k2)
                        v0 = self._ws(k2 + 1)
                        child = self._value(v0, path + (key,))
                        node.keys.append(key)
                        node.items.append((t[j:kend], t[kend:v0], child))
                    else:
                        child = self._value(j, path + ("*",))
                        node.items.append((None, None, child))
                    node.gaps.append(t[gap:j])
                    gap = child.end
                    j = self._ws(gap)
                    if t[j:j + 1] == ",":
                        j = self._ws(j + 1)
                        continue
                    if t[j:j + 1] != close:
                        raise ValueError("',' or '%s' expected at %d" % (close, j))
                    node.gaps.append(t[gap:j])
                    break
            node.end = j + 1
            if node.kind == "obj":
                node.value = {k: it[2].value for k, it in zip(node.keys, node.items)}
            else:
                node.value = [it[2].value for it in node.items]
        else:
            node = _Node("val", i)
            node.value, node.end = _decoder.raw_decode(t, i)
        self.by_path.setdefault(path, []).append(node)
        return node

    def _style(self):
        """The file's ways: indentation unit, an inline container's item
        separator, the colon, whether non-ASCII is escaped."""
        indent, sep, colon = None, None, None
        for nodes in self.by_path.values():
            for n in nodes:
                if n.kind == "val":
                    continue
                if colon is None and n.kind == "obj" and n.items:
                    colon = n.items[0][1]
                if len(n.gaps) > 2:
                    g = n.gaps[1]
                    if "\n" not in g and sep is None:
                        sep = g.strip(_WS[1:]) if g.strip() == "," else g
                    if "\n" in g and indent is None:
                        line = _line_indent(self.text, n.start)
                        child = g.rsplit("\n", 1)[1]
                        if child.startswith(line) and len(child) > len(line) and not child.strip():
                            indent = child[len(line):]
        escaped = "\\u" in self.text
        non_ascii = any(ord(c) > 127 for c in self.text)
        return {"indent": indent if indent is not None else "  ", "sep": sep if sep is not None else ", ",
                "colon": colon if colon is not None else ": ",
                "ensure_ascii": escaped or not non_ascii}


def _line_indent(text, i):
    """The whitespace that starts the line position i is on."""
    s = text.rfind("\n", 0, i) + 1
    e = s
    while e < i and text[e] in " \t":
        e += 1
    return text[s:e]


def same(a, b):
    """a and b are the same JSON value, written the same: the same types
    (1 isn't 1.0 or true) and an object's keys in the same order."""
    if type(a) is not type(b):
        return False
    if isinstance(a, dict):
        return list(a) == list(b) and all(same(a[k], b[k]) for k in a)
    if isinstance(a, list):
        return len(a) == len(b) and all(same(x, y) for x, y in zip(a, b))
    return a == b


def _key(v):
    return json.dumps(v, sort_keys=False, separators=(",", ":"))


class _Writer:
    def __init__(self, parsed):
        self.p = parsed
        self.style = parsed.style

    def dumps(self, v, inline, indent):
        st = self.style
        if inline:
            return json.dumps(v, separators=(st["sep"], st["colon"]), ensure_ascii=st["ensure_ascii"])
        out = json.dumps(v, indent=st["indent"], separators=(",", st["colon"]), ensure_ascii=st["ensure_ascii"])
        return out.replace("\n", "\n" + indent)

    def template(self, v, path):
        """An old value like v at v's place: a node of the same kind at the
        same path (any array index), a container one with items."""
        kind = "obj" if isinstance(v, dict) else "arr" if isinstance(v, list) else "val"
        for n in self.p.by_path.get(path, ()):
            if n.kind == kind and (kind == "val" or n.items):
                return n
        return None

    def value(self, node, v, path, indent, inline=False):
        """v's text, at node's place (node: the old value there, or None)."""
        if node is not None and same(node.value, v):
            return self.p.text[node.start:node.end]
        if node is not None and node.items and (
                (node.kind == "obj" and isinstance(v, dict)) or (node.kind == "arr" and isinstance(v, list))):
            return self.splice(node, v, path, indent)
        return self.fresh(v, path, indent, inline)

    def fresh(self, v, path, indent, inline):
        """A new value, written like the file's other values at its place."""
        if not isinstance(v, (dict, list)) or not v:
            return self.dumps(v, True, indent)
        t = self.template(v, path)
        if t is None:
            return self.dumps(v, inline, indent)
        return self.container(t, v, path, indent, matched={})

    def splice(self, node, v, path, indent):
        """A changed container, in its old text: its items matched with the
        old ones (by key; an array's by difflib), the old gaps kept."""
        if node.kind == "obj":
            index = {k: i for i, k in enumerate(node.keys)}
            matched = {i: index[k] for i, k in enumerate(v) if k in index}
        else:
            matched = {}
            sm = difflib.SequenceMatcher(None, [_key(x.value) for _, _, x in node.items], [_key(x) for x in v],
                                         autojunk=False)
            for tag, i1, i2, j1, j2 in sm.get_opcodes():
                if tag in ("equal", "replace"):
                    for k in range(min(i2 - i1, j2 - j1)):
                        matched[j1 + k] = i1 + k
        return self.container(node, v, path, indent, matched)

    def container(self, node, v, path, indent, matched):
        """v (a dict or list) in node's brackets and gaps; matched: v's item
        index -> the old item it is (an item without one is new)."""
        text = self.p.text
        gaps, n = node.gaps, len(node.items)
        inline = "\n" not in text[node.start:node.end]
        if n >= 2:
            between = gaps[1]
        elif inline:
            between = self.style["sep"]
        else:
            between = "," + gaps[0]
        items = list(v.items()) if isinstance(v, dict) else list(enumerate(v))
        out = [text[node.start]]
        line = _line_indent(text, node.start) if not inline else indent
        for i, (k, x) in enumerate(items):
            j = matched.get(i)
            if i == 0:
                gap = gaps[0]
            elif j is not None and j > 0:
                gap = gaps[j]
            else:
                gap = between
            out.append(gap)
            child_indent = gap.rsplit("\n", 1)[1] if "\n" in gap else (indent if inline else line)
            old = node.items[j][2] if j is not None else None
            sub = path + ((k,) if isinstance(v, dict) else ("*",))
            if isinstance(v, dict):
                if j is not None:
                    out.append(node.items[j][0] + node.items[j][1])
                else:
                    out.append(json.dumps(k, ensure_ascii=self.style["ensure_ascii"]) + node.items[0][1])
            out.append(self.value(old, x, sub, child_indent, inline))
        out.append(gaps[-1] if items else "")
        out.append(text[node.end - 1])
        return "".join(out)


def dumps_like(text, value):
    """value as JSON, in text's layout (text: a JSON document, the file's
    old text)."""
    parsed = _Text(text)
    w = _Writer(parsed)
    return parsed.prefix + w.value(parsed.root, value, (), _line_indent(text, parsed.root.start)) + parsed.suffix


__all__ = ["dumps_like", "same"]
