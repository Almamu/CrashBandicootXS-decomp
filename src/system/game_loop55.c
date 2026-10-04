#include "core.h"
#include "gba/io_reg.h"
#include "gba/dma_macros.h"

/* UpdateGameFrame - the main per-frame game-loop driver at the head of
 * the UpdateGameFrame-MainLoop cluster (GitHub issue #34,
 * docs/matching.md). Called once per frame from `MainLoop`
 * (src/system/main_loop.c) with `self` = `gUnknown_030012C0`, the
 * central per-level state object every other function in this
 * cluster (`sub_8022BF0`/`sub_8022CA0`, game_loop.c; the
 * `self+0x80`-`0xc4`/`+2` accessor family, game_loop2.c) also shares.
 *
 * Shape: a level-load loop (`LoadLevelGraphics` / `sub_8035E14`, the
 * map screen `sub_80354BC` on result 2), then the level loop. Each pass
 * restores the per-attempt block from `snap14C` (repeating while
 * `sub_801BAF0` asks to), runs the attempt loop (`sub_802375C` or
 * `InitActorCategory` per frame, the two bitmap ping-pongs through
 * `gUnknown_030012B4`, the checkpoint/respawn handling), and after the
 * attempt dispatches on the level number (20-24 are the special
 * levels) and records a time-trial best time.
 *
 * Real C under old_agbcc (docs/matching/big-naked-retry.md):
 * - The level loop and the attempt loop are real `for (;;)` loops.
 *   gcc rolls each one's first exit test to the end, which gives the
 *   ROM's `b` into the middle of the loop. The restore step is a `goto`
 *   loop, so it is not rotated and keeps using the hoisted `&level`.
 * - `bitmap` is assigned right before the attempt loop, which gives
 *   the ROM's `mov sb, r4` copy of `&gUnknown_030012B4`.
 * - The best-time "unset" test reads the record as a halfword
 *   (`raw & 0xfff8`), as the ROM does, while the compare and the store
 *   go through the 13-bit field.
 * - `self->unk_1b8 = self->unk_1bc = 0` computes the 0x1b8 address
 *   first, and the `sub_80231EC` argument starts at 2 and takes the
 *   tier only when it is <= 1.
 * - `sub_8027138` returns a typed pointer, so the store into
 *   `gUnknown_03001318` loads the global's address before the call.
 */
/* The per-level state object (`gUnknown_030012C0`) as UpdateGameFrame
 * uses it. The first 0x68 bytes are the per-attempt block that the
 * frame loop snapshots into `snapE4`/`snap14C` and restores from. */
union level_best_time
{
    struct
    {
        u32 flag:1;
        u32 unk_1:2;
        u32 time:13;    // tenths of a second, capped at 0x1fff
        u32 unk_16:16;
    } f;
    u16 raw;
};

struct level_category
{
    u8 unk_00[8];
    s32 kind;           // 0x08 - 0-2 plain level, 3 actor-category level
    u8 unk_0c[4];
    u16 category;       // 0x10
};

struct level_state
{
    u8 attempt[0x68];               // 0x000
    s32 unk_68;                     // 0x068
    u8 unk_6c[0x74 - 0x6c];
    s32 unk_74;                     // 0x074
    s32 bestTier;                   // 0x078
    u8 unk_7c[0x8c - 0x7c];
    u8 timeTrial;                   // 0x08c
    u8 unk_8d[3];
    s32 minutes;                    // 0x090
    s32 seconds;                    // 0x094
    s32 tenths;                     // 0x098
    u8 unk_9c[0xac - 0x9c];
    s32 unk_ac;                     // 0x0ac
    s32 unk_b0;                     // 0x0b0
    s32 unk_b4;                     // 0x0b4
    s32 unk_b8;                     // 0x0b8
    s32 unk_bc;                     // 0x0bc
    u8 unk_c0[4];
    s32 level;                      // 0x0c4 - also the head of the progress record
    s32 unk_c8;                     // 0x0c8
    s32 unk_cc;                     // 0x0cc
    u8 unk_d0;                      // 0x0d0
    u8 unk_d1[0xdc - 0xd1];
    struct level_category *cat;     // 0x0dc
    u8 unk_e0;                      // 0x0e0
    u8 unk_e1[3];
    u8 snapE4[0x68];                // 0x0e4
    u8 snap14C[0x68];               // 0x14c
    void *savedBitmap;              // 0x1b4
    s32 unk_1b8;                    // 0x1b8
    s32 unk_1bc;                    // 0x1bc
};

