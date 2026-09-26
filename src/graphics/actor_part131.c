#include "core.h"
#include "gba/io_reg.h"
#include "icon_manager.h"
#include "vram_pool.h"
#include "audio.h"

/* GitHub issue #64 (0x08034AA4-0x080354E0, 13 functions). Continues
 * straight on from issue #63's fade-overlay cluster (actor_part87.c/
 * actor_part88.c/actor_part89.c) - the first five functions here
 * (sub_8034AA4/sub_8034C40/sub_8034C5C/sub_8034C84/sub_8034CB0) are more
 * methods on that same `struct fade_overlay` "self" object, then the
 * chunk moves on to an unrelated "between-level map/progress screen"
 * object driven directly from `game_loop` (see docs/rom_map.md's
 * "sub_8034CB0 turns out to be a separate screen trigger"/"A fourth
 * thing in this file" sections) - see
 * docs/matching/issue-64-0x08034aa4-actor.md for the full write-up. */

/* Same `struct fade_overlay` as actor_part87.c/88.c/89.c, redeclared
 * locally per this project's minimal-local-type convention. This
 * chunk's functions pin down real meanings for two fields actor_part87.c
 * left vague: `unused_1c` is a per-item blink/flash toggle counter
 * (kept the same field name there since that file never touches it),
 * and `flag_20` - guessed there as "which of two alternating cue sfx
 * last fired" - turns out, in this sibling function set, to hold the
 * Yes/No dialog's currently-selected option index (0/1; any other value
 * means neither option is highlighted) instead. Same field, a related
 * but distinct use by this file's functions. */
struct fade_overlay {
    u8 *bg1Buf;   /* 0x00 */
    u8 *bg0Buf;   /* 0x04 */
    u8 *bg2Buf;   /* 0x08 */
    u16 dispcnt;  /* 0x0c */
    u8 unused_0e[0xa];
    struct icon_manager *icons; /* 0x18 */
    s32 blinkCounter; /* 0x1c */
    s32 selection;     /* 0x20 */
};

/* The "between-level map/progress screen" object (docs/rom_map.md's "A
 * fourth thing in this file" section) - a combined minimap-reveal +
 * floating-text-popup screen driven from `UpdateGameFrame`'s level-load
 * state machine, allocated `sub_8026EDC(0x98)` by `sub_80354BC`. Only
 * the fields this file's real-C functions actually touch are named;
 * `asset0`-`asset4` are five 0x18-apart pointer fields (each the head of
 * a separately heap-allocated buffer `sub_80352AC` DMA-fills and
 * `sub_803547C` frees) - the 0x14 bytes between each pair belong to
 * this same struct too, just not touched by any function in this file. */
struct map_screen {
    void *popupListHead; /* 0x00 - timed text-popup node list, see sub_80350A4 */
    const void *streamBase;   /* 0x04 - popup byte-opcode stream base */
    const void *streamCursor; /* 0x08 - popup byte-opcode stream cursor */
    void *mapObj;             /* 0x0c - the minimap object, sub_8034374 */
    s32 drawMode;              /* 0x10 */
    s32 suppressCounter;        /* 0x14 */
    u8 unused_18[0x18];
    void *asset0; /* 0x30 */
    u8 unused_34[0x14];
    void *asset1; /* 0x48 */
    u8 unused_4c[0x14];
    void *asset2; /* 0x60 */
    u8 unused_64[0x14];
    void *asset3; /* 0x78 */
    u8 unused_7c[0x14];
    void *asset4; /* 0x90 */
    u32 frameParity; /* 0x94 */
};


COMPILE_TIME_ASSERT(sizeof(struct map_screen) == 0x98);

extern struct oam_shadow_buffer *gUnknown_03001300;
extern void sub_8006AAC(struct oam_shadow_buffer *arg0);
extern void sub_80006A8(void);
extern void FlushVramDmaQueue(void);

/* Another instance of the by-now-familiar "refresh OAM + center text"
 * pattern (docs/rom_map.md's "sub_8034AA4 is just another instance of
 * ..." note): syncs the OAM shadow buffer and VRAM upload cursor, then
 * draws the Yes/No dialog's three labels (icon-manager mode ids
 * 0x28/0x29/0x2a) via `self->icons->record->slots[6]`'s position
 * (mode 0x28 twice - once for the plain label, once conditionally for
 * a highlighted "cursor" redraw keyed on `self->selection`), applying
 * `sub_8034C40`'s blink mask to each option's own OAM-hide byte via
 * `sub_8028A30` in between, and finally re-commits the OAM shadow
 * buffer.
 *
 * Written as NAKED asm, not plain C: `self` (r6), the OAM-shadow-buffer
 * address (sl), the constant 0x87 (r7), and the two 0x130/0x98<<1 index
 * constants (r8/sb) all stay resident in high registers across many
 * `bl` sites with no register left over - the same "many live values
 * across calls, no spare register" shape already NAKED throughout this
 * codebase (e.g. `sub_8034994`, actor_part89.c) - and this function was
 * flagged as exactly this class of difficulty in
 * docs/matching/issue-63-0x08033ef4-actor.md before this pass even
 * started. Every instruction below is transcribed directly from and
 * checked against the ROM's own disassembly. */
