# `InitHudTextWidget`: matched, first-try

`InitHudTextWidget` (ROM `0x08028B7C`, real bytes previously in
`asm/code_3_2_20_8b7c.s`) has no tracked GitHub issue - it sits
immediately after GitHub issue #46's fully-matched
`sub_8028AC4`-`sub_8028B58` run (`src/graphics/hud_icon_widget5.c`,
"trivial `struct icon_manager` getter/setter/trampoline-forwarder
family") and was explicitly called out in that chunk's own write-up as
next-but-out-of-scope.

It's a minimal constructor for the same `struct icon_manager` (see
`include/icon_manager.h`) that `hud_icon_widget5.c`'s whole file
already operates on: it points the manager's `record` field at a fixed
ROM table, then optionally registers the object for teardown -

```c
void InitHudTextWidget(struct icon_manager *self, u32 flags)
{
    self->record = (struct icon_record *)gStaticData_087E4DAC;
    if (flags & 1) {
        sub_8026ED0(self);
    }
}
```

`gStaticData_087E4DAC` is the same ROM table
`src/graphics/actor_aabb_setup.c`'s `sub_803AFF0`/`sub_803B024` and
`src/graphics/hud_icon_widget_8a78.c`'s `sub_8028A78` already point a
`record` field at, for a different self object each time - part of the
`gStaticData_087E3BEC`-family "per-type descriptor" convention
`docs/rom_map.md` documents at length, and the same conditional
`sub_8026ED0(self)` "register for teardown if bit 0 of flags is set"
idiom used throughout this codebase.

Matched byte-exact on the **first try**, with plain struct field
access - no inline-asm address anchor needed, unlike `sub_8028A78`'s
own `record` write or `sub_803AFF0`/`sub_803B024`'s double-store
variant (both documented as needing one to stop gcc's CSE/dead-store
elimination from collapsing a repeated address computation the ROM
computes fresh). The difference here: `InitHudTextWidget` only ever
computes `self + offsetof(icon_manager, record)` once in the whole
function, so there's nothing for gcc to collapse against - the natural
codegen already lands the address add in `r2` exactly like the ROM.

## File structure

Folded into `src/graphics/hud_icon_widget5.c` (immediately after
`sub_8028B58`) rather than getting its own object file, since it's
directly contiguous with that file's existing ROM range and shares the
same `struct icon_manager`/`gStaticData_087E3BEC`-family conventions -
see `docs/workflow.md`'s "one `.c` file per contiguous ROM region"
rule. The function's block was cut out of `asm/code_3_2_20_8b7c.s`,
which now starts at `InitObjTileFreeList` (`0x08028BA0`) instead;
`ldscript.txt` needed no change since `hud_icon_widget5.o` already
linked directly before `code_3_2_20_8b7c.o`. `tools/report_units.py`'s
`hud` category entry for `0x08028AC4` updated to note the extended
range; the old standalone `0x08028B7C` entry removed.

Verified via a full clean `rm -rf build && make NON_MATCHING=1 report`
and `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`La suma coincide`).
