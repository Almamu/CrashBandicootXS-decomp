# Issue #22: 0x08017AB0-0x08017ECC (`sub_8017AB0`)

Follow-up to `docs/matching/issue-22-0x08017a44-actor.md`, which left
this one function - the whole gap between `actor_part27.c` (ends
0x08017AAC) and `actor_part27b.c` (starts 0x08017ECC) - completely
untouched ("out of scope... given their size"). `docs/rom_map.md`'s own
whole-ROM pass had already read it at a high level: "1052 B, 3-state
dispatch gated by `gUnknown_030012D8[0x104]` and a bit test". This pass
transcribes it in full and confirms/expands that read into a complete
semantic account.

## Callers

No caller in already-decompiled `src/` calls `sub_8017AB0` directly
(`grep -rn "sub_8017AB0("` over `src/` is empty) - it's presumably
invoked through a function-pointer table still raw/unexamined
elsewhere, the same "no free caller-side context" situation as most of
this large action-table family.

## Semantics: the 3-state dispatch map

`self` (r4) is the large per-level "player/action" object this whole
region shares (`self+0xc` the per-category table pointer, records
`{s16 offset; u8 pad[2]; void *fn}` at fixed offsets 0x20/0x50/0x58,
fired through `sub_803AD80`/`sub_803AD84`); `other` (r5) is the "part"
object passed alongside it, with an *analogous* table of its own at
`other+0x18` (different record offsets: 0x28, 0x48, 0x68 are all seen
in this one function, each presumably a different logical "slot").
`gUnknown_030012D8` is the player/camera-viewport object docs/rom_map.md
already names - its `+0x104` byte is a "player busy" gate, and its own
`+0x18`-table feeds two more trampoline calls.

**Prelude (every state)**: if `self+0x1c` (a signed word - timestamp or
sentinel) equals `-1`, fires `sub_8017F14(self, other, 1)` and resets it
to `0`.

Dispatches on `self+8` (0/1/2; anything else returns immediately):

- **State 0** (`self+8==0`): bails if the player's `+0x104` busy gate is
  set. Otherwise falls straight into the **shared tail** below (the
  "activate/deactivate table entry 3" block), entering it exactly as if
  `other+0x38` were already known set and the busy gate already known
  clear.
- **State 1** (`self+8==1`): the largest branch.
  1. **Trampoline trio, conditionally skipped**: unless the player's
     `+0x104` gate is set *and* its `+0x44`-object's own `+8` field is
     `!=0x1e`, fires the `0x50`-indexed trampoline (mode 2), the
     `0x20`-indexed one (mode 0), then the `0x58`-indexed one (mode 0).
  2. **Busy-toggle latch**: runs a `sub_803AD7C` "occupied" probe
     against `other+0x18`'s own table (offset 0x28) up to twice - the
     ROM calls it, and only if the result is "free" *and*
     `self+0x20`'s own latch was already `0` does it stamp
     `self+0x1c = gUnknown_0300082C` (the game tick counter) and set
     `self+0x20 = 1`; otherwise it re-probes the *same* address a
     second time and, on "free" and `self+0x20 != 0`, stamps the
     timestamp again and clears `self+0x20` back to the fresh result.
     (Two probes of the same condition, not simplifiable to one - kept
     as the ROM has it.)
  3. **Timeout re-fire**: if `self+0x1c != 0` and
     `gUnknown_0300082C - self+0x1c > 0x3c` ticks, resets `self+0x1c` to
     `0`, then - keyed on `other+0x28` bit 4 and `self+0x20` - either
     does nothing (bit 4 set), calls `sub_8017F14(self, other, 2)`
     (bit 4 clear, `self+0x20==0`), or `sub_8017F14(self, other, 1)`
     (bit 4 clear, `self+0x20!=0`).
  4. **Screen-relative flag reset (entry 3)**: two independent gates,
     each comparing the player's X position against `other`'s X
     (`player.x < other.x` and `player.x <= other.x + 0xa00`), each
     re-checking `other+0x28` bit 4 and, when clear, rewriting that byte
     (`(flags & ~0x11) | 0x10` for the first, `flags & ~0x11` for the
     second) and firing the `0x58`-indexed trampoline (modes 3, then 1).
  5. **In-bounds check**: `abs(player.x - other.x) <= 0x27FF` and
     `abs(player.y - other.y) <= 0x31FF` (Q8 fixed point) fires
     `sub_8017F14(self, other, 0)`, the `0x20`-indexed trampoline
     (mode 2), and the `0x50`-indexed one (mode 1), then returns.
  6. **Out of bounds - "spawn + scan" cluster**: builds an AABB via
     `sub_8007B98(&box, other)`, unpacks it into
     `sub_8008A40(gUnknown_030012F0, box.x, box.y, box.w, box.h, 0,
     other)`, then walks the entire `gUnknown_0300130C` object list. For
     each `entry` whose own table (`entry+0x18`, offset 0x48) probes
     `==3` via `sub_803AD7C`, and whose Q8>>8 position is within a
     `0x27`/`0x3b` box of `other`, and whose `entry+0x4d` byte has bit
     `0x7f` clear: a `entry+0x4e` tag of `0xe`/`0x13`/`0x14`/`0x15`/`0xa`
     calls `sub_800EEF0(entry, 0)`; any other tag instead gates
     `sub_800E888(entry, 1)` behind `sub_8010908(entry, tag)`.
