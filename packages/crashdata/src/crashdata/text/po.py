"""A gettext file edited in place: entries changed, added or removed, the rest kept byte for byte (#1167).

The editors (docs/editors.md, "The cutscene editor") change a few strings
of games/<game>/text/*.pot and *.po at a time. Writing the files again
from what read_po gives would lose nothing gettext reads, but would change
them all (and `tools/text.py extract --check` compares them byte for byte).
So a PoFile keeps the file as its blank-line-separated blocks of text, and
an edit replaces only the blocks of the entries it changes, written as
msgcat writes them (`field_lines`: gettext's wrapping at 79 columns, which
gives back every entry of XS's files as they are). A file saved with no
change is its own bytes.
"""
import re
import textwrap

from crashdata import text as T

# msgcat's page width: a line is at most this many characters, its quotes included.
PAGE_WIDTH = 79

# No line break before these (UAX #14's LB13: CL, CP, EX, IS, SY; and the
# closing brackets): gettext breaks lines where Unicode's line breaking
# allows it (libunistring), and in XS's text that is after a space, unless
# one of these follows it, and after a hyphen or a slash before a letter.
NO_BREAK_BEFORE = set(")]}!?,.:;/")


def escape(s):
    return s.replace("\\", "\\\\").replace('"', '\\"').replace("\n", "\\n").replace("\t", "\\t")


def _breaks(s):
    """The positions in `s` a line may break at (between s[i-1] and s[i])."""
    out = []
    for i in range(1, len(s)):
        a, b = s[i - 1], s[i]
        if a == " " and b != " " and b not in NO_BREAK_BEFORE:
            out.append(i)
        elif a in "-/" and b.isalpha() and i >= 2 and s[i - 2] not in " ":
            out.append(i)
    return out


def _wrap(seg, width):
    """A segment's lines (escaped), each at most `width` characters where a
    break allows it: greedy, as libunistring's ulc_width_linebreaks."""
    lines = []
    while len(escape(seg)) > width:
        cut = None
        for b in _breaks(seg):
            if len(escape(seg[:b])) <= width:
                cut = b
            else:
                if cut is None:
                    cut = b  # a word longer than the line: it overflows
                break
        if cut is None:
            break
        lines.append(escape(seg[:cut]))
        seg = seg[cut:]
    lines.append(escape(seg))
    return lines


def field_lines(keyword, value, wrap=True):
    """`msgid "..."` (or msgstr, msgctxt) as msgcat writes it: one line if
    it fits in PAGE_WIDTH (or `wrap` is False: `msgcat --no-wrap`,
    N-Tranced's files) and has no newline before its end, else `KEYWORD
    ""` and the value's lines, broken after each newline and wrapped."""
    fits = not wrap or len(keyword) + 3 + len(escape(value)) <= PAGE_WIDTH
    if "\n" not in value[:-1] and fits:
        return ['%s "%s"' % (keyword, escape(value))]
    out = ['%s ""' % keyword]
    for seg in re.split(r"(?<=\n)", value):
        if seg:
            out += ['"%s"' % x for x in (_wrap(seg, PAGE_WIDTH - 2) if wrap else [escape(seg)])]
    return out


def comment_lines(comments):
    """`#.` comments as tools/text.py writes them (wrapped at 76)."""
    return ["#. " + x for c in comments for x in (textwrap.wrap(c, 76) or [""])]


def entry_text(key, msgid, msgstr, comments=(), wrap=True):
    return "\n".join(comment_lines(comments) + field_lines("msgctxt", key, wrap) + field_lines("msgid", msgid, wrap)
                     + field_lines("msgstr", msgstr, wrap))


