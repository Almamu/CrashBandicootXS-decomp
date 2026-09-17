#include "core.h"
#include "settings_sync.h"
#include "pause_options_screen.h"

extern void sub_8002C14(void *handle, s32 rowIndex, void *buf);
extern void sub_8002C40(void *handle, s32 rowIndex, void *buf);
extern void sub_8002C6C(void *handle, s32 rowIndex);

void sub_8002FCC(struct settings_sync_pump *self, struct settings_sync_record *tmpl)
{
    self->tmpl = tmpl;
    self->cursor = (u8 *)tmpl;
}

void *sub_8002FD4(struct settings_sync_pump *self)
{
    return self->data;
}

void sub_8002FD8(struct settings_sync_pump *self)
{
    self->remaining = sizeof(self->data);
    self->totalReceived = 0;
    self->cursor = (u8 *)self->tmpl;
    self->writePtr = self->data;
    self->field_214 = 0;
    self->field_218 = 0;
    self->field_21c = 0;
}

extern void *gUnknown_03001304;
/* gUnknown_030007E0 is a plain u32 elsewhere (e.g.
 * src/graphics/settings_menu.c's sub_8003B40) but this call site reads
 * only its upper 16 bits (the "newly pressed" half of a held/pressed
 * input pair) - matching the ROM's own `ldrh r1,[r0,#2]` (a runtime
 * +2 byte offset on the reloaded base address) requires a real field
 * access here rather than `(u8*)&gUnknown_030007E0 + 2`, which the
 * compiler folds into the linker-relocated constant instead. */
struct held_pressed_pair {
    u16 held;
    u16 pressed;
};
extern struct held_pressed_pair gUnknown_030007E0;
extern void sub_80007AC(void *arg0);
extern void sub_8004BD0(struct pause_options_screen *self);
extern void sub_80006A8(void);
extern void sub_8004CE8(struct pause_options_screen *self);
extern void *gUnknown_0300080C;
extern void sub_80031E4(struct pause_options_screen *self, u32 keys);

/* The "connecting..." spinner dialog's blocking modal loop: sets up
 * `gUnknown_0300080C`'s `state`/`field_10`/`flags`/`field_8`/`field_20`,
 * then repeatedly dispatches input (sub_80031E4) through
 * sub_8004BD0's state machine, VBlank-waits, and restores display
 * registers (sub_8004CE8) until `field_8` (set by one of the state
 * handlers below) requests an exit. Returns `field_20`, the handlers'
 * "result ready" flag. */
u8 sub_800300C(u32 state, u32 field10)
{
    struct pause_options_screen **selfAddr = (struct pause_options_screen **)&gUnknown_0300080C;
    struct pause_options_screen *self;

    self = *selfAddr;
    self->state = state;
    self->field_10 = field10;
    self->flags = 0;
    self->field_8 = 0;
    (*selfAddr)->field_20 = 0;

    goto dispatch;
    for (;;) {
        u16 keys;

        sub_80007AC(gUnknown_03001304);
        keys = gUnknown_030007E0.pressed;
        sub_80031E4(*selfAddr, keys);
    dispatch:
        sub_8004BD0(*selfAddr);
        sub_80006A8();
        sub_8004CE8(*selfAddr);
        if ((*selfAddr)->field_8 != 0) {
            break;
        }
    }
    return ((struct pause_options_screen *)gUnknown_0300080C)->field_20;
}

extern void *sub_8026EDC(s32 size);
extern void *gUnknown_030012BC;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern u8 sub_8002B44(void *arg0);
extern struct tile_asset_cache *gUnknown_030012B8;
extern void sub_8006EA8(struct tile_asset_cache *self);
extern void sub_800450C(struct pause_options_screen *self);
extern void sub_80047F8(struct pause_options_screen *self);
extern void sub_8001B54(void *arg0, s32 arg1);
extern void sub_80048BC(struct pause_options_screen *self);
extern void *gUnknown_030012C0;
extern void *sub_80236EC(void *arg0);
extern void sub_80048E0(void *self, struct settings_row_stats *dest, void *src);
extern void sub_8004860(struct pause_options_screen *self, void *handle);
extern void *sub_80016DC(s32 size);
extern void *sub_80027E8(void *arg0);
extern void sub_800132C(u8 flags, s32 frameDelay, u8 sync);
extern void sub_8002C84(struct settings_sync_record *self);

