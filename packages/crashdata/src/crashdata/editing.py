"""A data file's text for an editor's save (#1167, docs/editors.md "The editor's backend").

render(path, value) is what an editor writes back to one of the data
files the schemas describe: the tool's own writer where the file is in
that writer's layout (crashdata.levels' room.json and levels.json, the
graphics manifest's sheet.json), which is what the tool would write;
otherwise the file's own layout (crashdata.common.jsonfmt.dumps_like).
Either way a value the file already has gives back the file's bytes.
"""
import json
from pathlib import Path

from crashdata import schemas
from crashdata.common import jsonfmt


def _room(v):
    from crashdata import levels
    return levels.room_json_text(v)


def _levels(v):
    from crashdata import levels
    return levels.levels_json_text(v)


def _indent2(v):
    return json.dumps(v, indent=2) + "\n"


# The tools' writers, by the schema of the files they write.
WRITERS = {
    "room.schema.json": _room,
    "levels.schema.json": _levels,
    "sprite_sheet.schema.json": _indent2,
}


def _writer_text(writer, value):
    try:
        return writer(value)
    except (KeyError, TypeError, AttributeError, ValueError, IndexError):
        return None


def render(path, value, old_text=None, schema=None):
    """The text of `value` as `path`'s new contents (old_text: its text,
    read from path when not given; None for a new file)."""
    path = Path(path)
    if old_text is None and path.exists():
        old_text = path.read_text(encoding="utf-8")
    schema = schema or schemas.schema_for(path)
    writer = WRITERS.get(schema)
    if old_text is not None:
        try:
            old = json.loads(old_text)
        except json.JSONDecodeError:
            old = None
        if old is not None and jsonfmt.same(old, value):
            return old_text
        if writer is not None and old is not None and _writer_text(writer, old) == old_text:
            text = _writer_text(writer, value)
            if text is not None:
                return text
        if old is not None:
            return jsonfmt.dumps_like(old_text, value)
    if writer is not None:
        text = _writer_text(writer, value)
        if text is not None:
            return text
    return json.dumps(value, indent=2, ensure_ascii=False) + "\n"


__all__ = ["WRITERS", "render"]
