#include "core.h"
#include "actor.h"

extern void sub_8022208(void);
extern void sub_8024198(void);
extern void *sub_8026EDC(s32 size);
extern struct dual_array_manager *sub_8008EE4(struct dual_array_manager *manager, s32 count);
extern struct pool_manager *sub_8008F20(struct pool_manager *manager, s32 count);
extern void *sub_80268AC(void);
extern void *sub_800B3F0();
extern void sub_8007398(struct actor *self, s32 arg1, s32 arg2);
extern void sub_8026ED0(void *self);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern void *sub_801588C(void *selfArg);
extern void sub_800B69C(void *selfArg, s32 val);
extern void *sub_80174EC(void *arg0);
extern void *sub_8017A00(void *arg0);
extern void sub_80087C0(void *part);
extern void sub_80087B4(void *part);
extern void sub_800872C(void *part, u8 val);
extern s32 sub_8023A1C(void *self);
extern void sub_802680C(void *self, s32 flag);
extern void sub_8008EB4(struct dual_array_manager *manager, s32 flags);
extern void sub_8009B9C(struct pool_manager *manager, s32 flags);
extern void sub_80221F0(void);

extern struct dual_array_manager *gUnknown_030012E8;
extern struct dual_array_manager *gUnknown_030012EC;
extern struct pool_manager *gUnknown_0300130C;
extern struct dual_array_manager *gUnknown_030012F0;
extern struct dual_array_manager *gUnknown_030012F8;
extern struct dual_array_manager *gUnknown_030012F4;
extern void *gUnknown_030012D4;
extern void *gUnknown_03001308;
extern void *gUnknown_030012D8;
extern void *gUnknown_03001310;
extern void ***gUnknown_030012D0;
extern u8 gStaticData_0816B92C[];
extern u8 gStaticData_0816B934[];
extern u8 gStaticData_0816B93C[];

/* Level-start dispatcher, called once from `UpdateGameFrame` when the
 * level object's own `+0xdc->+8` state field is `2` (see
 * `asm/code_3_2_17_225a0.s`). Allocates the whole per-level widget set
 * (ring-buffer/pool object families already matched in
 * `actor_part11.c`/`actor_part12.c`: `gUnknown_030012E8/EC/F0/F8/F4` are
 * `dual_array_manager`s, `gUnknown_0300130C` a `pool_manager`), the
 * player actor itself (`gUnknown_030012D8`, `sub_800B3F0`), and the
 * text-box singleton (`gUnknown_03001308`, `sub_80268AC`). Dispatches on
 * the level-state record's (`self->0x18`) own `+8` "widget kind" field
 * to construct one of three HUD counter/ring-buffer widgets
 * (`gStaticData_0816B92C`/`0816B934`/`0816B93C`, still-uncharacterized
 * per-widget action tables), then unconditionally hands off to
 * `sub_8023A1C` and tears the per-frame update queues back down before
 * returning its status code. */