/* The composite pause/options screen's (and the spinner dialog's, via
 * sub_800306C above) `field_8c`/`field_90` constructor: allocates and
 * initialises both settings_sync_record instances (sub_8002C84), does
 * the screen's tile/BG/list setup (sub_800450C/sub_80047F8/
 * sub_80048BC, still raw), fills `currentStats` and the first
 * `rowStats` entry, then allocates and stashes the global SIO session
 * object (`gUnknown_03000804`, still uncharacterized - see
 * sub_8002D44/sub_8002E20, src/graphics/settings_menu8.c) and kicks off
 * a VBlank IRQ request. */
struct pause_options_screen *sub_800306C(struct pause_options_screen *arg0)
{
    register struct pause_options_screen *self asm("r5") = arg0;
    register void **field8cAddr asm("r9") = &self->field_8c;
    register void **field90Addr asm("r8");
    register s32 size asm("r6") = 0x200;
    register void *obj asm("r4");
    extern void *gUnknown_03000804;

    obj = sub_8026EDC(size);
    sub_8002C84(obj);
    *field8cAddr = obj;

    field90Addr = &self->field_90;
    obj = sub_8026EDC(size);
    sub_8002C84(obj);
    *field90Addr = obj;

    sub_8006EA8(gUnknown_030012B8);
    sub_800450C(self);
    sub_80047F8(self);
    sub_8001B54(gUnknown_030012BC, 0x10);
    sub_80048BC(self);
    sub_80048E0(self, &self->currentStats, sub_80236EC(gUnknown_030012C0));
    sub_8004860(self, *field8cAddr);

    {
        register void **sessionAddr asm("r4") = &gUnknown_03000804;
        *sessionAddr = sub_80027E8(sub_80016DC(0x408));
    }
    sub_800132C(0x80, 1, 0);
    self->field_20 = 0;
    return self;
}

extern void sub_80027B0(void *arg0, s32 arg1);
extern void sub_8026ED0(void *arg0);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);

/* Tears down the composite screen's (or spinner dialog's) field_8c/
 * field_90 pair, cancels the global SIO session object if one's still
 * active, erases each of the five settings-row icon widgets
 * (rowObjA/B/C, drawing a "blank" glyph via sub_803AD80's arg1=3), and
 * - only when `flags` bit 0 is set - destroys `self` itself. */
void sub_800312C(struct pause_options_screen *self, u32 flags)
{
    register void **c asm("r6");
    register void **b asm("r5");
    register void **a asm("r4");
    register s32 n asm("r8");
    extern void *gUnknown_03000804;

    if (gUnknown_03000804 != NULL) {
        sub_80027B0(gUnknown_03000804, 3);
    }

    sub_8026ED0(*(void **)((u8 *)self + 0x90));
    sub_8026ED0(*(void **)((u8 *)self + 0x8c));

    c = (void **)self->rowObjC;
    b = (void **)self->rowObjB;
    a = (void **)self->rowObjA;

    n = 4;
    do {
        void *obj;
        register u8 *p asm("r1");

        /* rowObjA/B/C[i]'s icon descriptor at +0x18 holds a {s16 offset,
         * u8 pad[2], void *fn} record at +0x50 - the same (offset, fn)
         * shape icon_manager's own record slots use elsewhere in this
         * screen, just inside a different, still-uncharacterized
         * container type. */
        obj = *a;
        if (obj != NULL) {
            p = *(u8 **)((u8 *)obj + 0x18) + 0x50;
            sub_803AD80((u8 *)obj + *(s16 *)p, (void *)3, *(void **)(p + 4));
        }
        obj = *b;
        if (obj != NULL) {
            p = *(u8 **)((u8 *)obj + 0x18) + 0x50;
            sub_803AD80((u8 *)obj + *(s16 *)p, (void *)3, *(void **)(p + 4));
        }
        obj = *c;
        if (obj != NULL) {
            p = *(u8 **)((u8 *)obj + 0x18) + 0x50;
            sub_803AD80((u8 *)obj + *(s16 *)p, (void *)3, *(void **)(p + 4));
        }
        c++;
        b++;
        a++;
        n--;
    } while (n >= 0);

    if (flags & 1) {
        sub_8026ED0(self);
    }
}