NAKED void sub_8034AA4(struct fade_overlay *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "add r6, r0, #0\n\t"
        "ldr r0, _08034C34\n\t"
        "mov sl, r0\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8006A90\n\t"
        "ldr r0, _08034C38\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8006C28\n\t"
        "ldr r4, [r6, #0x18]\n\t"
        "mov r1, #0x98\n\t"
        "lsl r1, r1, #1\n\t"
        "mov r8, r1\n\t"
        "add r0, r4, r1\n\t"
        "ldr r0, [r0]\n\t"
        "add r5, r0, #0\n\t"
        "add r5, #0x10\n\t"
        "mov r2, #0x10\n\t"
        "ldrsh r0, [r0, r2]\n\t"
        "add r4, r4, r0\n\t"
        "mov r0, #0x28\n\t"
        "bl sub_8026F38\n\t"
        "add r1, r0, #0\n\t"
        "ldr r2, [r5, #4]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_803AD80\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, [r6, #0x18]\n\t"
        "mov r1, #0\n\t"
        "bl sub_8028A30\n\t"
        "mov r1, #0x88\n\t"
        "sub r1, r1, r4\n\t"
        "ldr r4, [r6, #0x18]\n\t"
        "mov r7, #0x87\n\t"
        "mov r3, #0x88\n\t"
        "lsl r3, r3, #1\n\t"
        "add r0, r4, r3\n\t"
        "str r1, [r0]\n\t"
        "mov r1, #0x8a\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r4, r1\n\t"
        "str r7, [r0]\n\t"
        "mov r2, r8\n\t"
        "add r0, r4, r2\n\t"
        "ldr r0, [r0]\n\t"
        "add r5, r0, #0\n\t"
        "add r5, #0x20\n\t"
        "mov r3, #0x20\n\t"
        "ldrsh r0, [r0, r3]\n\t"
        "add r4, r4, r0\n\t"
        "mov r0, #0x28\n\t"
        "bl sub_8026F38\n\t"
        "add r1, r0, #0\n\t"
        "ldr r2, [r5, #4]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_803AD80\n\t"
        "ldr r4, [r6, #0x18]\n\t"
        "add r0, r6, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_8034C40\n\t"
        "add r1, r0, #0\n\t"
        "lsl r1, r1, #0x18\n\t"
        "lsr r1, r1, #0x18\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8028A30\n\t"
        "ldr r0, [r6, #0x20]\n\t"
        "cmp r0, #0\n\t"
        "bne _08034B6E\n\t"
        "ldr r3, [r6, #0x18]\n\t"
        "mov r1, #0x90\n\t"
        "mov r4, #0x88\n\t"
        "lsl r4, r4, #1\n\t"
        "add r0, r3, r4\n\t"
        "str r1, [r0]\n\t"
        "add r1, #0x84\n\t"
        "add r0, r3, r1\n\t"
        "str r7, [r0]\n\t"
        "mov r2, #0x98\n\t"
        "lsl r2, r2, #1\n\t"
        "add r0, r3, r2\n\t"
        "ldr r2, [r0]\n\t"
        "mov r4, #0x20\n\t"
        "ldrsh r0, [r2, r4]\n\t"
        "add r0, r3, r0\n\t"
        "ldr r1, _08034C3C\n\t"
        "ldr r2, [r2, #0x24]\n\t"
        "bl sub_803AD80\n"
    "_08034B6E:\n\t"
        "ldr r4, [r6, #0x18]\n\t"
        "mov r0, #0x98\n\t"
        "mov sb, r0\n\t"
        "mov r1, #0x88\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r4, r1\n\t"
        "mov r2, sb\n\t"
        "str r2, [r0]\n\t"
        "mov r3, #0x8a\n\t"
        "lsl r3, r3, #1\n\t"
        "add r0, r4, r3\n\t"
        "str r7, [r0]\n\t"
        "mov r1, r8\n\t"
        "add r0, r4, r1\n\t"
        "ldr r0, [r0]\n\t"
        "add r5, r0, #0\n\t"
        "add r5, #0x20\n\t"
        "mov r2, #0x20\n\t"
        "ldrsh r0, [r0, r2]\n\t"
        "add r4, r4, r0\n\t"
        "mov r0, #0x29\n\t"
        "bl sub_8026F38\n\t"
        "add r1, r0, #0\n\t"
        "ldr r2, [r5, #4]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_803AD80\n\t"
        "ldr r4, [r6, #0x18]\n\t"
        "add r0, r6, #0\n\t"
        "mov r1, #1\n\t"
        "bl sub_8034C40\n\t"
        "add r1, r0, #0\n\t"
        "lsl r1, r1, #0x18\n\t"
        "lsr r1, r1, #0x18\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8028A30\n\t"
        "ldr r0, [r6, #0x20]\n\t"
        "cmp r0, #1\n\t"
        "bne _08034BEA\n\t"
        "ldr r0, [r6, #0x18]\n\t"
        "mov r2, #0x90\n\t"
        "mov r3, #0x91\n\t"
        "mov r4, #0x88\n\t"
        "lsl r4, r4, #1\n\t"
        "add r1, r0, r4\n\t"
        "str r2, [r1]\n\t"
        "add r2, #0x84\n\t"
        "add r1, r0, r2\n\t"
        "str r3, [r1]\n\t"
        "mov r3, r8\n\t"
        "add r1, r0, r3\n\t"
        "ldr r2, [r1]\n\t"
        "mov r4, #0x20\n\t"
        "ldrsh r1, [r2, r4]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, _08034C3C\n\t"
        "ldr r2, [r2, #0x24]\n\t"
        "bl sub_803AD80\n"
    "_08034BEA:\n\t"
        "ldr r4, [r6, #0x18]\n\t"
        "mov r1, #0x91\n\t"
        "mov r2, #0x88\n\t"
        "lsl r2, r2, #1\n\t"
        "add r0, r4, r2\n\t"
        "mov r3, sb\n\t"
        "str r3, [r0]\n\t"
        "add r2, #4\n\t"
        "add r0, r4, r2\n\t"
        "str r1, [r0]\n\t"
        "mov r3, r8\n\t"
        "add r0, r4, r3\n\t"
        "ldr r0, [r0]\n\t"
        "add r5, r0, #0\n\t"
        "add r5, #0x20\n\t"
        "mov r1, #0x20\n\t"
        "ldrsh r0, [r0, r1]\n\t"
        "add r4, r4, r0\n\t"
        "mov r0, #0x2a\n\t"
        "bl sub_8026F38\n\t"
        "add r1, r0, #0\n\t"
        "ldr r2, [r5, #4]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_803AD80\n\t"
        "mov r2, sl\n\t"
        "ldr r0, [r2]\n\t"
        "bl sub_8006A48\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "_08034C34: .4byte gUnknown_03001300\n"
    "_08034C38: .4byte gUnknown_030012FC\n"
    "_08034C3C: .4byte gStaticData_0817C510\n"
    );
}

asm(".align 2, 0");

/* --------------------------------------------------------------------
 * sub_8034C40 - blink/toggle helper: returns 1 immediately if `mode`
 * isn't the dialog's current selection; otherwise advances the
 * selected item's blink counter and returns bit 1 of its pre-advance
 * value (a 0/2 flicker mask consumed by sub_8034AA4 to hide the label
 * every other frame-pair).
 * ------------------------------------------------------------------ */
s32 sub_8034C40(struct fade_overlay *self, s32 mode)
{
    register s32 result asm("r0");
    s32 counter;

    if (mode != self->selection) {
        result = 1;
    } else {
        counter = self->blinkCounter;
        result = (counter >> 1) & 2;
        self->blinkCounter = counter + 1;
    }
    return result;
}

/* --------------------------------------------------------------------
 * sub_8034C5C - one frame's "yield" helper for the fade overlay: syncs
 * the shared OAM shadow buffer, flushes the VRAM upload queue, then
 * re-applies the overlay's own DISPCNT mirror.
 * ------------------------------------------------------------------ */
void sub_8034C5C(struct fade_overlay *self)
{
    sub_80006A8();
    sub_8006AAC(gUnknown_03001300);
    FlushVramDmaQueue();
    REG_DISPCNT = self->dispcnt;
}

extern void sub_8026ED0(void *self);

/* --------------------------------------------------------------------
 * sub_8034C84 - the fade overlay's teardown: frees its three BG scratch
 * buffers, then frees `self` too when `mode` bit 0 is set.
 * ------------------------------------------------------------------ */
void sub_8034C84(struct fade_overlay *self, s32 mode)
{
    sub_8026ED0(self->bg0Buf);
    sub_8026ED0(self->bg1Buf);
    sub_8026ED0(self->bg2Buf);
    if (mode & 1)
        sub_8026ED0(self);
}

extern void *sub_8026EDC(s32 size);
extern s32 mem_free_bytes(s32 flags);
extern void *sub_803472C(void *selfArg);
extern s32 sub_8034994(void *selfArg);

