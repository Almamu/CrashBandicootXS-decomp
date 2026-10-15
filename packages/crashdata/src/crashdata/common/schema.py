"""JSON Schemas of the data files, written from the model (#1167, docs/editors.md).

Each domain of crashdata describes its files' structure here, as JSON
Schema (draft 2020-12) built from its own field lists and limits (the
keys a loader reads, its ranges), and its loaders validate the files
with that schema before their own checks: what the schema rejects, the
build rejects, so the two can't drift apart. The schemas are written to
packages/crashdata/schemas/ (crashdata.schemas, `tools/schemas.py`) for
editors; the build doesn't read those files.

The builders below make the schema dicts; validate() checks a value
against one and gives Diagnostics with each problem's JSON pointer. It
implements the part of the standard the schemas use: type, enum, const,
minimum/maximum, minLength/pattern, properties/required/
additionalProperties/patternProperties/propertyNames, items/prefixItems/
minItems/maxItems, anyOf/oneOf, $ref to "#/$defs/NAME", and ignores the
annotations (title, description, default, $comment).
"""
import re

from crashdata.common import diagnostics as D

DIALECT = "https://json-schema.org/draft/2020-12/schema"
# The schemas' $id prefix: their place in the repository.
BASE_ID = "https://github.com/Almamu/CrashBandicootXS-decomp/raw/main/packages/crashdata/schemas/"

U8 = (0, 0xFF)
U16 = (0, 0xFFFF)
S16 = (-0x8000, 0x7FFF)
U32 = (0, 0xFFFFFFFF)
S32 = (-0x80000000, 0x7FFFFFFF)
C_NAME = r"^[A-Za-z_][A-Za-z0-9_]*$"
G_SYMBOL = r"^g[A-Za-z0-9_]+$"


# ---------------------------------------------------------------------------
# Builders

def _doc(s, description):
    if description:
        s["description"] = description
    return s


def integer(lo=None, hi=None, description=None):
    """An integer (not a boolean), lo..hi inclusive (a (lo, hi) pair as lo)."""
    if isinstance(lo, tuple):
        lo, hi = lo
    s = {"type": "integer"}
    if lo is not None:
        s["minimum"] = lo
    if hi is not None:
        s["maximum"] = hi
    return _doc(s, description)


def string(pattern=None, enum=None, min_length=None, description=None):
    s = {"type": "string"}
    if enum is not None:
        s["enum"] = list(enum)
    if pattern is not None:
        s["pattern"] = pattern
    if min_length is not None:
        s["minLength"] = min_length
    return _doc(s, description)


def boolean(description=None):
    return _doc({"type": "boolean"}, description)


def null():
    return {"type": "null"}


def const(v, description=None):
    return _doc({"const": v}, description)


def enum(values, description=None):
    return _doc({"enum": list(values)}, description)


def array(items=None, min_items=None, max_items=None, length=None, prefix=None, description=None):
    """A list of `items`; `length` sets both bounds; `prefix` the schemas of
    the first items (then `items` is the rest's, or False for none)."""
    s = {"type": "array"}
    if prefix is not None:
        s["prefixItems"] = list(prefix)
    if items is not None:
        s["items"] = items
    if length is not None:
        min_items = max_items = length
    if min_items is not None:
        s["minItems"] = min_items
    if max_items is not None:
        s["maxItems"] = max_items
    return _doc(s, description)


def obj(properties=None, required=(), additional=False, pattern_properties=None, property_names=None,
        description=None):
    """An object of `properties` ({name: schema}), the `required` ones
    required; other keys rejected unless `additional` (True, or the
    schema of their values) or matched by `pattern_properties`."""
    s = {"type": "object"}
    if properties:
        s["properties"] = dict(properties)
    if required == "all":
        required = list(properties)
    if required:
        s["required"] = list(required)
    if pattern_properties:
        s["patternProperties"] = dict(pattern_properties)
    if property_names is not None:
        s["propertyNames"] = property_names
    if additional is not True:
        s["additionalProperties"] = additional
    return _doc(s, description)


