"""tools/sfx_table.py: the command line of crashdata.audio.sfx_table (#1167)."""
import sys

from crashdata.audio import sfx_table


def main():
    if len(sys.argv) >= 3 and sys.argv[1] == '--constants':
        sfx_table.write_constants(sys.argv[3] if len(sys.argv) > 3 else 'games/xs/sound/sfx_table.json', sys.argv[2])
        sys.exit(0)
    out_path = sys.argv[1]
    json_path = sys.argv[2] if len(sys.argv) > 2 else 'games/xs/sound/sfx_table.json'
    sfx_table.build(json_path, out_path)


if __name__ == '__main__':
    main()