/* --------------------------------------------------------------------
 * sub_8034CB0 - the "Are you sure?" confirmation-dialog trigger
 * (docs/rom_map.md's "sub_8034CB0 drives a Yes/No confirmation prompt"
 * section): allocates and builds the fade overlay (sub_803472C), runs
 * the Yes/No dialog to completion (sub_8034994), tears the overlay down
 * (sub_8034C84) if it was actually built, and returns which option was
 * selected.
 * ------------------------------------------------------------------ */
s32 sub_8034CB0(void)
{
    struct fade_overlay *self;
    u8 result;

    mem_free_bytes(0xc0000000);
    self = sub_803472C(sub_8026EDC(0x24));
    result = sub_8034994(self);
    if (self != NULL)
        sub_8034C84(self, 3);
    mem_free_bytes(0xc0000000);
    return result;
}

asm(".align 2, 0");

/* The map screen's constructor (docs/rom_map.md's "sub_8034CEC is a
 * combined constructor" note): builds the minimap sub-object
 * (`sub_8034374(sub_8026EDC(0x14))` -> `self->mapObj`), syncs the OAM
 * shadow buffer, hooks both text-icon managers
 * (`gUnknown_030012DC`/`gUnknown_030012E0`) up for this screen (firing
 * each one's slot-6 OAM trampoline via `sub_803AD7C`, and copying
 * `gUnknown_030012DC->field_12c` into `gUnknown_030012E0->field_108` -
 * a new, previously-unexplained cross-wiring between the two icon
 * managers), loads the popup-text glyph assets (`sub_80352AC`), resets
 * the shared tile cache and VRAM upload cursor, initializes the
 * popup-text opcode-stream fields to `gStaticData_0817C5D0`, sets the
 * DISPCNT "OBJ enable"-adjacent bit in `gUnknown_03001288`, and ducks
 * the audio context (`sub_8001B54(gUnknown_030012BC, 0x11)`). Returns
 * `self`.
 *
 * Written as NAKED asm, not plain C: `self` (r5), the zero constant
 * (r8), and `&gUnknown_030012E0` (sb) all stay resident across a long
 * run of `bl` sites, while r4/r6 each get rebound to a *different*
 * global's address multiple times over that same span (first
 * `&gUnknown_03001300`, then `&gUnknown_030012B8`, then
 * `&gUnknown_030012FC` for r4; `&gUnknown_030012DC` for r6) with several
 * unrelated calls in between each rebinding - the same "many high
 * registers held live across calls, with mid-function register
 * rebinding" shape already NAKED throughout this codebase for this
 * exact reason (compare `sub_803487C`, actor_part88.c, this cluster's
 * own sibling constructor, parked `NON_MATCHING` over a related
 * register-lifetime gap). Every instruction below is transcribed
 * directly from and checked against the ROM's own disassembly. */
NAKED struct map_screen *sub_8034CEC(struct map_screen *selfArg)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6}\n\t"
        "add r5, r0, #0\n\t"
        "mov r0, #0x14\n\t"
        "bl sub_8026EDC\n\t"
        "bl sub_8034374\n\t"
        "str r0, [r5, #0xc]\n\t"
        "ldr r4, _08034E0C\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8006A90\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8006A48\n\t"
        "bl sub_80006A8\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8006AAC\n\t"
        "ldr r4, _08034E10\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8006EA8\n\t"
        "ldr r6, _08034E14\n\t"
        "ldr r0, [r6]\n\t"
        "bl sub_8028A40\n\t"
        "ldr r0, _08034E18\n\t"
        "mov sb, r0\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #0\n\t"
        "bl sub_8028A30\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_80352AC\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8006DC8\n\t"
        "ldr r4, _08034E1C\n\t"
        "ldr r0, [r4]\n\t"
        "mov r1, #0\n\t"
        "mov r8, r1\n\t"
        "str r1, [r0, #8]\n\t"
        "bl sub_8006C4C\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8006C4C\n\t"
        "ldr r0, [r6]\n\t"
        "mov r2, #0x84\n\t"
        "lsl r2, r2, #1\n\t"
        "add r1, r0, r2\n\t"
        "mov r3, r8\n\t"
        "str r3, [r1]\n\t"
        "add r2, #0x28\n\t"
        "add r1, r0, r2\n\t"
        "ldr r1, [r1]\n\t"
        "add r1, #0x40\n\t"
        "mov r3, #0\n\t"
        "ldrsh r2, [r1, r3]\n\t"
        "add r0, r0, r2\n\t"
        "ldr r1, [r1, #4]\n\t"
        "bl sub_803AD7C\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r1, [r6]\n\t"
        "mov r2, #0x96\n\t"
        "lsl r2, r2, #1\n\t"
        "add r1, r1, r2\n\t"
        "ldr r1, [r1]\n\t"
        "lsl r1, r1, #5\n\t"
        "bl sub_8006C58\n\t"
        "ldr r0, [r6]\n\t"
        "mov r3, #0x96\n\t"
        "lsl r3, r3, #1\n\t"
        "add r0, r0, r3\n\t"
        "ldr r2, [r0]\n\t"
        "mov r1, sb\n\t"
        "ldr r0, [r1]\n\t"
        "sub r3, #0x24\n\t"
        "add r1, r0, r3\n\t"
        "str r2, [r1]\n\t"
        "mov r2, #0x98\n\t"
        "lsl r2, r2, #1\n\t"
        "add r1, r0, r2\n\t"
        "ldr r1, [r1]\n\t"
        "add r1, #0x40\n\t"
        "mov r3, #0\n\t"
        "ldrsh r2, [r1, r3]\n\t"
        "add r0, r0, r2\n\t"
        "ldr r1, [r1, #4]\n\t"
        "bl sub_803AD7C\n\t"
        "ldr r0, [r4]\n\t"
        "mov r2, sb\n\t"
        "ldr r1, [r2]\n\t"
        "mov r3, #0x96\n\t"
        "lsl r3, r3, #1\n\t"
        "add r1, r1, r3\n\t"
        "ldr r1, [r1]\n\t"
        "lsl r1, r1, #5\n\t"
        "bl sub_8006C58\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8006C30\n\t"
        "mov r0, r8\n\t"
        "str r0, [r5]\n\t"
        "str r0, [r5, #0x10]\n\t"
        "ldr r0, _08034E20\n\t"
        "str r0, [r5, #4]\n\t"
        "str r0, [r5, #8]\n\t"
        "mov r1, r8\n\t"
        "str r1, [r5, #0x14]\n\t"
        "ldr r1, _08034E24\n\t"
        "mov r0, #0x10\n\t"
        "ldrb r2, [r1, #1]\n\t"
        "orr r0, r2\n\t"
        "strb r0, [r1, #1]\n\t"
        "bl sub_8001614\n\t"
        "add r0, r5, #0\n\t"
        "add r0, #0x94\n\t"
        "mov r3, r8\n\t"
        "str r3, [r0]\n\t"
        "ldr r0, _08034E28\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #0x11\n\t"
        "bl sub_8001B54\n\t"
        "add r0, r5, #0\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "_08034E0C: .4byte gUnknown_03001300\n"
    "_08034E10: .4byte gUnknown_030012B8\n"
    "_08034E14: .4byte gUnknown_030012DC\n"
    "_08034E18: .4byte gUnknown_030012E0\n"
    "_08034E1C: .4byte gUnknown_030012FC\n"
    "_08034E20: .4byte gStaticData_0817C5D0\n"
    "_08034E24: .4byte gUnknown_03001288\n"
    "_08034E28: .4byte gUnknown_030012BC\n"
    );
}

