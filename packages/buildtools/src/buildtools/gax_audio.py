"""tools/gax_audio.py: the command line of crashdata.audio.gax (#1167)."""
import os
import sys

from crashdata.audio import gax
from crashdata.common import diagnostics as D


def run():
    if len(sys.argv) == 3 and sys.argv[1] == '--sfx':
        gax.sfx_pool()[0].report()
        alloc, _ = gax.build_sfx(gax.SFX_MANIFEST, gax.SFX_SAMPLES_DIR)
        n = alloc.write(sys.argv[2], gax.asm_path(sys.argv[2]))
        print(f"wrote {sys.argv[2]}: {n} bytes")
        sys.exit(0)
    if len(sys.argv) == 3 and sys.argv[1] == '--constants':
        gax.write_constants(os.path.join(gax.SOUND_DIR, 'gax_manifest.json'), sys.argv[2])
        sys.exit(0)
    if len(sys.argv) == 3 and sys.argv[1] == '--table':
        gax.write_song_table(os.path.join(gax.SOUND_DIR, 'gax_manifest.json'), sys.argv[2])
        sys.exit(0)
    if len(sys.argv) not in (2, 4):
        print(f"Usage: {sys.argv[0]} OUTPUT.bin [LAYOUT.bin SONGS.h]  (music block, default layout, song offsets)")
        print(f"       {sys.argv[0]} --sfx OUTPUT.bin                (sound-effect set)")
        print("       (each OUTPUT.bin comes with OUTPUT.s: its bytes, the pointers as relocations)")
        print(f"       {sys.argv[0]} --constants SONGS.h             (constants/songs.h: the song IDs)")
        print(f"       {sys.argv[0]} --table SONG_TABLE.inc          (gSongTable, in song id order)")
        sys.exit(1)
    gax.build(
        os.path.join(gax.SOUND_DIR, 'gax_manifest.json'),
        os.path.join(gax.SOUND_DIR, 'songs'),
        os.path.join(gax.SOUND_DIR, 'samples'),
        sys.argv[1],
        *sys.argv[2:],
    )


def main():
    """run(), or with --json its errors and warnings as one JSON report
    (crashdata.common.diagnostics, docs/editors.md)."""
    return D.json_cli('gax_audio', run)


if __name__ == '__main__':
    main()
