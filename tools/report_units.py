#!/usr/bin/env python3
"""Builds one objdiff unit per matched src/*.c file (instead of one giant
merged unit) and writes objdiff.json with each tagged by category, so
decomp.dev can show per-category progress (Graphics/Util/System - see
docs/decomp_dev.md for why category units have to be this fine-grained:
merging non-adjacent functions into one target reintroduces the same
"hidden function" boundary bug fixed for mem_collect, since ld -r-merged
objects lose the original per-file section boundaries objdiff's size
inference relies on).

Addresses below come from a clean `make compare` (NON_MATCHING=0) build's
crashbandicootxs.map - the only reliable source, since NON_MATCHING=1
addresses drift downstream of any parked function (its imperfect
reconstruction is a different byte count than the real ROM). Four files
contain a parked function and so have a wider range here than their
NON_MATCHING=0 object alone shows - the parked function's real bytes
currently live in the neighboring raw asm/*.s chunk instead, but
expected/code_3.s already has it labelled at its true address (it was
never extracted, only parked), so slicing still works unmodified.

Still-raw regions with no `base_object` can still carry a `category` -
docs/rom_map.md's whole-ROM reconnaissance pass split most of the ROM's
remaining raw stretches into game_loop/actor/graphics_loading/audio/hud/
overlay_ui by address, high-confidence enough (individually-read
functions, confirmed landmarks, or a dominant connected component) to be
worth a decomp.dev progress category even at 0% matched - these units
get `metadata.progress_categories` but no `base_path`, so they count
toward that category's total (all unmatched) without affecting per-file
match percentages. Two categories rom_map.md found - `menu_ui` and `fx` -
don't get their own address boundary: both are individual functions
scattered *inside* another category's contiguous range rather than a
separate block of their own (menu_ui's dispatch-table functions sit
inside graphics_loading's `LoadGraphicsPackage` cluster; fx's two-function
particle-queue pair sits inside the hud gap after `MainLoop`), so
splitting them out here would require a fake, unjustified address cut -
see docs/rom_map.md for exactly which functions are which; nothing here
should be read as more precise than "the dominant category in this
range." A handful of ranges have a similar, smaller-scale mix (a
confirmed SIO/link-cable subsystem inside `overlay_ui`'s span; a couple
of generic division-routine false positives inside the GAX2 span) -
flagged inline below, folded into the dominant category regardless.
Genuinely untouched regions (nothing in rom_map.md, too small to matter)
stay `None`/uncategorized, same as before.
"""
import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BUILD_DIR = ROOT / "build" / "expected" / "units"
CODE3 = ROOT / "expected" / "code_3.s"
LEGACY = ROOT / "expected" / "legacy.s"
CORRECTIONS = ROOT / "expected" / "corrections.txt"
CODE3_START = 0x080006A8  # legacy.s covers up to here (exclusive)

AS = ["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork"]
LD = ["arm-none-eabi-ld", "-r"]