asm(".align 2, 0");

extern void sub_80007AC(void *arg0);
extern void sub_80350A4(struct map_screen *self);
extern void sub_8034EF0(struct map_screen *self);
extern void sub_803544C(void *unused);
extern void sub_8034688(void *mapObj);
extern void sub_8001AC4(struct AudioContext *self, u32 value);
extern struct AudioContext *gUnknown_030012BC;
extern void *gUnknown_03001304;

struct held_pressed_pair {
    u16 held;
    u16 pressed;
};
extern struct held_pressed_pair gUnknown_030007E0;

/* --------------------------------------------------------------------
 * sub_8034E2C - the map screen's per-frame driver: an input-gated busy
 * loop toggling `frameParity` every iteration (driving the popup-text
 * system, sub_80350A4, every other frame) alongside the minimap reveal
 * (sub_8034688) every frame, until confirm or D-pad-down+L is pressed;
 * then a fixed 17-frame wipe/transition effect poking the window-blend
 * hardware registers directly; then frees every remaining popup-text
 * list node.
 * ------------------------------------------------------------------ */
void sub_8034E2C(struct map_screen *self)
{
    s32 i;

    while (1) {
        sub_80007AC(gUnknown_03001304);
        {
            register struct held_pressed_pair *p asm("r1") = &gUnknown_030007E0;
            register s32 mask asm("r0") = 9;

            mask &= p->pressed;
            if (mask)
                break;
        }

        self->frameParity = (self->frameParity + 1) & 1;
        if (self->frameParity != 0)
            sub_80350A4(self);

        sub_8034EF0(self);
        sub_80006A8();
        sub_803544C(self);
        sub_8034688(self->mapObj);
    }

    sub_8001AC4(gUnknown_030012BC, 0);

    for (i = 0; i <= 0x10; i++) {
        self->frameParity = (self->frameParity + 1) & 1;
        if (self->frameParity != 0)
            sub_80350A4(self);

        sub_8034EF0(self);
        sub_80006A8();
        REG_BLDY = i;
        REG_BLDCNT = 0xff;
        sub_803544C(self);
        sub_8034688(self->mapObj);
    }

    if (self->popupListHead != NULL) {
        void *node = self->popupListHead;
        do {
            void *next = *(void **)node;
            sub_8026ED0(node);
            node = next;
        } while (node != NULL);
    }
    self->popupListHead = NULL;
}

asm(".align 2, 0");

/* The map screen's per-frame OAM-icon draw dispatcher for the minimap
 * object (`self->mapObj`, the `sp[0xc]`-cached argument throughout):
 * for `mapObj->drawMode` (see `struct map_screen` above) 0/1/2 draws a
 * single centered label via `sub_803AD80` against
 * `gUnknown_030012DC`/`gUnknown_030012E0`; for any other drawMode value
 * (docs/rom_map.md's minimap/radar-dot description) DMA3-transfers a
 * procedurally-built tile buffer and iterates a per-tile record array,
 * building each dot's OAM attribute halfwords in place and applying
 * them via `sub_8006AC8`, before advancing to the next linked object in
 * `mapObj`'s list and repeating.
 *
 * Written as NAKED asm, not plain C: the inner tile loop keeps six
 * independent running values live simultaneously across a `bl
 * sub_8006AC8` call inside a nested loop (`sl`/`sb`/`r8`, plus r4-r7) -
 * the same "many high registers held live across calls inside a loop"
 * shape already NAKED throughout this codebase (see
 * docs/matching/issue-63-final-raw-actor.md's `sub_8034994` entry for
 * the fullest write-up of this recurring pattern). Every instruction
 * below, including the mid-function literal-pool placements, is
 * transcribed directly from and checked against the ROM's own
 * disassembly. */
