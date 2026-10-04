"""Build sound/sfx_table.bin from the editable sound/sfx_table.json source.

This is the GAX2 "sound effect" trigger table (originally at ROM address
0x0816AA6C): 99 fixed-size entries, one per sound effect ID. sub_8001854 in
asm/code_3.s (called ~264 times from all over the game's logic - jumping,
menus, hits, pickups, etc.) looks up the SFX ID in this table and uses it to
steal a mixing voice and play a note from the GAX2 sound-effect data set
(sound/gax_sfx_manifest.json, sound/sfx_samples/ - not the music's pool; see
docs/audio.md, "Sound effects").

Each entry is 3 little-endian signed 32-bit words:
  slot_id      - the sound-effect instrument to play (1-87; written into the
                 voice at offset 0x25 by GAX_fx_ex)
  pitch_offset - really GAX_fx_ex's priority argument (struct
                 SfxTableEntry.chanArg), the voice-steal threshold
  volume       - 8.8 fixed-point volume scale (0x100 == 1.0)

Field names describe what's empirically observable from the data and the
code that reads it; the deeper mixer semantics of slot_id/pitch_offset
haven't been fully reversed.
"""
import json
import struct
import sys

ENTRY_COUNT = 99
ENTRY_SIZE = 12


def build(json_path, out_path):
    manifest = json.load(open(json_path))
    entries = manifest['entries']
    assert len(entries) == ENTRY_COUNT, f"expected {ENTRY_COUNT} entries, got {len(entries)}"

    out = bytearray()
    for e in entries:
        out += struct.pack('<iii', e['slot_id'], e['pitch_offset'], e['volume'])

    assert len(out) == ENTRY_COUNT * ENTRY_SIZE
    open(out_path, 'wb').write(out)


if __name__ == '__main__':
    out_path = sys.argv[1]
    json_path = sys.argv[2] if len(sys.argv) > 2 else 'sound/sfx_table.json'
    build(json_path, out_path)
