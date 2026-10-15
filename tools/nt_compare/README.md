# XS ↔ N-Tranced code comparison

One-off feasibility tooling: finds each Crash XS function (from this
repo's build) in a Crash Bandicoot 2: N-Tranced (Europe) ROM
(`sha1: ee9234ca15ba2caa71fad7d6af71e27b2030ae0b`).

```
make compare
python3 tools/nt_compare/match.py path/to/ntranced.gba build/nt_compare.json
python3 tools/nt_compare/report.py build/nt_compare.json --files
```

How it matches (Thumb only; the IWRAM ARM code was checked by hand):

- Every halfword gets a masked value (BL offsets and pointer-looking
  literal-pool words zeroed) and a shape token (instruction format and
  opcode, no registers or immediates).
- **exact**: the masked bytes are found in N-Tranced: same source, same
  codegen, only addresses differ.
- Otherwise the function is located by voting on shape 6-grams, then
  refined between ordered anchors (unique high-confidence matches, kept
  in link order by a longest increasing subsequence). The shape score
  (difflib) gives **near** (≥ 0.95, mostly only struct offsets or
  constants differ), **similar** (≥ 0.80, same logic with insertions),
  **weak** (≥ 0.60) and **none**.

Sizes are the XS functions' sizes. Functions under 12 bytes are too small
to place reliably and mostly end up as `exact` duplicates or `none`.

## Data, structs, names and new code

```
NT=path/to/ntranced.gba
python3 tools/nt_compare/xrefs.py $NT build/nt_compare.json build/nt_xrefs.json
python3 tools/nt_compare/rooms_roundtrip.py $NT 081d0430 29     # XS control: crashbandicootxs.gba 0816c86c 25
python3 tools/nt_compare/nt_symbols.py build/nt_compare.json build/nt_xrefs.json > build/nt_symbols.txt
python3 tools/nt_compare/nt_new_code.py $NT build/nt_compare.json --list
```

- `xrefs.py`: the XS -> NT address map, from the literal-pool words and
  BL targets at aligned positions of matched function pairs (gLevelTable
  is NT `0x081D0430`).
- `rooms_roundtrip.py`: walks the level table with the XS structs and runs
  every room through `tools/levels.py`'s chunk codec, byte-compared, plus
  the entity-list invariants of docs/levels.md.
- `nt_symbols.py`: XS names for NT addresses (`shared` = several XS
  functions are the same NT code).
- `nt_new_code.py`: NT functions with no XS counterpart, attributed to the
  XS object they are linked next to.

Struct layouts (`struct_diff.py`, `struct_report.py`) need the XS objects
rebuilt with debug info, which doesn't change the code (checked on all
242 objects): every compile command from `make -n -B crashbandicootxs.elf`
again with `-gstabs` (line table) into one directory and with `-ggdb`
(DWARF 2, every class's layout; append `.section .debug_abbrev` /
`.byte 0` to each `.s` so binutils can read it, and dump it with
`readelf --debug-dump=info` to `<obj>.dw`) into another, `-Werror`
dropped, the two ARM objects left out. Then:

```
python3 tools/nt_compare/struct_diff.py $NT build/nt_compare.json <stabs dir> build/nt_structs.json <dwarf dir>
python3 tools/nt_compare/struct_report.py build/nt_structs.json
```

Each aligned load/store of a matched pair gives the XS and NT offset; the
member is the one at that offset and size whose name is on the XS source
line (or `this`'s only member there). `adds rX, #k` folded into the base
is added back. Members seen once or without a 60% majority are low
confidence; array elements accessed through computed indices show up as
false insertions.