NAKED void sub_8034EF0(struct map_screen *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x24\n\t"
        "str r0, [sp, #0xc]\n\t"
        "ldr r0, _08034F34\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8006A90\n\t"
        "ldr r0, _08034F38\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8006C28\n\t"
        "ldr r0, [sp, #0xc]\n\t"
        "ldr r0, [r0]\n\t"
        "mov sb, r0\n\t"
        "cmp r0, #0\n\t"
        "bne _08034F1A\n\t"
        "b _0803506E\n"
    "_08034F1A:\n\t"
        "mov r1, sp\n\t"
        "add r1, #4\n\t"
        "str r1, [sp, #0x10]\n"
    "_08034F20:\n\t"
        "mov r2, sb\n\t"
        "ldr r0, [r2, #0x10]\n\t"
        "cmp r0, #1\n\t"
        "beq _08034F4C\n\t"
        "cmp r0, #1\n\t"
        "bgt _08034F3C\n\t"
        "cmp r0, #0\n\t"
        "beq _08034F42\n\t"
        "b _08035062\n\t"
        ".align 2, 0\n"
    "_08034F34: .4byte gUnknown_03001300\n"
    "_08034F38: .4byte gUnknown_030012FC\n"
    "_08034F3C:\n\t"
        "cmp r0, #2\n\t"
        "beq _08034F84\n\t"
        "b _08035062\n"
    "_08034F42:\n\t"
        "ldr r0, _08034F48\n\t"
        "b _08034F4E\n\t"
        ".align 2, 0\n"
    "_08034F48: .4byte gUnknown_030012DC\n"
    "_08034F4C:\n\t"
        "ldr r0, _08034F80\n"
    "_08034F4E:\n\t"
        "ldr r3, [r0]\n\t"
        "mov r4, sb\n\t"
        "ldr r1, [r4, #4]\n\t"
        "ldr r2, [r4, #8]\n\t"
        "mov r4, #0x88\n\t"
        "lsl r4, r4, #1\n\t"
        "add r0, r3, r4\n\t"
        "str r1, [r0]\n\t"
        "mov r1, #0x8a\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r3, r1\n\t"
        "str r2, [r0]\n\t"
        "mov r2, #0x98\n\t"
        "lsl r2, r2, #1\n\t"
        "add r0, r3, r2\n\t"
        "ldr r2, [r0]\n\t"
        "mov r4, #0x30\n\t"
        "ldrsh r0, [r2, r4]\n\t"
        "add r0, r3, r0\n\t"
        "mov r3, sb\n\t"
        "ldrb r1, [r3, #0x14]\n\t"
        "ldr r2, [r2, #0x34]\n\t"
        "bl sub_803AD80\n\t"
        "b _08035062\n\t"
        ".align 2, 0\n"
    "_08034F80: .4byte gUnknown_030012E0\n"
    "_08034F84:\n\t"
        "mov r4, sb\n\t"
        "ldrb r4, [r4, #0x14]\n\t"
        "lsl r0, r4, #1\n\t"
        "mov r1, sb\n\t"
        "ldrb r1, [r1, #0x14]\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #3\n\t"
        "add r0, #0x1c\n\t"
        "ldr r2, [sp, #0xc]\n\t"
        "add r6, r2, r0\n\t"
        "ldr r4, _08035088\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8006C44\n\t"
        "mov sl, r0\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r1, [r6, #0x14]\n\t"
        "ldr r3, [r6, #4]\n\t"
        "ldr r2, [r6]\n\t"
        "mul r2, r3, r2\n\t"
        "lsl r2, r2, #9\n\t"
        "bl sub_8006C84\n\t"
        "mov r0, #0\n\t"
        "str r0, [sp]\n\t"
        "mov r0, sp\n\t"
        "ldr r1, [sp, #0x10]\n\t"
        "ldr r2, _0803508C\n\t"
        "bl sub_803A94C\n\t"
        "ldr r3, [sp, #0x10]\n\t"
        "ldrb r1, [r3, #3]\n\t"
        "mov r0, #0x3f\n\t"
        "and r0, r1\n\t"
        "mov r1, #0x80\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r3, #3]\n\t"
        "ldrb r4, [r6, #0x10]\n\t"
        "lsl r1, r4, #4\n\t"
        "mov r0, #0xf\n\t"
        "ldrb r2, [r3, #5]\n\t"
        "and r0, r2\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r3, #5]\n\t"
        "mov r3, sb\n\t"
        "ldr r3, [r3, #8]\n\t"
        "mov r8, r3\n\t"
        "mov r1, #0\n\t"
        "ldr r0, [r6, #4]\n\t"
        "mov r4, sp\n\t"
        "add r4, #4\n\t"
        "str r4, [sp, #0x20]\n\t"
        "cmp r1, r0\n\t"
        "bge _08035062\n"
    "_08034FF0:\n\t"
        "mov r2, r8\n\t"
        "ldr r0, [sp, #0x20]\n\t"
        "strb r2, [r0]\n\t"
        "mov r3, sb\n\t"
        "ldr r5, [r3, #4]\n\t"
        "mov r7, #0\n\t"
        "ldr r0, [r6]\n\t"
        "mov r4, r8\n\t"
        "add r4, #0x20\n\t"
        "str r4, [sp, #0x18]\n\t"
        "add r1, #1\n\t"
        "str r1, [sp, #0x14]\n\t"
        "cmp r7, r0\n\t"
        "bge _08035056\n\t"
        "ldr r4, [sp, #0x20]\n"
    "_0803500E:\n\t"
        "mov r0, r8\n\t"
        "add r0, #0x1f\n\t"
        "cmp r0, #0xbe\n\t"
        "bhi _08035048\n\t"
        "ldr r1, _08035090\n\t"
        "add r0, r1, #0\n\t"
        "add r2, r5, #0\n\t"
        "and r2, r0\n\t"
        "ldrh r0, [r4, #2]\n\t"
        "ldr r3, _08035094\n\t"
        "add r1, r3, #0\n\t"
        "and r0, r1\n\t"
        "orr r0, r2\n\t"
        "strh r0, [r4, #2]\n\t"
        "ldr r1, _08035098\n\t"
        "add r0, r1, #0\n\t"
        "mov r1, sl\n\t"
        "and r1, r0\n\t"
        "ldr r2, _0803509C\n\t"
        "add r0, r2, #0\n\t"
        "ldrh r3, [r4, #4]\n\t"
        "and r0, r3\n\t"
        "orr r0, r1\n\t"
        "strh r0, [r4, #4]\n\t"
        "ldr r0, _080350A0\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8006AC8\n"
    "_08035048:\n\t"
        "mov r0, #0x10\n\t"
        "add sl, r0\n\t"
        "add r5, #0x20\n\t"
        "add r7, #1\n\t"
        "ldr r0, [r6]\n\t"
        "cmp r7, r0\n\t"
        "blt _0803500E\n"
    "_08035056:\n\t"
        "ldr r1, [sp, #0x18]\n\t"
        "mov r8, r1\n\t"
        "ldr r1, [sp, #0x14]\n\t"
        "ldr r0, [r6, #4]\n\t"
        "cmp r1, r0\n\t"
        "blt _08034FF0\n"
    "_08035062:\n\t"
        "mov r2, sb\n\t"
        "ldr r2, [r2]\n\t"
        "mov sb, r2\n\t"
        "cmp r2, #0\n\t"
        "beq _0803506E\n\t"
        "b _08034F20\n"
    "_0803506E:\n\t"
        "ldr r0, _080350A0\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8006A48\n\t"
        "add sp, #0x24\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "_08035088: .4byte gUnknown_030012FC\n"
    "_0803508C: .4byte 0x05000002\n"
    "_08035090: .4byte 0x000001FF\n"
    "_08035094: .4byte 0xFFFFFE00\n"
    "_08035098: .4byte 0x000003FF\n"
    "_0803509C: .4byte 0xFFFFFC00\n"
    "_080350A0: .4byte gUnknown_03001300\n"
    );
}

asm(".align 2, 0");

/* The map screen's floating-text popup driver (docs/rom_map.md's "A
 * floating-text/glyph popup system" note): first walks `self`'s
 * `popupListHead` linked list, decrementing each node's countdown pair
 * (`+8`/`+0xc`) and unlinking/freeing (`sub_8026ED0`) any node whose
 * sum has expired; then, unless `self->suppressCounter` is still
 * counting down, parses `self`'s byte-opcode stream
 * (`streamCursor`/`streamBase`, `struct map_screen`) - opcodes 0/1
 * allocate and link a new 0x18-byte popup node (mode 0/1 respectively),
 * 2/3 set `self->drawMode`, 0xA terminates a line (measuring both
 * `gUnknown_030012DC`/`030012E0`'s text width via `sub_8028968` first)
 * - drawing the assembled line centered via `sub_803AD84` once `self`'s
 * `drawMode` is known, then finally re-derives `self->suppressCounter`
 * from the measured line width and walks the popup list one more time
 * shifting each node horizontally into position.
 *
 * Written as NAKED asm, not plain C: `self` (r5), the list-tail pointer
 * (r8), the running max-width accumulator (sb), and the horizontal
 * pen-position accumulator (sl) all stay resident across many `bl`
 * sites spanning several nested loops - the same "many high registers
 * held live across calls, no spare register" shape already NAKED
 * throughout this codebase (see docs/matching/issue-63-final-raw-actor.md's
 * `sub_8034994` entry). Every instruction below, including the
 * mid-function literal-pool placements, is transcribed directly from
 * and checked against the ROM's own disassembly. */