# (start_address, base_object_relpath_or_None, category_or_None)
# base_object is relative to build/crashbandicootxs/, matching the
# NON_MATCHING=1 report build (see Makefile's `report` target) - that's
# the object already compiled with parked functions included as C.
# Sorted by address; each entry's range runs to the next entry's start.
UNITS = [
    (0x08000170, "src/system/main.o", "system"),
    (0x080001CC, "src/system/memory.o", "system"),
    (0x08000544, "src/system/irq.o", "system"),
    (0x080007EC, None, None),  # code_3_1.o (sub_80007EC), still raw
    (0x080008B4, "src/util/math_util.o", "util"),
    (0x0800094C, "src/util/string_util.o", "util"),
    (0x08000AA8, "src/util/printf_util.o", "util"),  # incl. parked sub_8000CBC
    (0x08000D68, "src/util/string_util2.o", "util"),
    (0x08000E10, "src/util/rand_util.o", "util"),
    (0x08000E6C, "src/util/line_util.o", "util"),
    (0x08000EE4, "src/graphics/text_layout.o", "graphics"),  # entirely parked
    (0x0800106C, "src/util/time_util.o", "util"),
    (0x080010E0, "src/system/input_util.o", "system"),  # entirely parked
    (0x08001174, "src/system/asset_util.o", "system"),
    (0x080011F4, "src/util/word_util.o", "util"),
    (0x08001254, "src/util/line_util2.o", "util"),
    (0x080012AC, "src/graphics/fade_util.o", "graphics"),
    (0x080013FC, "src/graphics/palette_blend.o", "graphics"),
    (0x080014A4, "src/graphics/fade_screen_mode.o", "graphics"),  # fade/screen-mode utility cluster start - sub_80014A4 (parked fade-to-black loop, real bytes in asm/code_3_1_7.s) + sub_8001510 (matched); calls matched palette_blend.c - docs/rom_map.md "A fourth thing in this file"
    (0x08001524, None, "graphics"),  # sub_8001524 (parked - real bytes in asm/code_3_1_8.s); see docs/matching.md for the value-propagation gap
    (0x0800153C, "src/graphics/fade_screen_mode2.o", "graphics"),  # sub_800153C-sub_8001614 (13 fns): the rest of the fade/screen-mode cluster's DISPCNT-shadow bit accessors and commit; matched
    (0x08001624, None, "graphics"),  # sub_8001624 (parked - real bytes in asm/code_3_1_9.s, which also holds the following still-raw sub_8001640 onward); see docs/matching.md for the store+increment peephole-fusion gap
    (0x08001640, "src/graphics/aabb_util.o", "graphics"),  # sub_8001640/sub_8001688 (AABB overlap tests, X-edges inclusive vs exclusive - the latter already referenced by name from actor_part15.c's sub_800B37C) plus sub_80016D0/sub_80016DC (mem_free/mem_alloc wrappers); matched
    (0x080016EC, None, "overlay_ui"),  # pause-menu/dialog dominant component (118 fns/19.2KB); a smaller SIO/link-cable subsystem (sub_8001F50/sub_8001DB4/...) is interleaved throughout this same span, not separable by address - folded into overlay_ui regardless, see docs/rom_map.md
    (0x08006600, "src/graphics/oam_count.o", "graphics"),  # incl. parked sub_8006600
    (0x0800697C, "src/graphics/graphics.o", "graphics"),  # incl. AllocVramDmaQueue/sub_8006C28-sub_8006FB4 (a VRAM upload-cursor + tile/palette-bank asset-cache pair)/sub_8006FC8/nullsub_1/sub_8006FE4/sub_8007048/nullsub_11/sub_80070D4/sub_80070E8/sub_80070EC/sub_800710C/sub_8007110/sub_8007114/sub_8007174/sub_800719C/nullsub_12/sub_80071E4/sub_800722C/sub_8007230/sub_800725C/sub_8007278/sub_8007284/sub_8007290/sub_800729C/sub_80072A8/sub_80072B4/sub_80072C0/sub_80072CC/sub_80072D8/sub_800731C/sub_8007328/sub_8007334/sub_8007340/sub_800734C/sub_8007358/sub_8007364/sub_800736C/sub_8007374/sub_8007378/sub_800737C/sub_8007388/sub_8007398/sub_80073A0/sub_80073B0/sub_80073B4/sub_80073B8/sub_80073BC - moved here from the raw game_loop zone below since none of it is game_loop logic (see docs/matching.md). Also incl. parked sub_80073DC (NON_MATCHING C reconstruction widens this unit past its own real 0x08007634 end, per the same "parked function" convention as sub_8006600/sub_8000EE4 above)
    (0x08007634, None, "game_loop"),  # sub_8007634 itself - a giant GBA-affine-sprite-scaling variant of sub_80073DC, not yet reverse-engineered with confidence, left fully raw rather than guessed at
    (0x08007A48, "src/graphics/actor_part.o", "graphics"),  # sub_8007A48/sub_8007A84/sub_8007A98/nullsub_2/sub_8007AB4 - thin wrappers around sub_80073DC, a conditionally-free helper, an empty stub, and a part-object field initializer. Also incl. parked sub_8007B00/sub_8007B98 (NON_MATCHING C reconstructions widen this unit past its own real 0x08007B00 end, per the same "parked function" convention as sub_80073DC above) - sub_8007B00/sub_8007B98 themselves stay raw here, still linked from asm/code_3_2_2.o
    (0x08007C30, "src/graphics/actor_part2.o", "graphics"),  # sub_8007C30/sub_8007CF8 - a third and fourth AABB-for-keyframe builder (see actor_part.o's parked pair above), non-adjacent to actor_part.o since sub_8007B00/sub_8007B98 sit raw between them; matched. New object, inserted between asm/code_3_2_2.o (ends after sub_8007B98) and asm/code_3_2_3.o (holds just the parked sub_8007DBC, raw). Also incl. parked sub_8007DBC (NON_MATCHING C reconstruction widens this unit past its own real 0x08007F78 end, per the same "parked function" convention as sub_80073DC above) - sub_8007DBC itself stays raw, linked from asm/code_3_2_3.o
    (0x08007F78, "src/graphics/actor_part3.o", "graphics"),  # sub_8007F78/sub_8007FD8 - visibility/on-screen check and an AABB-vs-region overlap test (see sub_8006FE4 in graphics.o for sub_8007F78's sibling), non-adjacent to actor_part2.o since sub_8007DBC sits raw between them; matched. New object, inserted between asm/code_3_2_3.o (ends after sub_8007DBC's guard) and asm/code_3_2_4.o (starts at sub_80080C0, the remainder of the old code_3_2_3.o content). Also incl. parked sub_8008044 (NON_MATCHING C reconstruction widens this unit past its own real 0x080080C0 end, per the same "parked function" convention as sub_80073DC above) - sub_8008044 itself stays raw, linked from asm/code_3_2_4.o
    (0x080080C0, "src/graphics/actor_part4.o", "graphics"),  # sub_80080C0/sub_800815C - another AABB-vs-region overlap test (see actor_part3.o's sub_8007FD8) and a keyframe-record-to-tile-cache-id lookup, non-adjacent to actor_part3.o since the parked sub_8008044 sits raw between them; matched. New object, inserted between asm/code_3_2_4.o (ends after sub_8008044's guard) and asm/code_3_2_5.o (starts at sub_8008304, the remainder of the old code_3_2_4.o content). Also incl. parked sub_8008188/sub_8008200/sub_8008278 (NON_MATCHING C reconstructions widen this unit past its own real 0x08008304 end, per the same "parked function" convention as sub_80073DC above) - sub_8008188/sub_8008200/sub_8008278 themselves stay raw, linked from asm/code_3_2_5.o
    (0x08008304, "src/graphics/actor_part5.o", "graphics"),  # sub_8008304/sub_8008328/sub_800834C/sub_8008350/sub_8008364/sub_8008394/sub_80083A8 - defer to the already-matched sub_8007114/sub_8006FE4/sub_8007A84 respectively (forwarding args through unmodified), a trivial always-true stub, an animation-timer-tick plus two table-slot sub_803AD7C calls, a keyframe-record-field-pointer accessor, and a global-double-dereference accessor that ignores its own argument; non-adjacent to actor_part4.o since the parked sub_8008278 sits raw between them; matched. New object, inserted between asm/code_3_2_5.o (ends after sub_8008278's guard) and asm/code_3_2_6.o (starts at sub_8008408, the remainder of the old code_3_2_5.o content). Also incl. parked sub_80083B8 (NON_MATCHING C reconstruction widens this unit past its own real 0x08008408 end, per the same "parked function" convention as sub_80073DC above) - sub_80083B8 itself stays raw, linked from asm/code_3_2_6.o
    (0x08008408, "src/graphics/actor_part6.o", "graphics"),  # sub_8008408/sub_8008434/sub_8008480/sub_8008484/sub_80084A4/sub_80084C4/sub_8008518 - a small gUnknown_03001308 sub-object accessor (same convention as sub_8007F78/sub_8006FE4), a part-object constructor, a trivial always-true stub, a table-swap-plus-conditional-sub_8026ED0 call (same shape as sub_80073BC in graphics.o), a part re-initializer (same sub_800725C/table-swap/sub_8007AB4 shape as sub_8008434, re-using an existing part instead of allocating one), four sub_80083B8-derived keyframe-record pointer pickers (genuine native `switch`es, each non-contiguous enough on its own to force a jump table without needing scattering, except sub_80084C4 which did need its case labels scattered out of numeric order), a keyframe-record-address lookup (sub_8008604, same shape as sub_8008394), a frame-index clamp-and-store (sub_8008618, needed a one-instruction inline asm anchor for a resistant add-operand-order gap), fourteen trivial `part+0xd`/`part+0x25`/`part->flags`/`part+0x2d` flag/byte accessors (sub_8008640 through sub_80086D8), and eleven more part+0x2c/+0x28/+0x38/+0x29/+0x20 accessors and keyframe-record lookups (sub_80086E4 through sub_800876C, matched; sub_8008770 parked as NON_MATCHING - a resistant "which operand goes first" add plus a trailing byte-truncation the compiler optimizes away that the ROM still has). Also incl. parked sub_8008770 (NON_MATCHING C reconstruction widens this unit past its own real 0x0800876C end, per the same "parked function" convention as sub_80073DC above) - sub_8008770 itself stays raw, linked from asm/code_3_2_7.o. New object, inserted between asm/code_3_2_6.o (ends after sub_80083B8's guard) and asm/code_3_2_7.o (now holding just the parked sub_8008770, raw)
    (0x0800878C, "src/graphics/actor_part7.o", "graphics"),  # sub_800878C through sub_8008904 - two more sub_80083B8-derived keyframe-record byte lookups, a run of plain part+0x24/+0x28/+0x29/+0x2d/+0x30/+0x34/+0x38/+0x3c field accessors (some via the (u32<<N)>>31 logical-shift bit-getter idiom), a position-resolve-and-dispatch function (sub_8008890, forwarding to sub_8007634/sub_80073DC), and two more part-object table-swap constructors (sub_80088F0/sub_8008904, reusing sub_8008484/sub_80084A4); non-adjacent to actor_part6.o since the parked sub_8008770 sits raw between them; matched. Also incl. parked sub_800891C, sub_8008A40, and sub_8008AD8 (NON_MATCHING C reconstructions widen this unit past its own real 0x08008C80 end, per the same "parked function" convention as sub_80073DC above) - all three stay raw, linked from asm/code_3_2_8.o. sub_8008A40 resolved sub_800014C's long-standing mystery (docs/rom_map.md's "packed state round-tripping") as a plain memcpy-style wrapper around the GBA BIOS CpuSet SWI, read directly from its definition in asm/crt0.s; sub_8008AD8 is a ~150-instruction player/part collision-push-out resolver, matched down to two small structural gaps (an unavoidable extra boxH load, and a knock-on register-letter difference). New object, inserted between asm/code_3_2_7.o (now holding just the parked sub_8008770) and asm/code_3_2_8.o (now holding just the parked sub_800891C/sub_8008A40/sub_8008AD8, raw)
    (0x08008C80, "src/graphics/actor_part10.o", "graphics"),  # sub_8008C80/sub_8008CEC/sub_8008D30 - the same "extended screen box" filter shape as sub_800891C's boxB pass, a table-driven-trampoline-plus-clear teardown of both a manager's arrays, and a table-driven-trampoline dispatcher gated on a caller-supplied selector; non-adjacent to actor_part7.o since the parked sub_8008AD8 sits raw between them; matched. Also incl. parked sub_8008D80 (NON_MATCHING C reconstruction widens this unit past its own real 0x08008DC0 end, per the same "parked function" convention as sub_80073DC above) - sub_8008D80 itself stays raw, linked from the new asm/code_3_2_12.o. New object, inserted between asm/code_3_2_8.o (now holding just the parked sub_800891C/sub_8008A40/sub_8008AD8) and the new asm/code_3_2_12.o (starts at sub_8008D80's guard, holding it plus sub_8008DC0 onward, the remainder of the old code_3_2_8.o content)
    (0x08008DC0, "src/graphics/actor_part11.o", "graphics"),  # sub_8008DC0/sub_8008DEC/sub_8008E50/sub_8008E94/sub_8008EB4/sub_8008EE4 - a table+0x20/0x24-trampoline-firing loop, two array-element removal functions sharing the same CpuSet-based compaction shape (one locating the index by a linear pointer-walk search, one taking the index directly as an argument), a capacity-checked array append, a two-array teardown (mem_free via sub_8026EB4, then optionally the manager itself via sub_8026ED0), and a two-array manager initializer (mem_alloc via sub_8026EC0, zero-filling the first array); non-adjacent to actor_part10.o since the parked sub_8008D80 sits raw between them; matched. Also incl. parked sub_8008F20, sub_8009150, sub_800944C, sub_8009528, sub_80096C0, and sub_8009914 (NON_MATCHING C reconstructions widen this unit past its own real 0x080099F0 end, per the same "parked function" convention as sub_80073DC above) - all six stay raw, linked from asm/code_3_2_13.o. sub_8008F20 is a fixed-slot object-pool initializer (two big 256-word zeroed tables, likely spatial-partition/collision grids, plus a singly-linked free-list build over a node array) - every load/store confirmed, parked purely on a many-register (r3/sb/sl/r4/r8) allocation gap. sub_8009150 lazily creates a "large object" bucket-255 grid registration for an object that didn't get one at insert time - every load/store confirmed, parked purely on a loop-invariant-hoisting gap (a free-list-head address computation this compiler correctly hoists out of a 255-iteration loop, where the ROM recomputes it fresh every non-empty bucket). sub_800944C is the same \"extended screen box\" filter shape as sub_8008C80, but iterating the spatial hash grid directly and firing per-object trampolines instead of building a second array - every load/store confirmed, parked purely on a single register-reuse choice (`bucket = baseIdx + 2` computed in-place instead of into a fresh register). sub_8009528 is sub_8008A40's spatial-grid analog, dispatching hits to sub_80096C0/sub_80099F0 - semantically confirmed, parked on a stack-frame/register gap larger than the established boxH issue alone, not chased further given the size of the remaining cluster. sub_80096C0 is sub_8008AD8's twin (byte-identical collision-hit resolution logic) operating in this spatial-hash-grid cluster - hits the same boxH stack-layout gap. sub_8009914 resets a pool manager to empty (tears down every active object, then rebuilds the grid/free-list from scratch) - its tail is a byte-for-byte copy of sub_8008F20's own free-list-build loop and hits the identical many-register allocation gap. New object, inserted between asm/code_3_2_12.o (now holding just the parked sub_8008D80) and the new asm/code_3_2_13.o (starts at sub_8008F20's guard, holding it, sub_8009008 raw, sub_8009150's own guard, sub_80091D4 raw, sub_800944C's own guard, sub_8009528's own guard, sub_80096C0's own guard, sub_8009868 raw, sub_8009914's own guard, then sub_80099F0 onward, the remainder of the old code_3_2_12.o content)
    (0x08009008, None, "game_loop"),  # sub_8009008 - complex spatial-hash-grid removal logic that unlinks a node from potentially many grid buckets via a two-phase search, built on the sub_8008F20 pool-manager struct; left raw rather than guess at semantics - docs/rom_map.md "Two big unnamed systems"
    (0x080091D4, None, "game_loop"),  # sub_80091D4 - remainder of the AI/collision cluster; left raw rather than guess at semantics - docs/rom_map.md "Two big unnamed systems"
    (0x08009868, None, "game_loop"),  # sub_8009868 - remainder of the AI/collision cluster; left raw rather than guess at semantics - docs/rom_map.md "Two big unnamed systems"
    (0x080099F0, "src/graphics/actor_part12.o", "graphics"),  # sub_8009A30/sub_8009AA0/sub_8009AF0/sub_8009B3C/sub_8009B70/sub_8009B9C - active-object array remove (search-based and index-based, both via sub_8009008 + the CpuSet compaction shape), a free-list-pop-and-grid-insert primitive, a grid-insert wrapper handling the "large object, second bucket" case, an array append, and a pool-manager teardown; non-adjacent to actor_part11.o since the parked sub_8008F20 sits raw between them; matched. Also incl. parked sub_80099F0 (NON_MATCHING C reconstruction widens this unit past its own real 0x08009A30 end, per the same "parked function" convention as sub_80073DC above) - sub_80099F0 itself stays raw, linked from asm/code_3_2_13.o; it's byte-identical in shape to the already-parked sub_8008D80 (same boxH stack-layout gap). New object, inserted between asm/code_3_2_13.o (now holding sub_8008F20's guard through sub_8009914, plus sub_80099F0's own guard appended at its tail) and the new asm/code_3_2_14.o (starts at sub_8009BE0, the remainder of the old code_3_2_13.o content)
    (0x08009BE0, None, "game_loop"),  # sub_8009BE0 - a physics/collision step-probe function calling still-unexamined sub_8008278/sub_8026628 (Q8->int conversion, an up-to-4-attempt probe loop, mysterious +0x2a flag toggling on gUnknown_030012D8); left raw rather than guess at semantics - docs/rom_map.md "Two big unnamed systems"
    (0x08009CA0, "src/graphics/actor_part13.o", "graphics"),  # sub_8009CA0 - tests part for a collision-grid hit against the player, gated by flag bits and a periodic fast-path check against the gUnknown_0300082C frame counter, via sub_8007C30/sub_8007CF8 (already-matched AABB builders) and sub_800B37C; on a hit, calls sub_8009D5C. Needed a `switch` (not an if/else-if chain, which this compiler always normalizes `x>=1` down to `x>0` for) to reproduce the ROM's exact 3-way dispatch, plus several byte-destination-register pins for the flag-bit tests, and an unsigned (not signed) comparison for the frame-counter check; matched after a full-rebuild catch of a real logic bug (the first AABB's hit-test failure must fall through to the second AABB, not return early). Non-adjacent to actor_part12.o since the raw sub_8009BE0 sits between them. Also incl. parked sub_8009D5C (NON_MATCHING C reconstruction widens this unit past its own real 0x08009DF4 end) - sub_8009D5C itself stays raw, linked from the new asm/code_3_2_15.o; every branch/call is confirmed correct, parked on a single conditional-branch encoding gap (ROM's mode-3 case uses a 3-instruction cmp;beq;b where every equivalent C construct this compiler accepts collapses to a 2-instruction cmp;bne). New object, inserted between the trimmed asm/code_3_2_14.o (now holding just sub_8009BE0) and the new asm/code_3_2_15.o (holds just sub_8009D5C's guard) which precedes the existing asm/code_3_2_9.o (starts at sub_8009DF4)
    (0x08009EA8, "src/graphics/actor_part8.o", "graphics"),  # sub_8009EA8 through sub_8009FB0 - self+0x6c/+0x70 "previous position" accessors (cached by the parked sub_8009DF4), two more sub_8009ED0-family part-object table-swap constructors (0x78-byte objects, gStaticData_087E3D14), the shared sub_8009F50 field-clearer they call, and two more table+N-trampoline dispatchers (sub_8009F1C/sub_8009FB0, needing an addr-before-fn read order to avoid aliasing rec/fn onto the same pinned register); non-adjacent to code_3_2_8.o's raw content since the parked sub_8009DF4 sits raw between them (see docs/matching.md); sub_8009FD4 itself was left raw/unexamined (uncertain register roles for a 4th call argument) rather than guessed at. Also incl. parked sub_8009DF4 (NON_MATCHING C reconstruction widens this unit past its own real 0x08009EA8 end, per the same "parked function" convention as sub_80073DC above) - sub_8009DF4 itself stays raw, linked from asm/code_3_2_9.o. New object, inserted between asm/code_3_2_9.o (holds just the parked sub_8009DF4) and the new asm/code_3_2_10.o (starts at sub_8009FD4, the remainder of the old code_3_2_9.o content)
    (0x08009FD4, "src/graphics/actor_part9.o", "graphics"),  # sub_8009FD4 through sub_800A0F4 - a table+0x10/0x14-driven trampoline dispatcher forwarding its own args straight through to sub_803AD88 (the table+0x14 function-pointer read is a genuine "dead read", confirmed via the identical idiom already established for sub_8007DBC's own sub_803AD88 call), an AABB-vs-region overlap test using two already-matched AABB builders (sub_8007C30/sub_8007CF8), another table+N-trampoline dispatcher (sub_800A050), a branchless `(-x|x)>>31` nonzero-test idiom (sub_800A06C), and a long run of plain self+0x44/+0x48/+0x4c/+0x50/+0x54/+0x58/+0x5c/+0x60/+0x64/+0x68/+0x69/+0x74 field accessors and bulk setters (the velocity/accel fields sub_8009DF4 clamps, among others); non-adjacent to actor_part8.o since the parked sub_8009DF4 sits raw between them. New object, replacing the old empty asm/code_3_2_10.o (which held only sub_8009FD4, now fully matched here) - inserted directly between asm/code_3_2_9.o and the new asm/code_3_2_11.o (starts at sub_800A0FC, the remainder of the old code_3_2_10.o content)
    (0x0800A0FC, None, "game_loop"),  # sub_800A0FC through sub_800A590 (part-object update/collision dispatchers calling still-unexamined sub_800A178/sub_8009BE0) - remainder of the 94.4 KB main zone, still raw - docs/rom_map.md "Two big unnamed systems"
    (0x0800A5F4, "src/graphics/actor_part14.o", "graphics"),  # sub_800A5F4/sub_800A600/sub_800A604/sub_800A650/sub_800A664/sub_800A6A4/sub_800A6C4/sub_800A6D0/sub_800A6DC/sub_800A6E8/sub_800A6F4/sub_800A700/sub_800A70C/sub_800A718/sub_800A724/sub_800A730 - a run of small part-object constructors (gStaticData_087E3D8C-table siblings of the gStaticData_087E3D14-table sub_8009ED0/sub_8009F1C/sub_8009F50 family already matched in actor_part8.c), a field clearer/initializer, and a dozen single-bit accessor pairs (get/set/clear on part+0xc's flags byte and part+0xd's own flags byte) plus a plain s32 getter; non-adjacent to actor_part9.o since a large raw span (sub_800A0FC-sub_800A590) sits between them; matched. New object, inserted between the trimmed asm/code_3_2_11.o (now ending right before sub_800A5F4) and the new asm/code_3_2_16.o (starts at sub_800A734, the remainder of the old code_3_2_11.o content)
    (0x0800A734, None, "game_loop"),  # sub_800A734-sub_800B320 (calls still-unexamined sub_800815C, sub_800D040, sub_80109A4) - remainder of the 94.4 KB main zone, still raw - docs/rom_map.md "Two big unnamed systems"
    (0x0800B324, "src/graphics/actor_part15.o", "graphics"),  # sub_800B324-sub_800B3AC (6 fns): boolean/clear accessors on a new big unnamed object (0x108+ bytes, distinct from struct actor), a gUnknown_030012D8 AABB-vs-buf collision check (sub_800B37C, reused by many earlier-matched pool functions), and a table-swap+child-trampoline+teardown constructor helper (sub_800B3AC); matched
    (0x0800B3F0, None, "graphics"),  # sub_800B3F0 - a part-object constructor calling still-unexamined sub_80087C0/sub_80087B4/sub_800872C and sub_800A734 (itself still raw); left raw rather than guess at semantics
    (0x0800B4A4, "src/graphics/actor_part16.o", "graphics"),  # sub_800B4A4-sub_800B69C (38 fns) + sub_800B6A0/sub_800B6D0 (parked): the rest of the same big unnamed object's get/set/clear/increment accessors, an indexed 5-element s32 array getter/setter, and two mirror-flag-gated vector copies (parked - see docs/matching.md for the "unavoidable callee-saved register spill" gap); matched except the two parked functions, whose real bytes live in asm/code_3_2_18.s
    (0x0800B704, "src/graphics/actor_part17.o", "graphics"),  # sub_800B704/sub_800B734/sub_800B7B0/sub_800B838 (a table-driven trampoline pair and a fixed-point-scaled vector-copy pair, siblings of sub_800B3AC/sub_8009D5C and sub_800B6A0/sub_800B6D0 respectively), nullsub_13, sub_800B86C (guarded frame-index setter), and four more part+0xc/part+8 table-pointer accessors; matched
    (0x0800B8DC, None, "graphics"),  # sub_800B8DC onward (a 546+-line function and beyond) - not yet examined
    (0x0801E578, None, "graphics_loading"),  # LoadGraphicsPackage cluster (16.2KB); menu_ui's ~9.1KB dispatch-table functions and the trigger-effect spawner family are interleaved inside this same range, not a separate block - see docs/rom_map.md "Major correction: there is no second table"
    (0x080225A0, None, "game_loop"),  # UpdateGameFrame-MainLoop cluster (18,764B exact), confirmed same system/signature as the 0x08006C00 zone, not a separate island
    (0x08026EEC, None, "hud"),  # ~4.8 of 5.9KB is HUD stat-widgets; ~0.3KB (fx's particle/trajectory-queue pair) and ~0.8KB unlabeled remainder are interleaved inside this same span - docs/rom_map.md "fx wasn't right either"
    (0x0802866C, None, "hud"),  # InitHudIconWidgetA/B, MeasureText, UploadHudTile, InitHudTextWidget
    (0x08028BA0, None, "graphics_loading"),  # InitObjTileFreeList, LoadSpriteFrameTiles, SetupSpriteFrameOam, DecompressCategorySpriteSheet
    (0x080291A4, None, "actor"),  # SetupActorVramPool, InitActorCategory, SelectActorCategory, InitActorPart, UpdateAnimatedActorPart, ConstructAnimTableState, ConstructActorPart
    (0x0802B364, None, "actor"),  # 40.4 KB actor zone (docs/rom_map.md cites 0x0802B348, 28B before the nearest real function start - snapped forward since 0x0802B348 itself falls mid-function) - category/part/vtable system, boss-candidate + singleton object clusters
    (0x080354E0, None, "graphics_loading"),  # LoadLevelGraphics, LoadBg2Background, LoadObjSpriteTiles
    (0x08037110, None, "audio"),  # Shin'en GAX2 engine, boundary narrowed this session to end at 0x0803A944; a couple of generic 64-bit-division helpers are confirmed interleaved false positives - see docs/audio.md
    (0x0803A944, None, "system"),  # BIOS svc wrapper stubs + LZ77UnCompWrapper/RLUnCompWrapper, confirmed non-audio via matched asset_util.c callers
    (0x0803B058, "src/graphics/actor_anim.o", "graphics"),
    (0x0803B060, None, "actor"),  # GetAnimFrameData + 43 unnamed neighbors, medium confidence
    (0x0803B8B0, None, None),  # sentinel end address, not a real unit
]