extern void sub_803AD7C(void *arg0, void *fn);
extern void sub_80032E8(struct pause_options_screen *self, u32 keys);
extern void sub_80034BC(struct pause_options_screen *self, u32 keys, void *handle);
extern void sub_80035C0(struct pause_options_screen *self);
extern void sub_8004CB4(struct pause_options_screen *self, u32 keys);
extern void sub_8003824(struct pause_options_screen *self, u32 keys);
extern void sub_80038D0(struct pause_options_screen *self, u32 keys);
extern void sub_800376C(struct pause_options_screen *self, u32 keys);
extern void sub_800397C(struct pause_options_screen *self, u32 keys);

/* Per-frame input dispatch for the composite screen (or, via
 * sub_800300C above, the spinner dialog sharing the same struct shape):
 * redraws all 15 settings-row icon widgets (rowObjA/B/C[0..4] - same
 * still-uncharacterized descriptor shape as sub_800312C above), then
 * dispatches `keys` to whichever per-`state` handler is active, and
 * finally advances `flags` (as a wrapping 0-0xff per-frame counter) and
 * `field_0` (as a plain per-frame tick). */
void sub_80031E4(struct pause_options_screen *self, u32 keys)
{
    s32 i;

    for (i = 0; i <= 4; i++) {
        void **p;
        s16 off;

        p = *(void ***)((u8 *)self->rowObjA[i] + 0x18);
        off = *(s16 *)((u8 *)p + 0x18);
        sub_803AD7C((u8 *)self->rowObjA[i] + off, *(void **)((u8 *)p + 0x1c));

        p = *(void ***)((u8 *)self->rowObjB[i] + 0x18);
        off = *(s16 *)((u8 *)p + 0x18);
        sub_803AD7C((u8 *)self->rowObjB[i] + off, *(void **)((u8 *)p + 0x1c));

        p = *(void ***)((u8 *)self->rowObjC[i] + 0x18);
        off = *(s16 *)((u8 *)p + 0x18);
        sub_803AD7C((u8 *)self->rowObjC[i] + off, *(void **)((u8 *)p + 0x1c));
    }

    if ((u32)self->state <= 0xa) {
        switch (self->state) {
        case 0:
            sub_80032E8(self, keys);
            break;
        case 1:
            sub_80034BC(self, keys, self->field_8c);
            break;
        case 2:
            sub_80034BC(self, keys, self->field_90);
            break;
        case 3:
            sub_80035C0(self);
            break;
        case 4:
            sub_8004CB4(self, keys);
            break;
        case 5:
            sub_8003824(self, keys);
            break;
        case 6:
            sub_80038D0(self, keys);
            break;
        case 9:
            sub_800376C(self, keys);
            break;
        case 8:
            break;
        case 7:
            sub_800397C(self, keys);
            break;
        case 10:
            break;
        }
    }

    self->flags = (self->flags + 1) & 0xff;
    self->field_0 += 1;
}

extern s32 sub_8026F38(s32 arg0);
extern void sub_8004A80(struct pause_options_screen *self);

/* State 0's input handler: cancel/confirm-combo (bits 1/3) requests an
 * exit; confirm (bit 0) advances through this state's own little
 * sub-menu (`field_10` 0-4, mirroring the sub_8004BD0 states each
 * selects); L/R (bits 6/7) move the `field_10` cursor with wraparound. */
void sub_80032E8(struct pause_options_screen *self, u32 flags)
{
    if (flags & 0xa) {
        PlaySfx(gUnknown_030012BC, 0x47, 0x100);
        self->field_8 = 1;
        return;
    }
    if (flags & 1) {
        PlaySfx(gUnknown_030012BC, 0x49, 0x100);
        switch (self->field_10) {
        case 0:
            self->state = 1;
            self->field_10 = 0;
            sub_8004860(self, self->field_8c);
            break;
        case 1:
            self->state = 3;
            self->field_10 = 0;
            self->field_14 = sub_8026F38(0x2b);
            self->field_18 = sub_8026F38(0x2d);
            sub_8004A80(self);
            break;
        case 2:
            self->state = 5;
            self->field_10 = 0;
            sub_8004860(self, self->field_8c);
            break;
        case 3:
            self->state = 6;
            self->field_10 = 0;
            sub_8004860(self, self->field_8c);
            break;
        case 4:
            self->field_8 = 1;
            break;
        }
        return;
    }
    if (flags & 0x40) {
        PlaySfx(gUnknown_030012BC, 0x46, 0x100);
        self->field_10 -= 1;
        if (self->field_10 < 0) {
            self->field_10 = 4;
        }
    } else if (flags & 0x80) {
        PlaySfx(gUnknown_030012BC, 0x46, 0x100);
        self->field_10 += 1;
        if (self->field_10 > 4) {
            self->field_10 = 0;
        }
    }
}

