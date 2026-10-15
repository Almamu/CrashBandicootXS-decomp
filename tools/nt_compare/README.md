# XS ↔ N-Tranced comparison

Compares this repo's Crash XS build with Crash Bandicoot 2: N-Tranced
(Europe, `sha1: ee9234ca15ba2caa71fad7d6af71e27b2030ae0b`): where each XS
function is in N-Tranced and how close, the address map between the two,
XS names for N-Tranced's functions, which struct members moved, the
N-Tranced code with no XS counterpart, and whether N-Tranced's rooms go
through `tools/levels.py`. It is what #1106's numbers and the
`games/n2/gbadisasm.cfg` names come from (docs/n_tranced.md).

```
python3 tools/nt_compare/run.py [baserom_n2.gba]
```

builds XS if it isn't built and writes everything to `build/nt_compare/`
(about 30 seconds); it only reads `build/crashbandicootxs/`. `run.py` lists
the files; each step is also a script of its own (its docstring has its
usage):

| Step | Output | What |
|---|---|---|
| `debug_build.py` | `stabs/`, `dwarf/` | the XS objects with debug info (below) |
| `match.py` | `match.json` | each XS function's place in N-Tranced |
| `report.py` | `report.txt` | totals, per directory, per file (`--files`) |
| `xrefs.py` | `xrefs.json` | the XS → N-Tranced address map |
| `nt_symbols.py` | `nt_symbols.txt` | XS names for N-Tranced addresses |
| `nt_new_code.py` | `new_code.txt` | N-Tranced functions with no XS counterpart |
| `rooms_roundtrip.py` | `rooms.txt` | N-Tranced's rooms through `levels.py`'s codec |
| `struct_diff.py` | `structs.json` | struct member offsets in both games |
| `struct_report.py` | `structs.txt` | the same, per struct |

## How functions are matched (`match.py`)

Thumb only; the IWRAM ARM code is looked up byte for byte.

- Every halfword gets a masked value (BL offsets and pointer-looking
  aligned words zeroed) and a shape token (instruction format and opcode,
  no registers or immediates).
- **exact**: the masked bytes are found in N-Tranced *and* the raw bytes
  agree everywhere but the function's own relocations (its BL offsets and
  the pointer pool words its `ldr rX, [pc, #n]` load). The mask alone
  isn't enough: an instruction pair can look like a ROM pointer
  (`FlushVramDmaQueue`'s `ldrh`/`lsrs` did).
- Otherwise the function is located by voting on shape 6-grams, then
  refined between ordered anchors (unique high-confidence matches, kept
  in link order by a longest increasing subsequence). The shape score
  (difflib) gives **near** (≥ 0.95, mostly only struct offsets or
  constants differ), **similar** (≥ 0.80, same logic with insertions),
  **weak** (≥ 0.60) and **none**.

Sizes are the XS functions'. Functions under 12 bytes are too small to
place reliably and mostly end up as `exact` duplicates or `none`. Even
**exact** isn't proof that a source builds for N-Tranced: N-Tranced's
build sometimes used the other compiler for a file (docs/n_tranced.md,
"Linking a shared source into N-Tranced").

## The rest

- `xrefs.py`: at each aligned `ldr rX, [pc, #n]` of a matched pair the two
  pool words are paired, at each aligned BL the two call targets (votes);
  `gLevelTable` is N-Tranced's `0x081D0430`.
- `nt_symbols.py`: `<address> <name> <how>`; `shared` marks addresses
  several XS functions map to (template code), which the disassembly
  doesn't name.
- `nt_new_code.py`: N-Tranced's functions (BL targets and code pointers
  that start with a push) that no XS match covers, attributed to the XS
  object they are linked next to.
- `rooms_roundtrip.py`: walks the level table with the XS structs and
  runs every room through `levels.py`'s chunk codec, byte-compared, plus
  the entity-list invariants of docs/levels.md. `rooms_roundtrip.py
  crashbandicootxs.gba 0816c86c` is the XS control (39/41: the two rooms
  with `pad_overrides`).
- `xs_data_objects.py`: XS's data objects N-Tranced has whole, each one's
  `.rodata` found as one run with every pointer checked (the N-Tranced
  address of the symbol its relocation names, from `ntranced.map`, or
  into the object): the ones that can be linked into N-Tranced as they are
  (docs/n_tranced.md, "Tables shared with XS"). Needs both games built.

## Struct layouts (`debug_build.py`, `struct_diff.py`)

`debug_build.py` runs the XS build's own C++ compile commands again with
`-gstabs` (the line table) and `-ggdb` (DWARF 2, every class's layout:
g++ 2.9 leaves most classes out of stabs), into `build/nt_compare/`, and
checks each object's code is the XS build's (debug info doesn't change
it). agbcp doesn't end `.debug_abbrev` with the 0 binutils needs, so one
is appended before assembling. The two ARM objects are left out.

`struct_diff.py`: each aligned load/store of a matched pair gives the XS
and the N-Tranced offset; the member is the one at that offset and size
whose name is on the XS source line (or `this`'s only member there), and
an `adds rX, #k` folded into the base is added back. In `structs.txt`,
members seen once or without a 60% majority are low confidence, and array
elements accessed through computed indices show up as false insertions.