def with_keys(props, keys):
    """props ({key: schema}) with every key of the model's list `keys`: one
    the schema doesn't describe yet is allowed with any value, so a key
    added to the model is accepted (and shows in the regenerated schema,
    which `tools/schemas.py --check` asks for) until it is described."""
    out = dict(props)
    for k in keys:
        out.setdefault(k, {"$comment": "read by the model, not described here yet"})
    return out


def any_of(*schemas, description=None):
    return _doc({"anyOf": list(schemas)}, description)


def one_of(*schemas, description=None):
    return _doc({"oneOf": list(schemas)}, description)


def nullable(s):
    return any_of(s, null())


def ref(name):
    return {"$ref": "#/$defs/%s" % name}


def document(name, title, root, defs=None, description=None):
    """A whole schema: its $schema, $id (BASE_ID + name), title, the root's
    keywords and the $defs."""
    s = {"$schema": DIALECT, "$id": BASE_ID + name, "title": title}
    if description:
        s["description"] = description
    s.update({k: v for k, v in root.items() if k != "description"})
    if defs:
        s["$defs"] = dict(defs)
    return s


# ---------------------------------------------------------------------------
# Validation

_TYPES = {
    "object": lambda v: isinstance(v, dict),
    "array": lambda v: isinstance(v, list),
    "string": lambda v: isinstance(v, str),
    "integer": lambda v: isinstance(v, int) and not isinstance(v, bool),
    "number": lambda v: isinstance(v, (int, float)) and not isinstance(v, bool),
    "boolean": lambda v: isinstance(v, bool),
    "null": lambda v: v is None,
}
_ANNOTATIONS = {"$schema", "$id", "$defs", "$comment", "title", "description", "default", "examples",
                "deprecated", "readOnly", "writeOnly"}
_KNOWN = _ANNOTATIONS | {"type", "enum", "const", "minimum", "maximum", "minLength", "maxLength", "pattern",
                         "properties", "required", "additionalProperties", "patternProperties", "propertyNames",
                         "items", "prefixItems", "minItems", "maxItems", "anyOf", "oneOf", "$ref"}


def _show(v):
    import json
    t = json.dumps(v)
    return t if len(t) <= 60 else t[:57] + "..."