NAKED void sub_80350A4(struct map_screen *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x10\n\t"
        "add r5, r0, #0\n\t"
        "add r4, r5, #0\n\t"
        "ldr r0, [r5]\n\t"
        "cmp r0, #0\n\t"
        "beq _080350DE\n"
    "_080350BA:\n\t"
        "ldr r2, [r4]\n\t"
        "ldr r0, [r2, #8]\n\t"
        "sub r0, #1\n\t"
        "str r0, [r2, #8]\n\t"
        "ldr r1, [r2, #0xc]\n\t"
        "add r0, r0, r1\n\t"
        "cmp r0, #0\n\t"
        "bgt _080350D6\n\t"
        "ldr r0, [r2]\n\t"
        "str r0, [r4]\n\t"
        "add r0, r2, #0\n\t"
        "bl sub_8026ED0\n\t"
        "b _080350D8\n"
    "_080350D6:\n\t"
        "add r4, r2, #0\n"
    "_080350D8:\n\t"
        "ldr r0, [r4]\n\t"
        "cmp r0, #0\n\t"
        "bne _080350BA\n"
    "_080350DE:\n\t"
        "ldr r0, [r5, #0x14]\n\t"
        "cmp r0, #0\n\t"
        "beq _080350EA\n\t"
        "sub r0, #1\n\t"
        "str r0, [r5, #0x14]\n\t"
        "b _08035298\n"
    "_080350EA:\n\t"
        "mov r8, r5\n\t"
        "ldr r0, [r5]\n\t"
        "ldr r1, _08035154\n\t"
        "ldr r4, _08035158\n\t"
        "cmp r0, #0\n\t"
        "beq _08035102\n"
    "_080350F6:\n\t"
        "mov r0, r8\n\t"
        "ldr r0, [r0]\n\t"
        "mov r8, r0\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne _080350F6\n"
    "_08035102:\n\t"
        "mov r7, r8\n\t"
        "ldr r0, [r1]\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8028968\n\t"
        "str r0, [sp]\n\t"
        "ldr r0, _0803515C\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8028968\n\t"
        "str r0, [sp, #4]\n\t"
        "ldr r1, [sp]\n\t"
        "mov sb, r1\n\t"
        "mov r2, #0\n\t"
        "mov sl, r2\n\t"
        "ldr r0, [r5, #8]\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne _0803512E\n\t"
        "ldr r0, [r5, #4]\n\t"
        "str r0, [r5, #8]\n"
    "_0803512E:\n\t"
        "ldr r4, [r5, #8]\n\t"
        "ldrb r0, [r4]\n\t"
        "cmp r0, #0xa\n\t"
        "bne _08035138\n\t"
        "b _08035254\n"
    "_08035138:\n\t"
        "cmp r0, #0\n\t"
        "bne _0803513E\n\t"
        "b _0803524C\n"
    "_0803513E:\n\t"
        "add r3, r5, #0\n\t"
        "add r3, #0x24\n\t"
        "str r3, [sp, #8]\n"
    "_08035144:\n\t"
        "mov r2, #0\n\t"
        "mov r6, #0\n\t"
        "ldrb r0, [r4]\n\t"
        "cmp r0, #2\n\t"
        "bne _08035160\n\t"
        "str r6, [r5, #0x10]\n\t"
        "b _08035230\n\t"
        ".align 2, 0\n"
    "_08035154: .4byte gUnknown_030012DC\n"
    "_08035158: .4byte gStaticData_0817CF3C\n"
    "_0803515C: .4byte gUnknown_030012E0\n"
    "_08035160:\n\t"
        "cmp r0, #3\n\t"
        "bne _0803516A\n\t"
        "mov r0, #1\n\t"
        "str r0, [r5, #0x10]\n\t"
        "b _08035230\n"
    "_0803516A:\n\t"
        "cmp r0, #1\n\t"
        "bne _080351B4\n\t"
        "add r0, r4, #1\n\t"
        "str r0, [r5, #8]\n\t"
        "mov r0, #0x18\n\t"
        "bl sub_8026EDC\n\t"
        "str r0, [r7]\n\t"
        "mov r1, #2\n\t"
        "str r1, [r0, #0x10]\n\t"
        "str r6, [r0, #8]\n\t"
        "mov r1, sl\n\t"
        "str r1, [r0, #4]\n\t"
        "ldr r1, [r5, #8]\n\t"
        "ldrb r1, [r1]\n\t"
        "strb r1, [r0, #0x14]\n\t"
        "lsl r1, r1, #1\n\t"
        "ldrb r2, [r0, #0x14]\n\t"
        "add r1, r1, r2\n\t"
        "lsl r1, r1, #3\n\t"
        "ldr r3, [sp, #8]\n\t"
        "add r1, r3, r1\n\t"
        "ldr r1, [r1]\n\t"
        "str r1, [r0, #0xc]\n\t"
        "str r6, [r0]\n\t"
        "add r7, r0, #0\n\t"
        "ldrb r0, [r7, #0x14]\n\t"
        "lsl r1, r0, #1\n\t"
        "add r1, r1, r0\n\t"
        "lsl r1, r1, #3\n\t"
        "add r0, r3, r1\n\t"
        "ldr r6, [r0]\n\t"
        "add r0, r5, #0\n\t"
        "add r0, #0x28\n\t"
        "add r0, r0, r1\n\t"
        "ldr r2, [r0]\n\t"
        "b _08035230\n"
    "_080351B4:\n\t"
        "ldr r0, [r5, #0x10]\n\t"
        "cmp r0, #0\n\t"
        "bne _080351E0\n\t"
        "ldr r0, _080351DC\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x98\n\t"
        "lsl r2, r2, #1\n\t"
        "add r1, r0, r2\n\t"
        "ldr r2, [r1]\n\t"
        "mov r3, #0x18\n\t"
        "ldrsh r1, [r2, r3]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r3, [r2, #0x1c]\n\t"
        "add r1, r4, #0\n\t"
        "mov r2, #1\n\t"
        "bl sub_803AD84\n\t"
        "add r2, r0, #0\n\t"
        "ldr r6, [sp]\n\t"
        "b _08035200\n\t"
        ".align 2, 0\n"
    "_080351DC: .4byte gUnknown_030012DC\n"
    "_080351E0:\n\t"
        "ldr r0, _080352A8\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x98\n\t"
        "lsl r2, r2, #1\n\t"
        "add r1, r0, r2\n\t"
        "ldr r2, [r1]\n\t"
        "mov r3, #0x18\n\t"
        "ldrsh r1, [r2, r3]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r3, [r2, #0x1c]\n\t"
        "add r1, r4, #0\n\t"
        "mov r2, #1\n\t"
        "bl sub_803AD84\n\t"
        "add r2, r0, #0\n\t"
        "ldr r6, [sp, #4]\n"
    "_08035200:\n\t"
        "ldr r0, [r5, #8]\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0x20\n\t"
        "beq _08035230\n\t"
        "mov r0, #0x18\n\t"
        "str r2, [sp, #0xc]\n\t"
        "bl sub_8026EDC\n\t"
        "str r0, [r7]\n\t"
        "ldr r1, [r5, #0x10]\n\t"
        "str r1, [r0, #0x10]\n\t"
        "mov r1, #0\n\t"
        "str r1, [r0, #8]\n\t"
        "mov r3, sl\n\t"
        "str r3, [r0, #4]\n\t"
        "ldr r1, [r5, #8]\n\t"
        "ldrb r1, [r1]\n\t"
        "strb r1, [r0, #0x14]\n\t"
        "ldr r0, [r7]\n\t"
        "str r6, [r0, #0xc]\n\t"
        "add r7, r0, #0\n\t"
        "mov r0, #0\n\t"
        "str r0, [r7]\n\t"
        "ldr r2, [sp, #0xc]\n"
    "_08035230:\n\t"
        "add sl, r2\n\t"
        "cmp sb, r6\n\t"
        "bge _08035238\n\t"
        "mov sb, r6\n"
    "_08035238:\n\t"
        "ldr r0, [r5, #8]\n\t"
        "add r1, r0, #1\n\t"
        "str r1, [r5, #8]\n\t"
        "ldrb r0, [r0, #1]\n\t"
        "cmp r0, #0xa\n\t"
        "beq _08035254\n\t"
        "add r4, r1, #0\n\t"
        "cmp r0, #0\n\t"
        "beq _0803524C\n\t"
        "b _08035144\n"
    "_0803524C:\n\t"
        "ldr r0, [r5, #8]\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0xa\n\t"
        "bne _0803525A\n"
    "_08035254:\n\t"
        "ldr r0, [r5, #8]\n\t"
        "add r0, #1\n\t"
        "str r0, [r5, #8]\n"
    "_0803525A:\n\t"
        "mov r1, r8\n\t"
        "ldr r3, [r1]\n\t"
        "mov r6, sb\n\t"
        "add r6, #6\n\t"
        "cmp r3, #0\n\t"
        "beq _08035296\n\t"
        "mov r0, #0xf0\n\t"
        "mov r2, sl\n\t"
        "sub r0, r0, r2\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r4, r0, #1\n"
    "_08035272:\n\t"
        "ldr r0, [r3, #8]\n\t"
        "add r2, r0, #0\n\t"
        "add r2, #0xa0\n\t"
        "ldr r1, [r3, #0xc]\n\t"
        "add r0, r0, r1\n\t"
        "mov r1, sb\n\t"
        "sub r0, r1, r0\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r0, r0, #1\n\t"
        "add r2, r2, r0\n\t"
        "str r2, [r3, #8]\n\t"
        "ldr r0, [r3, #4]\n\t"
        "add r0, r0, r4\n\t"
        "str r0, [r3, #4]\n\t"
        "ldr r3, [r3]\n\t"
        "cmp r3, #0\n\t"
        "bne _08035272\n"
    "_08035296:\n\t"
        "str r6, [r5, #0x14]\n"
    "_08035298:\n\t"
        "add sp, #0x10\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "_080352A8: .4byte gUnknown_030012E0\n"
    );
}

