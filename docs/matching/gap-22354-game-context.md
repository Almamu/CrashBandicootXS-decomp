# 0x08022354-0x080225A0: the game-context destructor and the text-list pager

These are the two functions between issue #33's chunk
(`graphics_loading_21d80.c`, which ends with the game-context
constructor `sub_8022230`) and `UpdateGameFrame` (`game_loop55.c`). No
issue covered them. Both are now real C in
`src/graphics/graphics_loading_22354.c`, and `asm/code_3_2_17_22354.s` is
retired. Both match under either compiler. The file is built with the
current agbcc like its neighbours.

## `sub_8022354` - UNUSED

This is the destructor that pairs with `sub_8022230`. It calls
`FreeVramDmaQueue`, then destroys every singleton the constructor built,
each with flags 3 when it is non-NULL:

- `gUnknown_03001300` (`sub_8006AF4`)
- `gUnknown_030012FC` (`sub_8006CD0`)
- `gUnknown_03001304` (freed directly)
- the audio context `gUnknown_030012BC` (`sub_8001C64`, then `sub_8001C04`)
- the two icon managers `gUnknown_030012E0`/`gUnknown_030012DC`, through
  their method table
- `gUnknown_030012CC`/`D0`/`B8`/`B4`/`C8`

It then clears the context pointer `gUnknown_03000828` and frees `self`
on bit 0 of `flags`, gcc 2.x's deleting-destructor convention. Nothing
in the ROM calls it. There is no `bl` to it, no word `0x08022355`, and no
symbol reference in `asm/`/`src/`. That fits: the game never returns from
`MainLoop`.

The icon managers' destructor entry is the method record at +0x08 of
their `record`. `struct icon_record` (`include/icon_manager.h`) now names
it `destroy`, taken out of the leading `unused_00` padding. The call goes
through `_call_via_r2`, linked with the usual `.set`
alias.

`sub_8001C64` takes no argument in `audio_context.c`, but the ROM loads
the audio context into r0 before calling it, so the local prototype
passes it.

## `PlayCutscene(self, idx)`

`docs/rom_map.md` already read this one ("Into `graphics_loading`'s
remainder: a BG2-affine screen-effect setup"). It sets the DISPCNT
shadow `gUnknown_03001288` to 0x40 and calls the
`sub_8001524`/`sub_80015B0`/`sub_80015E0` mode setters. It then zero-fills
the 0x200-byte BG palette with DMA3 and resets the BG2 affine registers
to identity. Next it reloads the tile cache (`sub_8006EA8`) and resets
the font icon manager `gUnknown_030012DC` (tile base 0x200, then its
slot-6 method). Finally it runs a stack-allocated `InitCutscenePlayer` text
pager (`game_loop57.c`) over list `gCutscenes[idx]`, with the
per-level page table `gCutsceneTexts[gLanguage][idx]` and a
fixed box (7, 0x7E) + (0xE4, 0x1E), until `RunCutscenePlayer` returns. It
restores the shadow and destroys the pager.

Getting it byte-exact took several source-shape choices, each checked
against the alternatives:

- **One aggregate local** `{ box; fill; pager; }` instead of three
  locals. With `pager` as its own local, every field store after
  `InitCutscenePlayer(&pager)` goes through the register holding `&pager`
  (`str r0, [r4, #20]`). The ROM stores them sp-relative
  (`str r0, [sp, #40]`) while keeping `&pager` in r5 only for the calls.
  As fields of one frame object, the field addresses are frame-relative
  constants. The layout also reproduces the ROM's frame exactly: box at
  sp+0, the DMA fill halfword at sp+0x10, pager at sp+0x14, 0x3C bytes in
  all.
- **The box halves come from `MakeVec(x, y)`**, an inline function that
  builds the pair with a brace initializer from its parameters. The ROM
  materializes each half in r0:r1 and stores the pair
  (`movs r0,#7; movs r1,#0x7e; str r0,[sp]; str r1,[sp,#4]`). A constant
  initializer is copied out of `.rodata` instead. Field-by-field
  assignment of a local makes it live at entry (a partial set), and
  global allocation puts it in r2:r3.
- **`dispcnt = &gUnknown_03001288` as a local, assigned first**, so the
  address is loaded (into r8) before `zero`/`mode` are set, as in the
  ROM. **`zero` and `mode` are variables too**: both constants live in
  callee-saved registers (r4 and sb) across the mode-setter calls.
- **`tileBase` as a local assigned after the manager pointer is loaded.**
  The ROM materializes 0x200 before the +0x108 address and derives the
  +0x130 `record` offset from it (`subs r2, #0xd0`). A literal at the
  store derives 0x200 from 0x108 instead.
- **The box copy is spelled out.** The ROM copies `box` into
  `pager.box` with loads in pairs and the two x words stored sp-relative
  (`str r0, [sp, #44]`, `str r0, [sp, #52]`), but the two y words stored
  through one pointer register (`add r2, sp, #44; ...; str r1, [r2, #4];
  ... str r1, [r2, #12]`). A struct or `DImode` assignment stores all
  four words sp-relative. A pointer used for all four stores keeps
  offsets 8 and 12 on the pointer. What matches is a `s32 *d =
  &pager.box.pos.x` used for words 0, 1 and 3, with word 2 stored as the
  field, and with four separate temporaries for the loads (reusing two
  temporaries swaps the load registers with the pointer's).

The DMA uses `DmaSet` with `BG_PLTT_SIZE / 2` halfwords. The palette
fill value goes through a pointer to the frame's `fill` halfword, which
gives the ROM's `add r0, sp, #16; strh r4, [r0]` (a plain
`f.fill = zero` stores it sp-relative).

Verified via a full clean `rm -rf build && make NON_MATCHING=1 report`
(no warnings from the new file) and a full clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` (`crashbandicootxs.gba: OK`).