- **State 2** (`self+8==2`): if `other+0x30==8` and `other+0x34==0` and
  the same in-bounds Q8 check as state 1 step 5 passes, fires
  `sub_803AD88(player + player's-own-0x18-table[0x68].offset, 0, 1, 0)`
  and returns. Otherwise (either gate fails, or the in-bounds check
  fails) falls into the **shared tail**.

**Shared tail** (state 0 direct, or state 2's "otherwise" exit): gated
on `other+0x38` (checked only on the state-2 path - state 0 skips
straight past it) and the player's `+0x104` busy bit:

- `other+0x38==0`: return (state-2-only path).
- Busy bit clear, `other+0x28` bit 4 clear: fires the `0x58`-indexed
  trampoline (mode 3), *or* - bit 4 set - `sub_8017F14(self, other, 1)`
  instead; either way then the `0x50`-indexed trampoline (mode 0) and
  the `0x20`-indexed one (mode 1). (The "activate" shape.)
- Busy bit set (state-2-only path): the `0x50`-indexed trampoline
  (mode 2), the `0x20`-indexed one (mode 0), then the `0x58`-indexed one
  (mode 0). (The "deactivate" shape.)

## Why NAKED, not real C

Two separate blocks each independently reproduce a gcc-2.9 codegen gap
this project has already spent a full pass chasing down elsewhere and
confirmed unclosable from any C phrasing tried - re-litigating either
here would just rediscover the same dead end at roughly 5x the scale:

1. **The `gUnknown_0300130C` list walk** reloads `&gUnknown_0300130C`
   from the literal pool fresh on *every* loop-condition check and every
   loop-body entry (5 do-nothing-but-reload instructions, by the ROM's
   own choice, each time). This is the exact shape
   `actor_part108.c`'s `sub_800AAEC` (`#if NON_MATCHING` branch's own
   doc comment) already tried three different phrasings against - a
   plain `for`, a cached-`&var`-inside-an-`if` idiom, and an explicit
   `goto`-loop with both the address and the dereferenced value pinned
   to fixed registers - all three either dropped the per-iteration
   reload (gcc hoists the address as loop-invariant, unlike the ROM's
   own compiler here) or matched the reload but swapped which register
   won.
2. **The `sub_8008A40(manager, boxX, boxY, boxW, boxH, unused,
   compareViewport)` call** (7 args, 3 on the stack) reproduces the
   exact "which order the compiler batches its outgoing stack-argument
   stores in" gap `actor_part81.c`'s `sub_800AB9C` (`#if NON_MATCHING`
   branch) already documents as unclosable from *any* C-level phrasing
   tried (inlined values, named locals in ROM source order, an
   `asm volatile("":::"memory")` scheduling barrier - all failed the
   same way, this compiler batches every outgoing stack-argument store
   together immediately before the `bl` regardless of source order).
   Tellingly, the *stack-argument order* this ROM call actually uses
   (`unused`, `compareViewport`, `boxH`) is byte-for-byte identical to
   that already-parked call's own order in a completely unrelated
   function - strong independent confirmation this is a fixed compiler
   policy for this exact call signature, not anything the source
   arranged.

Given both gaps are independently pre-diagnosed and pre-exhausted in
this exact codebase, and this function is ~5x the size of either prior
case, this was recognized immediately and NAKED-transcribed rather than
re-attempted from scratch.

Transcribed mechanically from `asm/code_3_2_17_17ab0.s` (now retired -
its one function moved to the new `src/graphics/actor_part27a.c`),
keeping the ROM's own `_0XXXXXXX` hex-address labels verbatim as plain,
file-local asm symbols (safe since each is a unique ROM address) - the
same approach `actor_part82.c`'s `sub_8011BD4`, `actor_part_12fbc.c`'s
five functions, and `actor_part18.c`'s `sub_801434C` all already use.
Register/argument roles for every external call (`sub_803AD80`/
`sub_803AD84`/`sub_803AD7C`/`sub_803AD88`'s base+offset+fn-pointer
trampoline convention, `sub_8007B98`/`sub_8008A40`'s AABB-relocate-then-
unpack idiom) were cross-checked against their existing signatures in
`actor_part18.c`/`actor_part81.c`/`actor_part.c` before transcription,
confirming the semantic read above rather than leaving it a guess.

## Verification

Isolated `cpp`/`agbcc`/`arm-none-eabi-as` + `objcopy` produced a
1052-byte object whose only byte differences from the ROM's own raw
bytes at 0x08017AB0 were unlinked `bl`/literal-pool relocation
placeholders (expected and resolved correctly once linked - same shape
`docs/matching/issue-17-0x08012fbc-actor.md` documents for its own
NAKED transcriptions). Wired into `ldscript.txt`/`tools/report_units.py`
in place of the retired `asm/code_3_2_17_17ab0.s`; a full clean
`rm -rf build && make NON_MATCHING=1 report` succeeds with no warnings
from the new file, and `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare` confirms
`La suma coincide`.

## Status

Byte-exact, but as a NAKED transcription - parked, not matched, per
project policy (see `docs/status/actor.md`'s "Parked - NAKED
transcription" section for the tracking entry). `tools/report_units.py`
keeps `base_object=None` for this address, matching every other NAKED
entry in this table family. Issue #22 itself stays open -
`sub_8018008`/`sub_8018400`/`sub_801865C`/`sub_80186F0`
(0x08018008-0x080186F0) remain raw/unexamined for a future pass.