asm(".align 2, 0");

/* The map screen's popup-text asset loader (docs/rom_map.md's
 * `sub_80352AC` note): iterates `gStaticData_0817CF40`'s 5 records
 * (stride 0x14) into `self->asset0`-`asset4` (`struct map_screen` above
 * - each `sp[4]+0x1c+i*0x18`-relative in the ROM's own indexing),
 * converting each record's raw width/height into rounded Q-something
 * runtime units, DMA3-transferring custom glyph tile data
 * (`sub_8026EC0`/`LoadTaggedAsset`) into a freshly-decoded buffer and
 * building each glyph cell's OAM tile index via a nested nibble/row
 * loop, then loading the shared 15-color palette tail
 * (`gUnknown_030012B8+0x2c`) the same way and pinning the freshly-built
 * asset into the shared tile cache (`sub_8006D50`).
 *
 * Written as NAKED asm, not plain C: the innermost tile-index loop
 * holds seven live values simultaneously (`self`'s asset-record pointer
 * in r8, two masks in sl/sb, the palette-tail base in ip, plus r0-r6)
 * across a fully packed register budget, the same "no spare register"
 * wall already NAKED throughout this codebase. Every instruction below,
 * including the mid-function literal-pool placement, is transcribed
 * directly from and checked against the ROM's own disassembly. */