/* L/R-only row-cursor mover shared by several of this screen's other
 * states (called directly by several handlers below when their own
 * confirm/cancel bits are clear). */
void sub_80033E8(struct pause_options_screen *self, u32 flags)
{
    if (flags & 0x40) {
        PlaySfx(gUnknown_030012BC, 0x46, 0x100);
        if ((u32)self->field_10 <= 4) {
            switch (self->field_10) {
            case 0:
            case 2:
                self->field_10 = 4;
                break;
            case 1:
            case 3:
                self->field_10 = self->field_10 - 1;
                break;
            case 4:
                self->field_10 = 1;
                break;
            }
        }
        return;
    }
    if (flags & 0x80) {
        PlaySfx(gUnknown_030012BC, 0x46, 0x100);
        if ((u32)self->field_10 <= 4) {
            switch (self->field_10) {
            case 0:
            case 2:
                self->field_10 = self->field_10 + 1;
                break;
            case 1:
            case 3:
                self->field_10 = 4;
                break;
            case 4:
                self->field_10 = 0;
                break;
            }
        }
        return;
    }
    if (flags & 0x30) {
        if (self->field_10 != 4) {
            PlaySfx(gUnknown_030012BC, 0x46, 0x100);
            self->field_10 ^= 2;
        }
    }
}

extern void sub_80236AC(void *cache, void *buf);
extern void sub_8023334(void *cache, u8 arg1);
extern void sub_8001B50(void *arg0, u16 arg1);
extern void sub_8001B30(void *arg0, u16 arg1);

/* States 1/2's input handler (the two icon slider rows, `handle` =
 * field_8c/field_90 respectively): confirm/cancel-combo either resets
 * to state 0 (if maxed out) or, if the currently-highlighted row isn't
 * already selected (sub_8002CE8), toggles it on and pulls its stats
 * into the current-selection scratch fields; cancel (bit 1) resets to
 * state 0; otherwise falls through to the shared L/R cursor mover. */
void sub_80034BC(struct pause_options_screen *self, u32 flags, void *handle)
{
    u8 buf[0x70];
    extern u8 sub_8002CE8(void *handle, s32 rowIndex);

    if (flags & 1) {
        goto confirm;
    }
    if (flags & 8) {
    confirm:
        if (self->field_10 == 4) {
            PlaySfx(gUnknown_030012BC, 0x49, 0x100);
            self->state = 0;
            self->field_10 = 0;
            return;
        }
        if (sub_8002CE8(handle, self->field_10)) {
            PlaySfx(gUnknown_030012BC, 0x48, 0x100);
            return;
        }
        PlaySfx(gUnknown_030012BC, 0x49, 0x100);
        sub_8002C14(handle, self->field_10, buf);
        sub_80236AC(gUnknown_030012C0, buf);
        sub_8023334(gUnknown_030012C0, buf[0x68]);
        sub_8001B50(gUnknown_030012BC, *(u16 *)&buf[0x6a]);
        sub_8001B30(gUnknown_030012BC, *(u16 *)&buf[0x6c]);
        sub_80048E0(self, &self->currentStats, sub_80236EC(gUnknown_030012C0));
        self->field_20 = 1;
        self->field_8 = 1;
        return;
    }
    if (flags & 2) {
        PlaySfx(gUnknown_030012BC, 0x47, 0x100);
        self->state = 0;
        self->field_10 = 0;
        return;
    }
    sub_80033E8(self, flags);
}

extern s32 sub_8003B40(struct pause_options_screen *self);
/* Matches sub_8004A64's real (void)-taking, unused-argument prototype
 * from src/graphics/settings_menu3.c - this call site passes `self`
 * anyway (the ROM's caller sets it up in r0 even though the callee
 * never reads it), so it's declared here as taking one ignored
 * argument to reproduce that dead register setup. */
extern void sub_8004A64(void *arg0);
extern s32 gUnknown_03000810;
extern s32 gUnknown_03000814;

/* State 3's input handler: polls the SIO-handshake spinner
 * (sub_8003B40, parked). Timeout/cancel -> settle back to state 0;
 * error/checksum-mismatch -> a "connection failed" message (state 4);
 * otherwise, if both sides agree on the checksummed record
 * (sub_8002B94's version nibble), accept it (state 2); if they don't,
 * inspect the remote's nibble to merge either flag 2 or flag 4 into our
 * own record and show a matching "conflict" message. */
