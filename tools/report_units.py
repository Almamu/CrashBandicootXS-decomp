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
    (0x080007EC, "src/graphics/intro_screen.o", "graphics"),
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
    (0x080016EC, "src/audio/music_player.o", "audio"),  # sub_80016EC (per-tick music fade-envelope update)/sub_80017BC (start-song) - first matched code in the GAX2 wrapper layer; see docs/status/audio.md and docs/audio.md
    (0x08001854, None, "audio"),  # PlaySfx (parked - real bytes in asm/code_3_1_10.s, its reconstruction lives in src/audio/sfx_ambient.c); see docs/matching.md for the prologue register-save-scheduling gap
    (0x0800190C, "src/audio/sfx_ambient.o", "audio"),  # sub_800190C/sub_80019A8/sub_80019CC/sub_80019E8 - the ambient/looping-sfx-channel tick update, stop-if-playing scan, reset, and force-expire; matched
    (0x080019F8, None, "audio"),  # sub_80019F8 (parked - real bytes in asm/code_3_1_10_2.s, its reconstruction lives in src/audio/audio_context.c); see docs/matching.md for the u8-stack-parameter-load/CSE gap
    (0x08001AB8, "src/audio/audio_context.o", "audio"),  # sub_8001AB8-sub_8001C64 (17 fns) - the rest of the AudioContext accessor/state-machine cluster (play/pause/stop, both fade-envelope arm/setter pairs, the constructor); matched
    (0x08001C80, None, "overlay_ui"),  # pause-menu/dialog dominant component resumes here (real bytes in asm/code_3_1_10_3.s) - 118 fns/19.2KB; a smaller SIO/link-cable subsystem (sub_8001F50/sub_8001DB4/...) is interleaved throughout this same span, not separable by address - folded into overlay_ui regardless, see docs/rom_map.md
    (0x08003C90, "src/graphics/settings_menu.o", "overlay_ui"),  # sub_8003C90 (settings-row centered-label draw); matched. Also incl. parked sub_8003B40/sub_8003BDC/sub_8003D3C/sub_80041BC/sub_8004914/sub_80049CC (NON_MATCHING C reconstructions widen this unit past its own real 0x08003D3C end, per the same "parked function" convention as sub_8006600 above) - those six stay raw here, wrapped `.if NON_MATCHING == 0` across asm/code_3_1_10_3.s/code_3_1_10_4.s/code_3_1_10_5.s
    (0x08003D3C, None, "overlay_ui"),  # sub_8003D3C (parked, real bytes in asm/code_3_1_10_4.s) through sub_800450C - includes sub_8003F30/sub_800450C, left fully raw (semantics not confidently understood yet - see docs/matching.md)
    (0x080047F8, "src/graphics/settings_menu2.o", "overlay_ui"),  # sub_80047F8/sub_8004860/sub_80048BC/sub_80048E0 - screen-init BG-load helper + per-row stats gatherer/aggregator; matched
    (0x08004914, None, "overlay_ui"),  # sub_8004914/sub_80049CC (parked, real bytes in asm/code_3_1_10_5.s) - NON_MATCHING C reconstructions live in src/graphics/settings_menu.c
    (0x08004A50, "src/graphics/settings_menu3.o", "overlay_ui"),  # sub_8004A50-sub_8004C7C (12 fns) - the settings-row flag test, link-cancel-flag pair, six near-identical per-item wrappers (see docs/rom_map.md), the state jump-table dispatcher, and a final list-refresh trio; matched
    (0x08004CB4, "src/graphics/settings_menu4.o", "overlay_ui"),  # sub_8004CB4/sub_8004CE8/sub_8004D20/sub_8004D4C - confirm/cancel handler, BG0HOFS/DISPCNT save-restore, and the SIO-spinner teardown/construct pair; matched
    (0x08004D74, None, "overlay_ui"),  # sub_8004D74 onward - the composite pause/options screen's own constructor and its icon-manager-heavy sub-widgets; raw/untouched (real bytes in asm/code_3_1_10_7.s)
    (0x08005A78, "src/graphics/settings_menu6.o", "overlay_ui"),  # sub_8005A78 (settings-row icon widget constructor); matched. Also incl. parked sub_8005AE8/sub_8005B80/sub_8005C58/sub_8005D44 (NON_MATCHING C reconstructions widen this unit past its own real 0x08005AE8 end, same convention as settings_menu.o above) - those four stay raw here, wrapped `.if NON_MATCHING == 0` in asm/code_3_1_10_8.s
    (0x08005E5C, None, "overlay_ui"),  # sub_8005E5C - raw/untouched (real bytes in asm/code_3_1_10_9.s)
    (0x08005EF4, "src/graphics/settings_menu7.o", "overlay_ui"),  # sub_8005EF4/sub_8005FBC (per-row percentage inc/dec pair) - entirely parked, real bytes wrapped `.if NON_MATCHING == 0` in asm/code_3_1_10_10.s
    (0x08006084, "src/graphics/settings_menu5.o", "overlay_ui"),  # sub_8006084/sub_800609C - a small counter/threshold wrap-increment/decrement pair on the settings-row sub-widget; matched
    (0x080060AC, None, "overlay_ui"),  # raw/untouched continues (real bytes in asm/code_3_1_10_11.s)
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
    (0x0801426C, "src/graphics/actor_part18.o", "graphics"),  # sub_801426C/sub_80142B0 - two entries of the 42-slot action dispatch table (gStaticData_0816BF20, docs/rom_map.md) sharing a state/flag/table-index trio (+0x27/+0x28/+0x29/+0x2f/+0x30/+0x31/+0x32) and the usual base+offset+fn-pointer trampoline pair via self+0xc/self+0x10; matched. Non-adjacent to actor_part17.o since a large raw span (sub_800B8DC onward, including the still-unexamined sub_8012FBC/sub_8013228/sub_80134B8/sub_8013994 neighbors of this same table) sits between them. Also incl. parked sub_801434C (NON_MATCHING C reconstruction widens this unit past its own real 0x0801434C end, per the same "parked function" convention as sub_80073DC/etc. elsewhere in this file) - sub_801434C itself stays raw, linked from the new asm/code_3_2_17_1434c.s. New object, inserted between asm/code_3_2_17.o (now ending right before sub_801426C) and the new asm/code_3_2_17_14674.o (starts at sub_8014674, the remainder of the old code_3_2_17.o content), with asm/code_3_2_17_1434c.o (the parked sub_801434C), src/graphics/actor_part18b.o (sub_80144E0/sub_8014524, matched, plus parked sub_80145E4 - see its own UNITS entry below), and asm/code_3_2_17_145e4.o (the parked sub_80145E4) between them in the matching build
    (0x080144E0, "src/graphics/actor_part18b.o", "graphics"),  # sub_80144E0/sub_8014524 - continuation of actor_part18.o's action-table entries, non-adjacent since the parked sub_801434C sits raw between them; matched. Also incl. parked sub_80145E4 (NON_MATCHING C reconstruction widens this unit past its own real 0x080145E4 end) - sub_80145E4 itself stays raw, linked from asm/code_3_2_17_145e4.s. New object, inserted between the new asm/code_3_2_17_1434c.o and asm/code_3_2_17_145e4.o
    (0x08014674, None, "graphics"),  # sub_8014674 onward - remainder of the still-unexamined core, including further gStaticData_0816BF20 action-table entries; not yet examined
    (0x0801E578, None, "graphics_loading"),  # LoadGraphicsPackage cluster (16.2KB); menu_ui's ~9.1KB dispatch-table functions and the trigger-effect spawner family are interleaved inside this same range, not a separate block - see docs/rom_map.md "Major correction: there is no second table"
    (0x080225A0, None, "game_loop"),  # UpdateGameFrame-MainLoop cluster (18,764B exact), confirmed same system/signature as the 0x08006C00 zone, not a separate island. UpdateGameFrame itself (0x080225A0-0x08022BF0) is a ~730-instruction jump-table state machine left raw - see docs/matching.md, GitHub issue #34
    (0x08022BF0, "src/system/game_loop.o", "game_loop"),  # sub_8022BF0/sub_8022CA0 (parked, NON_MATCHING) - real bytes live in asm/code_3_2_17_22bf0.s - see docs/matching.md
    (0x08022D50, None, "game_loop"),  # sub_8022D50 - level-start/reset routine (gUnknown_030012EC array walk, sub_803AD7C trampolines), left raw - see docs/matching.md
    (0x08022EA8, "src/system/game_loop2.o", "game_loop"),  # sub_8022EA8/sub_8022F2C (parked, NON_MATCHING, real bytes in asm/code_3_2_17_22ea8.s) plus sub_8022FEC/sub_802306C and the self+0x80/0x84/0x88/0xac/0xc0/+2-flags accessor family (matched) - see docs/matching.md
    (0x080231CC, None, "game_loop"),  # remainder of the UpdateGameFrame-MainLoop cluster, still raw
    (0x08024E68, "src/system/game_loop3.o", "game_loop"),  # GitHub issue #40: sub_8024E68/sub_8024E90/sub_8024EB4/sub_8024F04/sub_8024F0C/sub_8024F10/sub_8024F14/sub_8024F18/sub_8024F1C/sub_8024F20 (the viewport/parallax-layer accessors, matched) + sub_8024F24/sub_80250BC/sub_8025130/sub_8025228/sub_8025334 (the 16-slot terrain-tile decode/LRU cache and its consumers, parked NON_MATCHING - real bytes in asm/code_3_2_17_24f24.s) - see docs/matching/issue-40-terrain-tile-cache.md
    (0x08025444, "src/system/game_loop4.o", "game_loop"),  # GitHub issue #40: sub_8025444/nullsub_4 (matched) + sub_8025460 (parked, NON_MATCHING, real bytes in asm/code_3_2_17_25460.s) - see docs/matching/issue-40-terrain-tile-cache.md
    (0x080254C0, "src/system/game_loop5.o", "game_loop"),  # GitHub issue #40: sub_80254C0/sub_80254F8/sub_8025554/sub_8025588/sub_80255A8/sub_80255C4 (matched) - see docs/matching/issue-40-terrain-tile-cache.md
    (0x080255D4, None, "game_loop"),  # sub_80255D4 - DMA-writes to OBJ palette RAM and a BG window register, then walks a small count-prefixed gUnknown_030012E4 list (docs/rom_map.md: "a per-frame visible-object/window list processor") - left raw, GitHub issue #40
    (0x08026EEC, "src/system/main_loop.o", "system"),  # MainLoop/sub_8026F38 - the game's top-level per-frame loop (sets up the central state object, runs UpdateGameFrame forever) and a two-level per-widget-mode table lookup; matched. GitHub issue #45
    (0x08026F54, None, "hud"),  # sub_8026F54/sub_8027018 - a fixed 3-entry particle/effect queue's consumer/producer pair, left raw - see docs/rom_map.md's "fx" investigation (GitHub issue #45)
    (0x08027088, "src/graphics/hud_icon_slot.o", "graphics"),  # sub_8027088-sub_8027120 - the same fx-queue's reset/constructor pair, a HUD digit-slot draw helper (sub_80270E0), and two struct-actor-table-swap slot constructors (sub_802710C UNUSED, sub_8027120 matched); matched. Non-adjacent to hud_counter.o since sub_8027138-sub_802763C sit raw between them. GitHub issue #45
    (0x08027138, None, "hud"),  # sub_8027138-sub_802763C - the 34-slot icon-array setup cluster (sub_8027138/sub_802732C) and the HUD stat-widget dispatcher family (sub_80274EC/sub_802757C/sub_802763C), left raw - see docs/rom_map.md's "hud" investigation (GitHub issue #45)
    (0x08027838, "src/graphics/hud_counter.o", "graphics"),  # sub_8027838 - cached two-digit HUD counter update
    (0x08027940, None, "hud"),  # sub_8027940/sub_8027D5C/sub_8027E88 - more of the HUD stat-widget dispatcher family, left raw (GitHub issue #45)
    (0x08028400, "src/graphics/hud_blink.o", "graphics"),  # sub_8028400-sub_8028520 - a 3-slot icon-blink animation timer: a per-frame tick, three per-slot trigger functions, and the generic single-slot advance helper they share; matched. GitHub issue #45
    (0x08028568, None, "hud"),  # remainder of the HUD stat-widget region, left raw
    (0x0802866C, None, "hud"),  # InitHudIconWidgetA/B, MeasureText, UploadHudTile, InitHudTextWidget
    (0x08028BA0, None, "graphics_loading"),  # InitObjTileFreeList, LoadSpriteFrameTiles, SetupSpriteFrameOam, DecompressCategorySpriteSheet
    (0x080291A4, None, "actor"),  # SetupActorVramPool, InitActorCategory, SelectActorCategory, InitActorPart, UpdateAnimatedActorPart, ConstructAnimTableState, ConstructActorPart
    (0x0802B364, None, "actor"),  # 40.4 KB actor zone (docs/rom_map.md cites 0x0802B348, 28B before the nearest real function start - snapped forward since 0x0802B348 itself falls mid-function) - category/part/vtable system, boss-candidate + singleton object clusters
    (0x0802BED8, "src/graphics/actor_part19.o", "actor"),  # sub_802BED8-sub_802C19C (issue #52): the same player/action-object action-table family as actor_part17.o/actor_part18.o (state/table-index/anim-frame fields, a self+0x50 trampoline record, and the self+0x48/0x4c circular actor list sub_802C19C unlinks from); matched
    (0x0802C208, "src/graphics/actor_part19e.o", "actor"),  # sub_802C208 (parked, NON_MATCHING) - a gStaticData_0817A6B8 stride-8 trampoline-record dispatcher; real bytes live in asm/code_3_2_20_28568_c208.s - see docs/matching.md
    (0x0802C264, "src/graphics/actor_part19f.o", "actor"),  # sub_802C264/sub_802C270 - a constant getter and a velocity/anim-frame-threshold updater for the same self object; matched
    (0x0802C2FC, "src/graphics/actor_part19b.o", "actor"),  # sub_802C2FC (parked, NON_MATCHING) - OAM setup for one sprite frame; real bytes live in asm/code_3_2_20_28568_c2fc.s - see docs/matching.md
    (0x0802C394, "src/graphics/actor_part19c.o", "actor"),  # sub_802C394 - same teardown/unlink shape as sub_802C19C, iteration-count variant; matched
    (0x0802C3E8, "src/graphics/actor_part19c2.o", "actor"),  # sub_802C3E8 (parked, NON_MATCHING) - a homing/seek-toward-point spawn-effect constructor; real bytes live in asm/code_3_2_20_28568_c3e8.s - see docs/matching.md
    (0x0802C464, "src/graphics/actor_part19g.o", "actor"),  # sub_802C464-sub_802C6C0 - proximity/overlap-dispatch and "used"-state transition family sharing the tail sub_802C4C8; matched
    (0x0802C7A8, None, "actor"),  # sub_802C7A8 - a circular-list AABB-overlap scan (self+0x4c walk, type==4 filter, sub_800014C-based translate+compare) - left raw, stack-buffer layout not pinned down with enough confidence for a byte-exact attempt this pass
    (0x0802C904, "src/graphics/actor_part19d.o", "actor"),  # sub_802C904 - same proximity/overlap "used"-state transition shape as actor_part19g.o's family; matched
    (0x0802C99C, None, "actor"),  # remainder of the 40.4 KB actor zone, still raw
    (0x080354E0, None, "graphics_loading"),  # LoadLevelGraphics, LoadBg2Background, LoadObjSpriteTiles
    (0x08037110, "src/audio/counter_selector.o", "audio"),  # sub_8037110/nullsub_7/sub_8037154/sub_803716C/sub_80371B4/sub_8037224 - a small on-screen 0-5 "counter" widget (increments/decrements with input, confirms/cancels with PlaySfx), reads like game/HUD-side code using PlaySfx rather than GAX2 internals; sub_803716C is UNUSED (no caller found); matched
    (0x080372BC, None, "audio"),  # sub_80372BC/sub_8037388 - fully understood (icon-manager draw loop / tile-cache init for the widget above) but hits the same gcc-2.9 many-register-allocation difficulty already documented for sub_8006600 (src/graphics/oam_count.c); left raw rather than force a low-confidence register pin
    (0x080374D0, "src/audio/counter_selector_setup.o", "audio"),  # sub_80374D0/sub_8037534/sub_8037548/sub_8037578/sub_80375A0/sub_80375EC/sub_8037620 - the widget's graphics/BG setup, draw-flush, and init/teardown pair; matched
    (0x08037648, None, "audio"),  # sub_8037648/sub_8037A7C/sub_8037E54/sub_8037ECC/sub_8037F3C - confirmed/likely generic 64-bit software division/multiply helpers (see docs/audio.md's sub_8037648/sub_8037A7C entries) interleaved in the GAX2 range; left raw, not chased further this pass
    (0x08037FA0, "src/audio/song_slot_lookup.o", "audio"),  # sub_8037FA0 - a 12-entry threshold-table lookup; table contents not understood; matched
    (0x08037FC0, None, "audio"),  # sub_8037FC0 - a large, genuinely hard-to-follow GAX2 mixer/timing computation over gStaticData_085A6150 and several SoundHandler-shaped structures; left raw, not attempted this pass
    (0x080381FC, "src/audio/sound_object_init.o", "audio"),  # sub_80381FC - a SoundHandler/channel-object-shaped constructor (zero-fill plus a few sentinel fields); matched
    (0x08038240, None, "audio"),  # sub_8038240/sub_80384DC - core GAX2 mixer-state wiring (gUnknown_03001630) and a hardware sound-register reset with an inlined timing-loop compiler quirk already flagged in the raw asm; left raw, not attempted this pass
    (0x08038538, None, "audio"),  # sub_8038538 (play-start/init entry point, docs/audio.md), sub_8038A1C, sub_8038B68 - genuine GAX2 mixer-state internals hitting the same many-register (r8/sb/sl) gcc-2.9 allocation difficulty documented for sub_8006600/sub_80372BC; left raw, issue #67
    (0x08038C28, "src/audio/gax_dma_control.o", "audio"),  # sub_8038C28/sub_8038C50 - Direct Sound A output stop/start pair (SOUNDCNT_H bits 8/9, FIFO_A flush); matched, issue #67
    (0x08038C88, None, "audio"),  # sub_8038C88/sub_8038DC0/sub_8038E74 - more GAX2 mixer-tick/voice-stealing internals (sub_8038E74 is the voice-stealing allocator, docs/audio.md); left raw, issue #67
    (0x08038F94, "src/audio/gax_note_param.o", "audio"),  # sub_8038F94 - conditional per-voice note-period update; matched, issue #67
    (0x08038FD0, None, "audio"),  # sub_8038FD0/sub_8039064/sub_80390F8/sub_8039198/sub_80391E8/sub_8039214 - per-channel mute/volume-set family (hit the same many-register loop-allocation difficulty as sub_8038538 above) plus a hardware-register NOP-delay quirk and a text/console state machine; left raw, issue #67
    (0x080392C4, "src/audio/gax_swi.o", "audio"),  # sub_80392C4 - HuffUnComp (SWI 0x13) wrapper with hand-written-looking r0/r1 preservation; matched (NAKED asm transcription), issue #67
    (0x080392E0, None, "audio"),  # sub_80392E0 - fatal-error display (renders a message via sub_8039214, then an infinite loop) - left raw, issue #67
    (0x080393D0, "src/audio/gax_sound_handler_info.o", "audio"),  # sub_80393D0/sub_80393FC/sub_803941C/nullsub_39 - the GAX2_SoundHandler "Info" type's init_fn/unknown_fn (docs/audio.md's per-type function-pointer table); matched, issue #67
    (0x0803943C, None, "audio"),  # sub_803943C (Info type's play_fn)/sub_8039518 (Channel type's init_fn) - left raw, issue #67
    (0x080395A0, "src/audio/gax_sound_handler_channel.o", "audio"),  # nullsub_40 - the GAX2_SoundHandler "Channel" type's unknown_fn; matched, issue #67
    (0x080395A4, None, "audio"),  # sub_80395A4 (Channel type's play_fn) onward - left raw, issue #67
    (0x0803A944, "src/system/timer_util.o", "system"),  # BIOS svc wrapper stubs (sub_803A944-sub_803A95C/sub_0803A960) + LZ77UnCompWrapper/RLUnCompWrapper (confirmed non-audio via matched asset_util.c callers), sub_803A968 (picks a 12-byte EepromConfig table by chip-size code - see the struct's header comment) and sub_803A9D0 (claims a hardware timer, hands back an IRQ-handler-stub address); matched, issue #69. Also incl. parked sub_803AA08/sub_803AA90/sub_803AAD4 (NON_MATCHING C reconstructions widen this unit past its own real 0x0803AA08 end, per the same "parked function" convention as sub_80073DC above) - all three stay raw, linked from asm/code_3_2_20e_aa08.s; every field/register access is confirmed against the ROM, parked purely on register-allocation/loop-shape gaps (see docs/matching.md)
    (0x0803AB54, None, "system"),  # sub_803AB54/sub_803AC04/sub_803ACE0/sub_803AD38 - a DMA3 bit-serial EEPROM read/write/retry cluster (working theory, not confirmed enough to commit even a parked reconstruction); left raw - see asm/code_3_2_20e_ab54.s's header comment and docs/matching.md, issue #69
    (0x0803AD78, "src/system/reg_trampolines.o", "system"),  # sub_803AD78-sub_803AD94 (bx-r0..sp trampolines, called with the target function pointer already sitting in that register) and nullsub_43 (bonus, just past issue #69's listed range); matched
    (0x0803ADB4, None, "util"),  # sub_803ADB4 - signed integer division (docs/rom_map.md); NON_MATCHING C reconstruction in src/util/math_div_util.c matches every instruction except the prologue/epilogue (agbcc doesn't shrink-wrap the divide-by-zero fast path) - parked, raw bytes stay in asm/code_3_2_20e_3adb4.s, issue #70
    (0x0803AE48, "src/util/math_div_util.o", "util"),  # nullsub_8 - shared divide-by-zero handler for sub_803ADB4/sub_803AE4C/sub_803AF1C; matched, issue #70
    (0x0803AE4C, None, "util"),  # sub_803AE4C (signed modulo)/sub_803AF1C (unsigned modulo) - same shift-and-subtract shape as sub_803ADB4, NON_MATCHING C reconstructions in src/util/math_div_util.c; parked (same prologue/epilogue gap, plus a `ror`-codegen gap), raw bytes stay in asm/code_3_2_20e_3ae4c.s, issue #70
    (0x0803AFDC, "src/graphics/actor_aabb_setup.o", "graphics"),  # sub_803AFDC/sub_803AFE4 (shared AABB set-size/set-position primitives), sub_803AFEC (raw-offset getter), sub_803AFF0/sub_803B024 (per-type descriptor table constructors, gStaticData_087E3BEC family); matched, issue #70
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