def _errors(s, v, path, root, out):
    """Append (path, message) for each way v breaks s."""
    if s is True or s == {}:
        return
    if s is False:
        out.append((path, "not allowed here"))
        return
    if "$ref" in s:
        name = s["$ref"].rsplit("/", 1)[1]
        _errors(root["$defs"][name], v, path, root, out)
    unknown = set(s) - _KNOWN
    assert not unknown, "schema keywords validate() doesn't implement: %s" % ", ".join(sorted(unknown))
    t = s.get("type")
    if t is not None:
        types = t if isinstance(t, list) else [t]
        if not any(_TYPES[x](v) for x in types):
            out.append((path, "%s isn't %s" % (_show(v), " or ".join("a boolean" if x == "boolean" else
                                                                      "an " + x if x[0] in "aeiou" else "a " + x
                                                                      for x in types))))
            return
    if "const" in s and not _same(v, s["const"]):
        out.append((path, "%s isn't %s" % (_show(v), _show(s["const"]))))
    if "enum" in s and not any(_same(v, e) for e in s["enum"]):
        out.append((path, "%s isn't one of %s" % (_show(v), ", ".join(_show(e) for e in s["enum"]))))
    if _TYPES["number"](v):
        if "minimum" in s and v < s["minimum"] or "maximum" in s and v > s["maximum"]:
            lo, hi = s.get("minimum"), s.get("maximum")
            rng = "%s to %s" % (lo, hi) if lo is not None and hi is not None else \
                ("at least %s" % lo if lo is not None else "at most %s" % hi)
            out.append((path, "%s is out of range (%s)" % (v, rng)))
    if isinstance(v, str):
        if "minLength" in s and len(v) < s["minLength"]:
            out.append((path, "%s is shorter than %d characters" % (_show(v), s["minLength"])))
        if "maxLength" in s and len(v) > s["maxLength"]:
            out.append((path, "%s is longer than %d characters" % (_show(v), s["maxLength"])))
        if "pattern" in s and not re.search(s["pattern"], v):
            out.append((path, "%s doesn't match %s" % (_show(v), s["pattern"])))
    if isinstance(v, dict):
        props = s.get("properties", {})
        for k in s.get("required", []):
            if k not in v:
                out.append((path, "no \"%s\"" % k))
        pats = [(re.compile(p), sub) for p, sub in s.get("patternProperties", {}).items()]
        extra = s.get("additionalProperties", True)
        for k, x in v.items():
            if "propertyNames" in s:
                _errors(s["propertyNames"], k, path + (k,), root, out)
            matched = False
            if k in props:
                matched = True
                _errors(props[k], x, path + (k,), root, out)
            for p, sub in pats:
                if p.search(k):
                    matched = True
                    _errors(sub, x, path + (k,), root, out)
            if not matched:
                if extra is False:
                    known = sorted(props)
                    out.append((path + (k,), "unknown key \"%s\"%s" % (
                        k, " (the keys are %s)" % ", ".join(known) if known and len(known) <= 16 else "")))
                elif extra is not True:
                    _errors(extra, x, path + (k,), root, out)
    if isinstance(v, list):
        if "minItems" in s and len(v) < s["minItems"] or "maxItems" in s and len(v) > s["maxItems"]:
            lo, hi = s.get("minItems"), s.get("maxItems")
            want = ("%d" % lo if lo == hi else "%d to %d" % (lo, hi)) if lo is not None and hi is not None else \
                ("at least %d" % lo if lo is not None else "at most %d" % hi)
            out.append((path, "%d items, not %s" % (len(v), want)))
        prefix = s.get("prefixItems", [])
        for i, x in enumerate(v):
            if i < len(prefix):
                _errors(prefix[i], x, path + (i,), root, out)
            elif "items" in s:
                _errors(s["items"], x, path + (i,), root, out)
    for key in ("anyOf", "oneOf"):
        if key not in s:
            continue
        results = []
        for sub in s[key]:
            e = []
            _errors(sub, v, path, root, e)
            results.append(e)
        passing = sum(1 for e in results if not e)
        if passing == 0:
            # the alternative that came closest (fewest errors, deepest first)
            best = min(results, key=lambda e: (len(e), -max(len(p) for p, _ in e)))
            if len(s[key]) > 1 and all(len(e) == 1 and e[0][0] == path for e in results):
                out.append((path, "; or ".join(e[0][1] for e in results)))
            else:
                out.extend(best)
        elif key == "oneOf" and passing > 1:
            out.append((path, "%s matches more than one of the alternatives" % _show(v)))


def _same(a, b):
    """JSON equality: 1 isn't true."""
    if isinstance(a, bool) or isinstance(b, bool):
        return isinstance(a, bool) and isinstance(b, bool) and a == b
    return a == b


def validate(schema, value, file=None, code="schema"):
    """The Diagnostics (errors) of value against schema, each with its JSON
    pointer."""
    out = []
    _errors(schema, value, (), schema, out)
    return [D.Diagnostic(msg, file, D.pointer(path), D.ERROR, code) for path, msg in out]


def check(schema, value, file=None):
    """Raise a DataError if value breaks schema: its errors, each with its
    line and column in the file."""
    diags = validate(schema, value, file)
    if diags:
        if file is not None:
            D.locate(diags, D.file_texts(diags))
        raise D.DataError(diags)
    return value