extern void *gUnknown_030012B4;
extern void *gUnknown_030012B8;
extern void *gUnknown_030012BC;
extern struct level_state *gUnknown_030012C0;
extern struct level_state *gUnknown_030012C4;
extern void *gUnknown_03001318;
extern s32 gUnknown_0300082C;

extern void sub_80231E4(struct level_state *self);
extern void sub_80231DC(struct level_state *self);
extern void sub_80231D4(struct level_state *self);
extern void sub_8023120(struct level_state *self, s32 n);
extern void sub_8023118(struct level_state *self, s32 n);
extern void sub_8023110(struct level_state *self, s32 n);
extern void *sub_800014C(void *dest, void *src, s32 size);
extern void *sub_8026EDC(u32 size);
extern void *LoadLevelGraphics(void *mem);
extern s32 sub_8035E14(void *gfx);
extern void sub_8036154(void *gfx, u32 flag);
extern void sub_80354BC(void);
extern void sub_8004D4C(void);
extern s32 sub_800300C(s32 a, s32 b);
extern void sub_8004D20(void);
extern void sub_8022468(struct level_state *self, s32 screen);
extern u8 sub_80231BC(struct level_state *self);
extern u8 sub_80231CC(struct level_state *self);
extern u8 sub_80231B4(struct level_state *self);
extern u8 sub_80231C4(struct level_state *self);
extern void sub_801D41C(void);
extern void sub_8023190(struct level_state *self);
extern void sub_80231A8(struct level_state *self);
extern void sub_8023184(struct level_state *self);
extern void sub_802319C(struct level_state *self);
extern void sub_80067D4(void);
extern void sub_80067C4(void);
extern void sub_80067B4(void);
extern void sub_80067A4(void);
extern s32 sub_800697C(struct level_state *self);
extern union level_best_time *sub_8023404(struct level_state *self);
extern s32 sub_801BAF0(s32 *progress);
extern void sub_80232D8(struct level_state *self);
extern s32 sub_8024278(s32 level);
extern struct hud_stat_widget *sub_8027138(void *mem);
extern void sub_8028568(void *cache, s32 arg1);
extern void sub_80232A8(struct level_state *self);
extern void sub_80232C0(struct level_state *self);
extern void sub_8023280(struct level_state *self);
extern void sub_8023298(struct level_state *self);
extern void sub_80232D0(struct level_state *self);
extern void sub_8022CA0(struct level_state *self, u8 arg1);
extern void sub_8023304(struct level_state *self);
extern void sub_8006EA8(void *cache);
extern void sub_802732C(void *cache, s32 arg1);
extern void sub_8023318(struct level_state *self, s32 arg1);
extern void sub_8024498(s32 *progress);
extern s32 mem_free_bytes(s32 arg0);
extern s32 sub_802375C(s32 *progress);
extern s32 InitActorCategory(u16 category);
extern s32 sub_8029730(void);
extern void sub_8023140(struct level_state *self, s32 arg1);
extern void ResetAmbientSfx(void *arg0);
extern void sub_80231EC(struct level_state *self, s32 tier);
extern u8 sub_8024404(s32 *progress);
extern u8 sub_80243E0(s32 *progress);
extern u8 sub_80244F0(s32 *progress);
extern u8 sub_802455C(s32 *progress);
extern void sub_8024540(s32 *progress);
extern void sub_8024524(s32 *progress);
extern u8 sub_80232B8(struct level_state *self);
extern u8 sub_8023290(struct level_state *self);
extern void sub_8025A44(void *bitmap, s32 arg1);
extern void *sub_8025A5C(void *mem);
extern void sub_8022BF0(struct level_state *self, u8 arg1);
extern void sub_80235E4(struct level_state *self, u8 arg1);
extern s32 sub_803AFEC(struct level_state *self);
extern void sub_8023548(struct level_state *self);
extern s32 sub_802325C(struct level_state *self);
extern s32 sub_8023414(struct level_state *self);
extern s32 sub_8024464(struct level_category *cat);
extern void sub_8028574(void *cache, s32 arg1);
extern u8 sub_8034CB0(void);