s32 sub_802375C(void *selfArg)
{
    /* `self` is pinned to r8 for the whole function, matching the ROM:
     * it has to survive dozens of `bl`s while r4-r7 are already busy
     * with other live locals, so this compiler (like the ROM) needs a
     * `mov` through a low register before every field access - each
     * such access below is its own small register-pinned block for
     * that reason. */
    register u8 *self asm("r8") = selfArg;
    void **d8;
    s32 mode;
    s32 result;

    sub_8022208();
    sub_8024198();

    {
        struct dual_array_manager **slot = &gUnknown_030012E8;
        *slot = sub_8008EE4(sub_8026EDC(0x14), 0x20);
    }
    {
        struct dual_array_manager **slot = &gUnknown_030012EC;
        *slot = sub_8008EE4(sub_8026EDC(0x14), 0xc0);
    }
    {
        struct pool_manager **slot = &gUnknown_0300130C;
        *slot = sub_8008F20(sub_8026EDC(0x818), 0xc0);
    }
    {
        struct dual_array_manager **slot = &gUnknown_030012F0;
        *slot = sub_8008EE4(sub_8026EDC(0x14), 0x80);
    }
    {
        struct dual_array_manager **slot = &gUnknown_030012F8;
        *slot = sub_8008EE4(sub_8026EDC(0x14), 0x40);
    }
    {
        struct dual_array_manager **slot = &gUnknown_030012F4;
        *slot = sub_8008EE4(sub_8026EDC(0x14), 0x40);
    }
    {
        void **slot = &gUnknown_030012D4;
        *slot = sub_8026EDC(0x18);
    }

    gUnknown_03001308 = sub_80268AC();

    d8 = &gUnknown_030012D8;
    *d8 = sub_800B3F0(sub_8026EDC(0x350), 0xffff, 0, 0, 0);
    {
        u8 *p = self;
        sub_8007398((struct actor *)*d8, *(s32 *)(p + 0x10), *(s32 *)(p + 0x14));
    }

    /* Register-pinned (rather than a plain `*p |= 0x10`) so the mask
     * value is loaded before the pointer's current byte, matching the
     * ROM's own operand-evaluation order for this store - a plain
     * compound assignment here evaluates the load first instead. */
    {
        register u8 *p asm("r1") = (u8 *)*d8;
        register u8 val asm("r0") = 0x10;
        register u8 cur asm("r3") = p[0xc];
        register u8 result asm("r0") = val | cur;
        p[0xc] = result;
    }
    {
        register u8 *p asm("r2") = (u8 *)*d8 + 0x28;
        register u32 one asm("r1") = 1;
        register u8 *sp asm("r4") = self;
        register u8 rawbit asm("r4") = sp[0x1c];
        register u32 bit asm("r1") = (one & rawbit) << 4;
        /* Register-pinned negative-constant mask (`-0x11`, not `~0x10`)
         * so this compiler emits the ROM's own `movs r0, #0x11 / rsbs
         * r0, r0, #0` runtime mask computation instead of
         * constant-folding it to a single immediate load - the
         * "negative-constant bit-clear idiom" documented in
         * docs/matching.md (see `sub_800A70C` in actor_part14.c for the
         * established `register ... = -N` shape this mirrors). */
        register s32 mask asm("r0") = -0x11;
        register u8 cur asm("r3") = *p;
        register s32 result asm("r0") = (mask & cur) | bit;
        *p = result;
    }

    /* The ROM re-derives `self` from `r8` into `r4` again here (a
     * redundant `mov r4, r8` this compiler's own value tracking would
     * otherwise elide, since r4 still holds that exact value from the
     * block above) - the barrier below forces the reload to keep the
     * instruction count matching. */
    asm volatile("" ::: "r4");
    {
        register u8 *p asm("r4") = self;
        mode = *(s32 *)(*(u8 **)(p + 0x18) + 8);
    }

    switch (mode) {
    case 0: {
        u8 *widget = sub_801588C(sub_8026EDC(0x38));

        sub_800B69C(widget, (s32)gStaticData_0816B92C);

        *((u8 *)*d8 + 0x88) = mode;
        {
            void *val = **gUnknown_030012D0;
            u8 *pl = *d8;
            u8 *w1c;
            s32 off;

            *(void **)(pl + 0x20) = val;
            *(void **)(pl + 0x44) = widget;

            w1c = *(u8 **)(widget + 0xc);
            off = *(s16 *)(w1c + 0x18);
            widget += off;
            sub_803AD80(widget, pl, *(void **)(w1c + 0x1c));
        }
        break;
    }
    case 1: {
        void *w;

        {
            void **slot = &gUnknown_03001310;
            *slot = sub_80174EC(sub_8026EDC(0x30));
        }
        sub_800B69C(gUnknown_03001310, (s32)gStaticData_0816B934);

        *((u8 *)*d8 + 0x88) = mode;
        {
            void *val = (u8 *)**gUnknown_030012D0 + 0xc;
            u8 *pl = *d8;
            *(void **)(pl + 0x20) = val;
            {
                u8 v = 0x1f;
                pl[0x2d] = v;
            }
            sub_80087C0(pl);
            sub_80087B4(pl);
            sub_800872C(pl, 0);
        }
        {
            u8 *pl = *d8;
            u8 *w1c;
            s32 off;

            w = gUnknown_03001310;
            *(void **)(pl + 0x44) = w;
            w1c = *(u8 **)((u8 *)w + 0xc);
            off = *(s16 *)(w1c + 0x18);
            w = (u8 *)w + off;
            sub_803AD80(w, pl, *(void **)(w1c + 0x1c));
        }
        break;
    }
    case 2: {
        u8 *widget = sub_8017A00(sub_8026EDC(0x28));

        sub_800B69C(widget, (s32)gStaticData_0816B93C);

        {
            u8 *pl = *d8;
            u8 v = 3;
            pl[0x88] = v;
        }
        {
            void *val = (u8 *)**gUnknown_030012D0 + 0x18;
            u8 *pl = *d8;
            u8 *w1c;
            s32 off;

            *(void **)(pl + 0x20) = val;
            *(void **)(pl + 0x44) = widget;

            w1c = *(u8 **)(widget + 0xc);
            off = *(s16 *)(w1c + 0x18);
            widget += off;
            sub_803AD80(widget, pl, *(void **)(w1c + 0x1c));
        }
        break;
    }
    }

    result = sub_8023A1C(self);

    if (gUnknown_03001308 != NULL) {
        sub_802680C(gUnknown_03001308, 3);
    }
    sub_8026ED0(gUnknown_030012D4);

    if (gUnknown_030012D8 != NULL) {
        u8 *p = *(u8 **)((u8 *)gUnknown_030012D8 + 0x18) + 0x50;
        s32 off = *(s16 *)p;

        sub_803AD80((u8 *)gUnknown_030012D8 + off, (void *)3, *(void **)(p + 4));
    }

    if (gUnknown_030012F4 != NULL) {
        sub_8008EB4(gUnknown_030012F4, 3);
    }
    if (gUnknown_030012F8 != NULL) {
        sub_8008EB4(gUnknown_030012F8, 3);
    }
    if (gUnknown_030012F0 != NULL) {
        sub_8008EB4(gUnknown_030012F0, 3);
    }
    if (gUnknown_0300130C != NULL) {
        sub_8009B9C(gUnknown_0300130C, 3);
    }
    if (gUnknown_030012EC != NULL) {
        sub_8008EB4(gUnknown_030012EC, 3);
    }
    if (gUnknown_030012E8 != NULL) {
        sub_8008EB4(gUnknown_030012E8, 3);
    }

    sub_80221F0();

    return result;
}