# Display names for progress_categories - report_units.py-only categories
# (game_loop/actor/... don't mirror a src/ directory the way graphics/util/
# system do, so `category.capitalize()` alone would read oddly).
CATEGORY_NAMES = {
    "system": "System",
    "util": "Util",
    "graphics": "Graphics",
    "game_loop": "Game Loop",
    "actor": "Actor",
    "graphics_loading": "Graphics Loading",
    "audio": "Audio (GAX2)",
    "hud": "HUD",
    "overlay_ui": "Overlay UI",
}


def run(cmd, **kwargs):
    subprocess.run(cmd, check=True, **kwargs)


def slice_source(source, start, end):
    args = ["python3", str(ROOT / "tools" / "slice_expected.py"), str(source), f"{start:x}"]
    if end is not None:
        args.append(f"{end:x}")
    return subprocess.run(args, capture_output=True, text=True, check=True).stdout


def build_target(name, start, end):
    """Assembles the frozen-source slice(s) covering [start, end) into
    build/expected/units/<name>_target.o, applying corrections.txt."""
    out_o = BUILD_DIR / f"{name}_target.o"
    if start < CODE3_START and (end is None or end <= CODE3_START):
        text = slice_source(LEGACY, start, end)
        out_s = BUILD_DIR / f"{name}_target.s"
        out_s.write_text(text)
        run(AS + ["-o", str(out_o), str(out_s)])
    elif start >= CODE3_START:
        text = slice_source(CODE3, start, end)
        out_s = BUILD_DIR / f"{name}_target.s"
        out_s.write_text(text)
        run(AS + ["-o", str(out_o), str(out_s)])
    else:
        # Straddles both frozen sources (only irq.c does) - slice each
        # half separately, then merge.
        legacy_text = slice_source(LEGACY, start, CODE3_START)
        code3_text = slice_source(CODE3, CODE3_START, end)
        legacy_s = BUILD_DIR / f"{name}_legacy.s"
        code3_s = BUILD_DIR / f"{name}_code3.s"
        legacy_s.write_text(legacy_text)
        code3_s.write_text(code3_text)
        legacy_o = BUILD_DIR / f"{name}_legacy.o"
        code3_o = BUILD_DIR / f"{name}_code3.o"
        run(AS + ["-o", str(legacy_o), str(legacy_s)])
        run(AS + ["-o", str(code3_o), str(code3_s)])
        run(LD + ["-o", str(out_o), str(legacy_o), str(code3_o)])

    run(["python3", str(ROOT / "tools" / "patch_expected_target.py"), str(CORRECTIONS), str(out_o)])
    return out_o


def main():
    BUILD_DIR.mkdir(parents=True, exist_ok=True)
    categories = {}
    units = []

    for i in range(len(UNITS) - 1):
        start, base_rel, category = UNITS[i]
        end = UNITS[i + 1][0]
        if base_rel is None:
            name = f"raw_{start:08X}"
        else:
            name = Path(base_rel).stem

        target_o = build_target(name, start, end)
        unit = {
            "name": name,
            "target_path": str(target_o.relative_to(ROOT)),
        }
        if base_rel is not None:
            unit["base_path"] = f"build/crashbandicootxs/{base_rel}"
        if category is not None:
            unit["metadata"] = {"progress_categories": [category]}
            categories[category] = CATEGORY_NAMES.get(category, category.capitalize())
        units.append(unit)

    objdiff = {
        "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
        "build_target": False,
        "build_base": False,
        "progress_categories": [{"id": cid, "name": name} for cid, name in categories.items()],
        "units": units,
    }
    (ROOT / "objdiff.json").write_text(json.dumps(objdiff, indent=4) + "\n")
    print(f"Wrote objdiff.json with {len(units)} units, {len(categories)} categories")


if __name__ == "__main__":
    main()
