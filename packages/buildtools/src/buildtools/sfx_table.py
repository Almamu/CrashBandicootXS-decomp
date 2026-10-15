"""tools/sfx_table.py: the command line of crashdata.audio.sfx_table (#1167)."""
import sys

from crashdata.audio import sfx_table
from crashdata.common import diagnostics as D


def run():
    if len(sys.argv) >= 3 and sys.argv[1] == '--constants':
        sfx_table.write_constants(sys.argv[3] if len(sys.argv) > 3 else 'games/xs/sound/sfx_table.json', sys.argv[2])
        sys.exit(0)
    out_path = sys.argv[1]
    json_path = sys.argv[2] if len(sys.argv) > 2 else 'games/xs/sound/sfx_table.json'
    sfx_table.build(json_path, out_path)


def main():
    """run(), or with --json its errors and warnings as one JSON report
    (crashdata.common.diagnostics, docs/editors.md)."""
    return D.json_cli('sfx_table', run)


if __name__ == '__main__':
    main()