class PoFile:
    """A .po or .pot file as its blocks; entries by key (msgctxt)."""

    def __init__(self, text, wrap=True):
        self.original = text
        self.wrap = wrap
        # blocks are separated by one empty line; the file ends in a newline
        body = text[:-1] if text.endswith("\n") else text
        self.blocks = body.split("\n\n")
        self.final_newline = text.endswith("\n")

    @classmethod
    def read(cls, path, wrap=True):
        with open(path, encoding="utf-8", newline="") as f:
            return cls(f.read(), wrap)

    def _fields(self, key, msgid, msgstr):
        return (field_lines("msgctxt", key, self.wrap) + field_lines("msgid", msgid, self.wrap)
                + field_lines("msgstr", msgstr, self.wrap))

    def text(self):
        return "\n\n".join(self.blocks) + ("\n" if self.final_newline else "")

    @staticmethod
    def parse(block):
        """A block's entry: {ctx, id, str, comments} (read_po's), or None
        (the header, an obsolete entry)."""
        cur = {"ctx": None, "id": None, "str": None, "comments": []}
        field = None
        for line in block.split("\n"):
            s = line.strip()
            if s.startswith("#~"):
                return None
            if s.startswith("#."):
                cur["comments"].append(s[3:] if s.startswith("#. ") else s[2:])
                continue
            if s.startswith("#") or not s:
                continue
            m = re.match(r'^(msgctxt|msgid|msgstr)\s+"(.*)"$', s)
            if m:
                field = {"msgctxt": "ctx", "msgid": "id", "msgstr": "str"}[m.group(1)]
                cur[field] = T.po_unquote(m.group(2))
                continue
            m = re.match(r'^"(.*)"$', s)
            if m and field:
                cur[field] += T.po_unquote(m.group(1))
        return cur if cur["ctx"] is not None else None

    def keys(self):
        return [e["ctx"] for e in map(self.parse, self.blocks) if e is not None]

    def index(self, key):
        for i, b in enumerate(self.blocks):
            e = self.parse(b)
            if e is not None and e["ctx"] == key:
                return i
        return None

    def get(self, key):
        i = self.index(key)
        return None if i is None else self.parse(self.blocks[i])

    def set(self, key, msgid=None, msgstr=None, comments=None):
        """Changes an entry's msgid, msgstr or comments (None: as it is).
        Its block is written again only if something changes."""
        i = self.index(key)
        if i is None:
            raise KeyError(key)
        e = self.parse(self.blocks[i])
        new = (e["id"] if msgid is None else msgid, e["str"] if msgstr is None else msgstr,
               e["comments"] if comments is None else list(comments))
        if new == (e["id"], e["str"], e["comments"]):
            return False
        if comments is None:
            # keep the block's own comment lines as they are
            head = [ln for ln in self.blocks[i].split("\n") if ln.startswith("#")]
            self.blocks[i] = "\n".join(head + self._fields(key, new[0], new[1]))
        else:
            self.blocks[i] = entry_text(key, new[0], new[1], new[2], self.wrap)
        return True

    def remove(self, key):
        i = self.index(key)
        if i is None:
            raise KeyError(key)
        del self.blocks[i]

    def rename(self, key, new_key):
        """An entry's key changed, its block otherwise kept."""
        i = self.index(key)
        if i is None:
            raise KeyError(key)
        e = self.parse(self.blocks[i])
        head = [ln for ln in self.blocks[i].split("\n") if ln.startswith("#")]
        self.blocks[i] = "\n".join(head + self._fields(new_key, e["id"], e["str"]))

    def insert_after(self, after_key, key, msgid, msgstr, comments=()):
        """A new entry right after `after_key`'s (or after the last entry
        whose key starts with `after_key` when it ends in "*")."""
        if after_key.endswith("*"):
            idx = [i for i, b in enumerate(self.blocks)
                   if (e := self.parse(b)) is not None and e["ctx"].startswith(after_key[:-1])]
            if not idx:
                raise KeyError(after_key)
            i = idx[-1]
        else:
            i = self.index(after_key)
            if i is None:
                raise KeyError(after_key)
        self.blocks.insert(i + 1, entry_text(key, msgid, msgstr, comments, self.wrap))