NAKED void sub_80352AC(struct map_screen *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x24\n\t"
        "str r0, [sp, #4]\n\t"
        "ldr r0, _0803543C\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, #0x2c\n\t"
        "str r0, [sp, #8]\n\t"
        "mov r0, #1\n\t"
        "str r0, [sp, #0xc]\n\t"
        "mov r5, #0\n"
    "_080352C8:\n\t"
        "lsl r0, r5, #2\n\t"
        "add r0, r0, r5\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, _08035440\n\t"
        "add r7, r0, r1\n\t"
        "lsl r0, r5, #1\n\t"
        "add r0, r0, r5\n\t"
        "lsl r0, r0, #3\n\t"
        "add r0, #0x1c\n\t"
        "ldr r1, [sp, #4]\n\t"
        "add r1, r1, r0\n\t"
        "mov r8, r1\n\t"
        "ldr r1, [r7]\n\t"
        "ldr r2, [r7, #4]\n\t"
        "lsl r0, r2, #3\n\t"
        "mov r3, r8\n\t"
        "str r0, [r3, #8]\n\t"
        "lsl r0, r1, #3\n\t"
        "str r0, [r3, #0xc]\n\t"
        "add r0, r1, #3\n\t"
        "cmp r0, #0\n\t"
        "bge _080352F6\n\t"
        "add r0, r1, #6\n"
    "_080352F6:\n\t"
        "asr r0, r0, #2\n\t"
        "mov r1, r8\n\t"
        "str r0, [r1]\n\t"
        "add r0, r2, #3\n\t"
        "cmp r0, #0\n\t"
        "bge _08035304\n\t"
        "add r0, r2, #6\n"
    "_08035304:\n\t"
        "asr r0, r0, #2\n\t"
        "mov r2, r8\n\t"
        "str r0, [r2, #4]\n\t"
        "ldr r0, [r7, #0xc]\n\t"
        "ldr r0, [r0]\n\t"
        "lsr r0, r0, #8\n\t"
        "bl sub_8026EC0\n\t"
        "str r0, [sp, #0x10]\n\t"
        "ldr r0, [r7, #0xc]\n\t"
        "ldr r1, [sp, #0x10]\n\t"
        "bl LoadTaggedAsset\n\t"
        "mov r3, r8\n\t"
        "ldr r1, [r3]\n\t"
        "ldr r0, [r3, #4]\n\t"
        "add r4, r1, #0\n\t"
        "mul r4, r0, r4\n\t"
        "lsl r4, r4, #9\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8026EC0\n\t"
        "mov r1, r8\n\t"
        "str r0, [r1, #0x14]\n\t"
        "mov r1, #0\n\t"
        "str r1, [sp]\n\t"
        "ldr r2, _08035444\n\t"
        "mov r3, sp\n\t"
        "str r3, [r2]\n\t"
        "str r0, [r2, #4]\n\t"
        "cmp r4, #0\n\t"
        "bge _08035346\n\t"
        "add r4, #3\n"
    "_08035346:\n\t"
        "asr r0, r4, #2\n\t"
        "mov r1, #0x85\n\t"
        "lsl r1, r1, #0x18\n\t"
        "orr r0, r1\n\t"
        "str r0, [r2, #8]\n\t"
        "ldr r0, [r2, #8]\n\t"
        "mov r6, #0\n\t"
        "ldr r0, [r7, #4]\n\t"
        "ldr r1, [sp, #0xc]\n\t"
        "add r1, #1\n\t"
        "str r1, [sp, #0x18]\n\t"
        "add r5, #1\n\t"
        "str r5, [sp, #0x1c]\n\t"
        "cmp r6, r0\n\t"
        "bge _080353CC\n"
    "_08035364:\n\t"
        "mov r4, #0\n\t"
        "ldr r3, [r7]\n\t"
        "add r2, r6, #1\n\t"
        "str r2, [sp, #0x20]\n\t"
        "cmp r4, r3\n\t"
        "bge _080353C4\n\t"
        "asr r0, r6, #2\n\t"
        "str r0, [sp, #0x14]\n\t"
        "mov r1, #3\n\t"
        "mov sl, r1\n\t"
        "add r0, r6, #0\n\t"
        "and r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "mov sb, r0\n\t"
        "ldr r5, _08035444\n\t"
        "mov r2, r8\n\t"
        "ldr r2, [r2, #0x14]\n\t"
        "mov ip, r2\n"
    "_08035388:\n\t"
        "mov r1, r8\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r2, [sp, #0x14]\n\t"
        "add r1, r2, #0\n\t"
        "mul r1, r0, r1\n\t"
        "asr r0, r4, #2\n\t"
        "add r1, r1, r0\n\t"
        "add r2, r4, #0\n\t"
        "mov r0, sl\n\t"
        "and r2, r0\n\t"
        "add r2, sb\n\t"
        "add r0, r6, #0\n\t"
        "mul r0, r3, r0\n\t"
        "add r0, r0, r4\n\t"
        "lsl r0, r0, #5\n\t"
        "ldr r3, [sp, #0x10]\n\t"
        "add r0, r3, r0\n\t"
        "str r0, [r5]\n\t"
        "lsl r1, r1, #4\n\t"
        "add r1, r1, r2\n\t"
        "lsl r1, r1, #5\n\t"
        "add r1, ip\n\t"
        "str r1, [r5, #4]\n\t"
        "ldr r0, _08035448\n\t"
        "str r0, [r5, #8]\n\t"
        "ldr r0, [r5, #8]\n\t"
        "add r4, #1\n\t"
        "ldr r3, [r7]\n\t"
        "cmp r4, r3\n\t"
        "blt _08035388\n"
    "_080353C4:\n\t"
        "ldr r6, [sp, #0x20]\n\t"
        "ldr r0, [r7, #4]\n\t"
        "cmp r6, r0\n\t"
        "blt _08035364\n"
    "_080353CC:\n\t"
        "ldr r0, [sp, #0x10]\n\t"
        "cmp r0, #0\n\t"
        "beq _080353D6\n\t"
        "bl sub_8026EB4\n"
    "_080353D6:\n\t"
        "ldr r0, [r7, #8]\n\t"
        "ldr r0, [r0]\n\t"
        "lsr r0, r0, #9\n\t"
        "lsl r0, r0, #1\n\t"
        "bl sub_8026EC0\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, [r7, #8]\n\t"
        "add r1, r4, #0\n\t"
        "bl LoadTaggedAsset\n\t"
        "add r2, r4, #0\n\t"
        "ldr r1, [sp, #0xc]\n\t"
        "lsl r0, r1, #5\n\t"
        "ldr r3, [sp, #8]\n\t"
        "add r1, r0, r3\n\t"
        "mov r3, #0xf\n"
    "_080353F8:\n\t"
        "ldrh r0, [r2]\n\t"
        "strh r0, [r1]\n\t"
        "add r2, #2\n\t"
        "add r1, #2\n\t"
        "sub r3, #1\n\t"
        "cmp r3, #0\n\t"
        "bge _080353F8\n\t"
        "cmp r4, #0\n\t"
        "beq _08035410\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8026EB4\n"
    "_08035410:\n\t"
        "ldr r0, _0803543C\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [sp, #0xc]\n\t"
        "bl sub_8006D50\n\t"
        "ldr r0, [sp, #0xc]\n\t"
        "mov r1, r8\n\t"
        "str r0, [r1, #0x10]\n\t"
        "ldr r2, [sp, #0x18]\n\t"
        "str r2, [sp, #0xc]\n\t"
        "ldr r5, [sp, #0x1c]\n\t"
        "cmp r5, #4\n\t"
        "bgt _0803542C\n\t"
        "b _080352C8\n"
    "_0803542C:\n\t"
        "add sp, #0x24\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "_0803543C: .4byte gUnknown_030012B8\n"
    "_08035440: .4byte gStaticData_0817CF40\n"
    "_08035444: .4byte 0x040000D4\n"
    "_08035448: .4byte 0x84000008\n"
    );
}

asm(".align 2, 0");

extern void sub_8001614(void);
extern void sub_8006DC8(struct tile_asset_cache *arg0);
extern struct tile_asset_cache *gUnknown_030012B8;

/* --------------------------------------------------------------------
 * sub_803544C - end-of-frame commit for the map screen: resets BG0's
 * scroll registers, flushes the shared tile cache and OAM shadow
 * buffer, and flushes the VRAM upload queue. Takes (and ignores) an
 * unused `self` argument - both of sub_8034E2C's call sites pass it
 * anyway (leftover from a shared call-site shape with its neighbors),
 * so the parameter is kept here rather than dropped, to match the
 * ROM's own call sites byte-for-byte. */
void sub_803544C(void *unused)
{
    sub_8001614();
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    sub_8006DC8(gUnknown_030012B8);
    sub_8006AAC(gUnknown_03001300);
    FlushVramDmaQueue();
}

extern void sub_80346FC(void *self, s32 arg1);
extern void sub_8026EB4(void *ptr);

/* --------------------------------------------------------------------
 * sub_803547C - map screen teardown: kicks the minimap object's own
 * teardown (`sub_80346FC(mapObj, 3)`) if one was ever built, frees each
 * of the five popup-asset buffers still allocated, then frees `self`
 * too when `mode` bit 0 is set.
 * ------------------------------------------------------------------ */
void sub_803547C(struct map_screen *self, s32 mode)
{
    u8 *slot;
    s32 i;

    if (self->mapObj != NULL)
        sub_80346FC(self->mapObj, 3);

    slot = (u8 *)&self->asset0;
    i = 4;
    do {
        if (*(void **)slot != NULL)
            sub_8026EB4(*(void **)slot);
        slot += 0x18;
        i--;
    } while (i >= 0);

    if (mode & 1)
        sub_8026ED0(self);
}

/* --------------------------------------------------------------------
 * sub_80354BC - the between-level map/progress screen's top-level
 * entry point (docs/rom_map.md's "A fourth thing in this file"
 * section): allocates and constructs the screen (sub_8034CEC), runs it
 * to completion (sub_8034E2C), and tears it down (sub_803547C).
 * ------------------------------------------------------------------ */
void sub_80354BC(void)
{
    struct map_screen *self = sub_8034CEC(sub_8026EDC(0x98));

    sub_8034E2C(self);
    if (self != NULL)
        sub_803547C(self, 3);
}

asm(".align 2, 0");