void sub_80035C0(struct pause_options_screen *self)
{
    extern u32 sub_8002B94(void *arg0);
    extern s32 sub_8002BA4(void *arg0);
    extern void sub_8002D28(void *handle, u8 flags);
    s32 state = sub_8003B40(self);

    sub_8004A64(self);

    if (state == 3) {
        self->state = 0;
        self->field_10 = 1;
        PlaySfx(gUnknown_030012BC, 0x47, 0x100);
        return;
    }

    if (state == 2 || !sub_8002B44(self->field_90)) {
        self->state = 4;
        self->field_14 = sub_8026F38(0x2c);
        self->field_18 = sub_8026F38(0x2e);
        return;
    }

    if (sub_8002B94(self->field_8c) == sub_8002B94(self->field_90)) {
        self->state = 2;
        self->field_10 = 0;
        sub_8004860(self, self->field_90);
        return;
    }

    switch (sub_8002B94(self->field_90)) {
    case 2:
        sub_8002D28(self->field_8c, 2);
        sub_8002BA4(self->field_8c);
        self->state = 4;
        self->field_14 = gUnknown_03000810;
        break;
    case 3:
        sub_8002D28(self->field_8c, 4);
        sub_8002BA4(self->field_8c);
        self->state = 4;
        self->field_14 = gUnknown_03000814;
        break;
    default:
        self->state = 0;
        self->field_10 = 1;
        return;
    }
    self->field_18 = sub_8026F38(0x2e);
}

#if NON_MATCHING
extern u8 sub_8002CE8(void *handle, s32 rowIndex);
extern void sub_800014C(void *dst, const void *src, s32 size);
extern s32 sub_802332C(void *arg0);
extern s32 sub_8001ABC(void *arg0);
extern s32 sub_8001AC0(void *arg0);
extern s32 sub_8002BA4(void *arg0);

/* Reconstructed (semantics fully understood - see
 * docs/matching/issue-5-overlay-ui-sync.md) but NOT YET BYTE-MATCHING:
 * every technique tried elsewhere in this chunk (loop restructuring,
 * `handleAddr`-style cached-address pointers matching sub_800306C's
 * pattern, cached global addresses for gUnknown_030012C0/BC) landed
 * this function at the ROM's exact byte size but not byte-for-byte
 * content - the same unresolved gcc-2.9 scratch-register class
 * documented throughout this chunk. Real bytes stay in
 * asm/code_3_1_10_3_3698.s, wrapped `.if NON_MATCHING == 0`.
 *
 * Shared "commit or refresh row `rowIndex`" step used by states 5-9
 * below: pulls the row's stats/name/icon scratch data, feeds it through
 * `field_8c`'s pending-edit slot, and either finalises the edit
 * (sub_8002C6C, when it wasn't already selected) or just refreshes the
 * row's aggregate stats. */
void sub_8003698(struct pause_options_screen *self, s32 rowIndex)
{
    u8 buf[0xe0];
    void **handleAddr = &self->field_8c;
    u32 wasSelected;
    void **c0Addr;
    void **bcAddr;

    if (!sub_8002CE8(*handleAddr, rowIndex)) {
        sub_8002C14(*handleAddr, rowIndex, buf);
        wasSelected = 0;
    } else {
        wasSelected = 1;
    }

    c0Addr = &gUnknown_030012C0;
    sub_800014C(buf + 0x70, sub_80236EC(*c0Addr), 0x68);
    *(u8 *)(buf + 0xd8) = (u8)sub_802332C(*c0Addr);

    bcAddr = &gUnknown_030012BC;
    *(u16 *)(buf + 0xda) = (u16)sub_8001ABC(*bcAddr);
    *(u16 *)(buf + 0xdc) = (u16)sub_8001AC0(*bcAddr);

    sub_8002C40(*handleAddr, rowIndex, buf + 0x70);
    if (sub_8002BA4(*handleAddr)) {
        if (wasSelected) {
            sub_8002C6C(*handleAddr, rowIndex);
        } else {
            sub_8002C40(*handleAddr, rowIndex, buf);
        }
    } else {
        sub_80048E0(self, &self->rowStats[rowIndex], sub_80236EC(*c0Addr));
    }
}
#endif /* NON_MATCHING */