void UpdateGameFrame(struct level_state *self)
{
    vu16 zero;
    s32 best;
    s32 status;
    void **bitmap;

    self->unk_68 = 0;
    sub_80231E4(self);
    sub_80231DC(self);
    sub_80231D4(self);
    sub_8023120(self, 5);
    sub_8023118(self, 5);
    sub_8023110(self, 5);
    self->level = 0;
    self->unk_e0 = 0;
    {
        struct dma_regs *dma;

        zero = 0;
        dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
        dma->src = (u32)&zero;
        dma->dst = (u32)self;
        dma->cnt = 0x81000034;
        dma->cnt;
    }
    sub_800014C(self->snap14C, self, 0x68);
    gUnknown_030012C4 = self;
    self->bestTier = 0;
    {
        void *gfx;
        s32 result;

    load:
        gfx = LoadLevelGraphics(sub_8026EDC(0x220));
        result = sub_8035E14(gfx);
        if (gfx != NULL)
            sub_8036154(gfx, 3);
        if (result == 2)
        {
            sub_80354BC();
            goto load;
        }
        if (result != 0)
        {
            sub_8004D4C();
            sub_800300C(1, 0);
            sub_8004D20();
        }
        else
        {
            sub_8022468(gUnknown_030012C0, 2);
        }
    }
    for (;;)
    {
        {
            u8 quit;

        restore:
            {
                s32 level = self->level;

                if (level > 23)
                    level = 23;
                self->level = level;
            }
            quit = sub_801BAF0(&self->level);
            sub_800014C(self, self->snap14C, 0x68);
            sub_800014C(self->snapE4, self, 0x68);
            if (quit)
            {
                sub_8004D4C();
                quit = sub_800300C(0, 0);
                sub_8004D20();
                if (quit)
                    self->unk_e0 = 0;
                goto restore;
            }
        }
        sub_80232D8(self);

    start:
        self->unk_c8 = 0;
        self->unk_cc = 0;
        self->unk_e0 = 0;
        status = 1;
        self->unk_bc = sub_8024278(self->level);
        gUnknown_03001318 = sub_8027138(sub_8026EDC(0x68));
        sub_8028568(gUnknown_03001318, self->unk_bc);
        sub_80232A8(self);
        sub_80232C0(self);
        sub_8023280(self);
        sub_8023298(self);
        sub_80231D4(self);
        sub_80232D0(self);
        self->unk_ac = 0;
        *(s32 *)gUnknown_030012B4 = 0;
        best = self->bestTier;
        sub_8022CA0(self, 0);
        sub_8023304(self);
        bitmap = &gUnknown_030012B4;
        for (;;)
        {
            self->unk_1b8 = self->unk_1bc = 0;
            if (sub_80232B8(self) || sub_8023290(self))
            {
                self->savedBitmap = *bitmap;
                *bitmap = sub_8025A5C(sub_8026EDC(0x408));
                if (sub_80232B8(self))
                {
                    self->unk_b0 = sub_802325C(self);
                    self->unk_b8 = sub_803AFEC(self);
                    self->unk_b4 = sub_8023414(self);
                    sub_80231DC(self);
                    self->unk_74 = 0;
                    sub_80231D4(self);
                    sub_8024540(&self->level);
                    sub_8028568(gUnknown_03001318, sub_8024464(self->cat));
                }
                else
                {
                    self->unk_b4 = sub_8023414(self);
                    sub_80231D4(self);
                    sub_8024524(&self->level);
                    sub_8028568(gUnknown_03001318, sub_8024464(self->cat));
                }
                sub_8023304(self);
            }
            else if (!sub_802455C(&self->level))
            {
                break;
            }
            sub_8006EA8(gUnknown_030012B8);
            sub_802732C(gUnknown_03001318, 0);
            gUnknown_0300082C = 0;
            sub_8023318(self, 0);
            sub_8024498(&self->level);
            mem_free_bytes(0xC0000000);
            switch (self->cat->kind)
            {
            case 0:
            case 1:
            case 2:
                status = sub_802375C(&self->level);
                break;
            case 3:
                status = InitActorCategory(self->cat->category);
                if (status == 0)
                {
                    sub_8023140(self, sub_8029730());
                    sub_8022CA0(self, 0);
                }
                break;
            }
            ResetAmbientSfx(gUnknown_030012BC);
            {
                s32 tier = self->bestTier;
                s32 arg = 2;

                if (tier <= 1)
                    arg = tier;
                sub_80231EC(self, arg);
            }
            mem_free_bytes(0xC0000000);
            if (sub_8024404(&self->level) && sub_80232B8(self))
            {
                if (gUnknown_030012B4 != NULL)
                    sub_8025A44(gUnknown_030012B4, 3);
                gUnknown_030012B4 = self->savedBitmap;
                sub_8022BF0(self, status == 0);
            }
            if (sub_80243E0(&self->level) && sub_8023290(self))
            {
                if (gUnknown_030012B4 != NULL)
                    sub_8025A44(gUnknown_030012B4, 3);
                gUnknown_030012B4 = self->savedBitmap;
                sub_80235E4(self, status == 0);
            }
            if (status == 2)
                break;
            if (status == 1 && sub_803AFEC(self) < 0)
                break;
            if (self->timeTrial && status == 1)
            {
                self->unk_ac = 0;
                self->unk_d0 = 0;
                self->unk_c8 = 0;
                self->unk_cc = 0;
                sub_8023304(self);
                sub_80232D8(self);
            }
            if (status == 0)
            {
                u8 done;

                if (!sub_8024404(&self->level) && !sub_80232B8(self) && !sub_80243E0(&self->level)
                    && !(done = sub_8023290(self)))
                {
                    if (!sub_80244F0(&self->level))
                        break;
                    sub_8023304(self);
                    *(s32 *)*bitmap = done;
                }
            }
            else
            {
                sub_8023548(self);
            }
        }
        if (gUnknown_03001318 != NULL)
            sub_8028574(gUnknown_03001318, 3);
        if (sub_803AFEC(self) < 0)
        {
            if (sub_8034CB0())
                sub_80231E4(self);
            else
                break;
        }

        if (status == 2)
        {
            s32 tier = best;

            if (tier > self->bestTier)
                tier = self->bestTier;
            self->bestTier = tier;
        }
        if (status == 0)
        {
            switch (self->level)
            {
            case 20:
                if (!sub_80231BC(self))
                {
                    sub_801D41C();
                    sub_8023190(self);
                    sub_80067D4();
                    sub_8022468(self, 4);
                }
                break;
            case 21:
                if (!sub_80231CC(self))
                {
                    sub_801D41C();
                    sub_80231A8(self);
                    sub_80067C4();
                    sub_8022468(self, 5);
                }
                break;
            case 22:
                if (!sub_80231B4(self))
                {
                    sub_801D41C();
                    sub_8023184(self);
                    sub_80067B4();
                    sub_8022468(self, 6);
                }
                break;
            case 23:
                if (!sub_80231C4(self))
                {
                    sub_801D41C();
                    sub_802319C(self);
                    sub_80067A4();
                }
                if (sub_800697C(self) > 99)
                {
                    sub_8022468(self, 8);
                    self->level++;
                    self->unk_c8 = 0;
                    goto start;
                }
                sub_8022468(self, 10);
                sub_80354BC();
                break;
            case 24:
                sub_8022468(self, 9);
                sub_80354BC();
                break;
            default:
                if (self->cat->kind == 3)
                    sub_8023404(self)->f.flag = 1;
                break;
            }
            if (self->timeTrial)
            {
                u32 t = self->tenths + self->seconds * 10 + self->minutes * 600;

                if (t > 0x1fff)
                    t = 0x1fff;
                if (t < sub_8023404(self)->f.time || (sub_8023404(self)->raw & 0xfff8) == 0)
                    sub_8023404(self)->f.time = t;
            }
            sub_800014C(self->snap14C, self, 0x68);
        }
    }
}
