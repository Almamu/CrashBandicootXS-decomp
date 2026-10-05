#include "core.h"
#include "actor.h"
#include "actor_self.h"

/* GitHub issue #9/#10 (0x0800B8DC-0x0800D040 cluster, see
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md): the small gap the
 * three parallel closing sessions all missed - `asm/code_3_2_17_ca04.s`
 * (ROM 0x0800CA04-0x0800CBD4, 464 bytes, 19 functions/stubs), sitting
 * directly between two already-matched neighbors from the same
 * session: `LaunchHarmfulEffectPart` (`actor_part116.c`) just before it, and
 * `CreateKnockedEnemyCtrl` (`actor_part117.c`) - which calls this file's own
 * `nullsub_14` - immediately after. Despite the address range's small
 * size the whole file turned out to be nothing but tiny single-purpose
 * accessors on this cluster's already-well-characterized `self`/`owner`
 * object shape (field table in the Phase 1 section of the doc above),
 * plus two slightly larger helpers (`GetSfxVolumeAt`'s camera-distance/
 * volume calculator, `UpdatePeriodicSpawner`'s conditional `_call_via_r4`
 * trigger) and one instance of the "flag active + bitmap-set" idiom
 * (`UpdateKnockedEnemyCtrl`) already matched as real C once before, in
 * `actor_part27c.c`'s `UpdateOneShotAnimCtrl`.
 *
 * All 19 matched as **real C**, no NAKED fallback needed anywhere in
 * this file - smaller and more resistant-shape-free than most of this
 * cluster's other files, despite several needing the project's usual
 * gcc 2.9 register-pinning toolbox (see individual comments below).
 *
 * New field offsets this file resolves/confirms on the shared
 * `self`/`owner` object shape (offsets not already in the Phase 1
 * doc's own field table):
 *
 * - `self+0x70`: the `owner` pointer itself - `AttachEnemyCtrl` is its
 *   setter (every other function in this cluster only ever *reads*
 *   `self+0x70`; this is the first confirmed writer).
 * - `self+0x4`: the "manager" pointer Phase 2's doc already
 *   identified (`StartCtrlTargetMotionYFromSet`/`StartCtrlTargetMotionXFromSet`'s own 8-byte-record
 *   array base) - `ResetEnemyCtrl` resets it to the fixed global
 *   `gEnemyCtrlMotionSet`.
 * - `self+0x84`: the per-instance mode-indexed pointer table Phase 2
 *   already identified (`SetEnemyAnimMode`/`SetEnemyState`'s own trigger
 *   table) - `SetEnemyModeTable` is its setter, `ResetEnemyCtrl` clears it.
 * - `self+0x88`: the floating-popup child pointer the Phase 1 doc's
 *   field table already names - `ResetEnemyCtrl` clears it (part of the
 *   same reset this function performs on `self+0x70`/`self+0x84`).
 * - `self+0x3c`/`0x40`/`0x44`: the sine-oscillator parameters
 *   `UpdateEnemyOscillateX`/`UpdateEnemyBob`/`UpdateEnemyOscillateY` (`actor_part116.c`)
 *   already consume (`self->0x3c` divisor, `self->0x40` phase offset,
 *   `self->0x44` amplitude) - `SetEnemyOscillator` is their setter.
 * - `self+0x48`/`0x4c`: the fields `UpdateEnemyShooter`'s (`actor_part121.c`)
 *   own `__modsi3` "close enough" gate reads - `SetEnemyShotPeriod` is
 *   their setter.
 * - `self+0x30`/`0x34`/`0x38`: the "blocking condition" pair plus
 *   "enabled" byte the Phase 1 doc's field table already names -
 *   `sub_800CAA4` is their setter.
 * - `self+0x20`/`0x24`/`0x28`/`0x2c`: the per-instance AABB trigger
 *   box `UpdateEnemyTriggerBox` (`actor_part116.c`) already builds from -
 *   `SetEnemyTriggerBox` is its full 4-corner setter, `SetPeriodicSpawnerPeriod` a
 *   2-field (position-only) partial setter.
 * - `self+0x6c`: the "second, larger-range state/anim-id byte" the
 *   Phase 1 doc's field table already names - `sub_800CAC8` is its
 *   setter.
 * - `self+0x1c`: the Y-axis homing bound `SetEnemyRangeYSpeed`/`SetEnemyRangeX`
 *   (`actor_part122.c`) already write - `sub_800CB60` is its setter,
 *   and `UpdatePeriodicSpawner` reads it (into a value it never uses - see that
 *   function's own comment).
 * - `self+0x18`: reused here as the struct-actor-shaped "table"
 *   pointer role (`DestroyPeriodicSpawner`/`CreatePeriodicSpawner`, and `UpdateKnockedEnemyCtrl`'s own
 *   `other` argument) - the same nominal offset the Phase 4 doc's
 *   `SetEnemyRangeYSpeed` uses for a Y-axis homing bound instead, on what must
 *   be a differently-shaped object at that call site (this cluster's
 *   `self`/`owner`/`other` roles are not one single reconciled struct,
 *   per the Phase 1 doc's own explicit caution - not resolved further
 *   here).
 *
 * Confirmed byte-identical to `baserom.gba` at `0x0800CA04`-`0x0800CBD4`
 * (464 bytes, all 19 functions) via the isolated cpp/agbcc/as +
 * objcopy/cmp pipeline (the only per-function differences from a direct
 * ROM slice were `bl`/literal-pool relocation sites, in every case),
 * plus a full clean `rm -rf build && make NON_MATCHING=1 report` (no
 * warnings) and `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
 * crashbandicootxs.map && make compare` (`crashbandicootxs.gba: La
 * suma coincide`). This fully consumes `asm/code_3_2_17_ca04.s`,
 * retired from `ldscript.txt`. */

/* This cluster's controller object (the class of UpdateEnemyCtrl,
 * actor_part112.c), as far as this file's accessors describe it. The
 * 0x30/0x34/0x38 triple is only ever set here - its readers (the
 * "blocking condition" checks in the docs) test the *owner's* fields at
 * the same offsets, so its own meaning is still unknown. */
struct trigger_ctrl {
    u8 unk_00[4];
    void *manager;          // 0x04 - 8-byte-record array (StartCtrlTargetMotionYFromSet/StartCtrlTargetMotionXFromSet)
    u8 unk_08[4];
    void *vtable;           // 0x0C
    u8 unk_10[0x10];
    s32 boxX;               // 0x20 - trigger box (UpdateEnemyTriggerBox)
    s32 boxY;               // 0x24
    s32 boxW;               // 0x28
    s32 boxH;               // 0x2C
    s32 unk_30;             // 0x30
    s32 unk_34;             // 0x34
    s32 unk_38;             // 0x38
    s32 oscDivisor;         // 0x3C - sine oscillator (UpdateEnemyOscillateX/UpdateEnemyBob/UpdateEnemyOscillateY)
    s32 oscPhase;           // 0x40
    s32 oscAmplitude;       // 0x44
    s32 period;             // 0x48 - UpdateEnemyShooter's gate passes once every `period` frames...
    s32 phase;              // 0x4C - ...at this offset
    u8 unk_50[0x1C];
    s32 unk_6c;             // 0x6C - state/anim id
    void *owner;            // 0x70 - the controlled object
    u8 unk_74[0x10];
    void *triggerTable;     // 0x84 - mode-indexed (SetEnemyAnimMode/SetEnemyState)
    void *popup;            // 0x88 - floating popup child
};

/* A periodic trigger actor (CreatePeriodicSpawner builds one on top of graphics.c's
 * `struct actor`): fires _call_via_r4 at its own position once every
 * `period` frames while near the camera (UpdatePeriodicSpawner). */
struct periodic_spawner {
    struct actor base;      // 0x00 - `base.table` is the method table
    s32 unk_1c;             // 0x1C - read but unused by UpdatePeriodicSpawner
    s32 period;             // 0x20
    s32 phase;              // 0x24
};

/* `self+0x70` ("owner") setter - the first confirmed writer of this
 * field anywhere in the cluster (every other function only reads it). */
void AttachEnemyCtrl(struct trigger_ctrl *self, void *owner)
{
    self->owner = owner;
}

extern struct actor *gPlayer;

/* The exact "distance-scaled ambient sound volume" calculation
 * `UpdateEnemyCtrl` state 18 (`actor_part112.c`) already documents inline
 * - `max(|x - cameraX|, |y - cameraY|)` against `gPlayer`
 * (the player/camera object), clamped to `[0x20, 0xa0]`, converted to
 * `0x100 - (clamped - 0x20) * 2`. Whether this is the literal function
 * that inline block was compiled from, or an independently-written
 * sibling with identical logic, isn't resolved here - either way it's
 * the same primitive.
 *
 * Needed [[matching_decomp_register_pinning]]: pinning `x`/`y` to
 * `r0`/`r1` and every intermediate to the ROM's own `r2`/`r3` choices
 * was enough to get gcc 2.9 to emit the ROM's own branch-free
 * sign-mask abs idiom (`mask = v >> 31; v = (v ^ mask) - mask;`)
 * without spilling to `r4` - unpinned, the compiler kept `x`'s
 * distance live across the `y` computation in a spilled `r4`, forcing
 * an unwanted `push {r4, lr}`/`pop {r4}` pair the ROM's own leaf
 * function (no `bl` calls at all) never has. The final `d < 0x20`
 * clamp also needed rephrasing as `d = (d >= 0x20) ? d : 0x20` (rather
 * than the more natural `if (d < 0x20) d = 0x20;`) to make gcc emit
 * the ROM's own `cmp r1, #0x20; bge` pair instead of canonicalizing
 * the negated branch condition into `cmp r1, #0x1f; bgt`. */
s32 GetSfxVolumeAt(s32 x, s32 y)
{
    register s32 dx asm("r0") = x;
    register s32 dy asm("r1") = y;
    register struct actor *obj asm("r3") = gPlayer;
    register s32 mask asm("r2");
    register s32 d asm("r1");

    mask = obj->x >> 8;
    dx = dx - mask;
    mask = dx >> 31;
    dx = dx ^ mask;
    mask = dx - mask;

    dy = dy - (obj->y >> 8);
    {
        register s32 mask2 asm("r0");
        mask2 = dy >> 31;
        dy = dy ^ mask2;
        dy = dy - mask2;
    }

    d = (dy >= mask) ? dy : mask;
    d = (d >= 0x20) ? d : 0x20;
    if (d > 0xa0)
        d = 0xa0;
    return 0x100 - (d - 0x20) * 2;
}

extern u8 gEnemyCtrlMotionSet[];

/* Resets `self+0x70` ("owner"), `self+0x84` (the per-instance
 * mode-indexed pointer table) and `self+0x88` (the floating-popup
 * child pointer) to null, and re-points `self+4` (the "manager"
 * pointer, per the Phase 2 doc) at the fixed `gEnemyCtrlMotionSet`
 * table - an initializer/reset for this object's own extension
 * fields, called by `CreateEnemyCtrl` below as part of its own
 * construction sequence. */
void ResetEnemyCtrl(struct trigger_ctrl *self)
{
    self->owner = NULL;
    self->triggerTable = NULL;
    self->manager = gEnemyCtrlMotionSet;
    self->popup = NULL;
}

extern u8 gEnemyCtrlVtable[];
extern void DestroyCtrl(void *self, s32 flags);
extern void InitCtrl(void *self);

/* Sets `self+0xc`'s table pointer to `gEnemyCtrlVtable` - the
 * same 93-vtable-family record `UpdateEnemyCtrl`/`HitEnemy`
 * (`actor_part112.c`) themselves live in, per the Phase 1 doc's own
 * "Bounds and vtable status" section - then tail-calls `DestroyCtrl`.
 * Same "dead store, immediately overwritten by the callee" shape
 * already flagged as a likely oddity for this exact function in the
 * Phase 1 doc's own state-9 note: `DestroyCtrl` (`actor_part17.c`)
 * unconditionally resets `self+0xc` right back to
 * `gCtrlVtable` on every call, so this function's own store
 * never survives past the call - the same harmless double-set pattern
 * already established for `DestroyStompedHopPadCtrl`/`DestroyBossCtrl`/`DestroyMegaMixCtrl`/
 * `DestroyEffectCtrl`. */
void DestroyEnemyCtrl(struct trigger_ctrl *self, s32 flags)
{
    self->vtable = gEnemyCtrlVtable;
    DestroyCtrl(self, flags);
}

/* Resets via `InitCtrl`, re-points `self+0xc` at the same
 * `gEnemyCtrlVtable` table `DestroyEnemyCtrl` above uses, then calls
 * `ResetEnemyCtrl` (clearing this object's own extension fields) and
 * returns `self` - the same "reset, re-point, hook, return self"
 * constructor shape already matched for `CreateStompedHopPadCtrl`/`DestroyStompedHopPadCtrl`/
 * `CreateKnockedEnemyCtrl`/`InitEffectCtrl`, with `ResetEnemyCtrl` playing the
 * `nullsub_N`-hook role those other constructors give a no-op. */
void *CreateEnemyCtrl(struct trigger_ctrl *self)
{
    InitCtrl(self);
    self->vtable = gEnemyCtrlVtable;
    ResetEnemyCtrl(self);
    return self;
}

/* `self+0x3c`/`0x40`/`0x44` setter - the sine-oscillator parameters
 * (divisor, phase offset, amplitude) `UpdateEnemyOscillateX`/`UpdateEnemyBob`/
 * `UpdateEnemyOscillateY` (`actor_part116.c`) already consume. */
void SetEnemyOscillator(struct trigger_ctrl *self, s32 a, s32 b, s32 c)
{
    self->oscDivisor = a;
    self->oscPhase = b;
    self->oscAmplitude = c;
}
asm(".align 2, 0");

/* `self+0x48`/`0x4c` setter - the fields `UpdateEnemyShooter`'s
 * (`actor_part121.c`) own `__modsi3` "close enough" gate reads. */
void SetEnemyShotPeriod(struct trigger_ctrl *self, s32 a, s32 b)
{
    self->period = a;
    self->phase = b;
}

/* `self+0x30`/`0x34`/`0x38` setter - the "blocking condition" pair
 * plus "enabled" byte the Phase 1 doc's field table already names. */
void sub_800CAA4(struct trigger_ctrl *self, s32 a, s32 b, s32 c)
{
    self->unk_30 = a;
    self->unk_34 = b;
    self->unk_38 = c;
}

/* `self+0x20`/`0x24`/`0x28`/`0x2c` full 4-corner setter - the
 * per-instance AABB trigger box `UpdateEnemyTriggerBox` (`actor_part116.c`)
 * already builds from (`self`'s own position offset/size, distinct
 * from `owner`'s own smaller flags-byte field layout at the same
 * nominal offsets, per that function's own doc comment). The fourth
 * argument arrives on the stack (only 3 fit in `r1`-`r3`); needed the
 * trailing `[[matching_decomp_alignment_fix]]` idiom since its own
 * 18-byte body isn't 4-byte-aligned. */
void SetEnemyTriggerBox(struct trigger_ctrl *self, s32 a, s32 b, s32 c, s32 d)
{
    self->boxX = a;
    self->boxW = c;
    self->boxY = b;
    self->boxH = d;
}
asm(".align 2, 0");

/* `self+0x84` setter - the per-instance mode-indexed pointer table
 * `SetEnemyAnimMode`/`SetEnemyState` (`actor_part113.c`/`actor_part122.c`)
 * both trigger through. */
void SetEnemyModeTable(struct trigger_ctrl *self, void *a)
{
    self->triggerTable = a;
}
asm(".align 2, 0");

/* `self+0x6c` setter - the "second, larger-range state/anim-id byte"
 * the Phase 1 doc's field table already names. */
void sub_800CAC8(struct trigger_ctrl *self, s32 a)
{
    self->unk_6c = a;
}

extern u32 gRoomFrameCount;
extern s32 __modsi3(s32 a, s32 b);
extern void _call_via_r4(void *arg0, s32 arg1, s32 arg2, s32 arg3);

/* If `self`'s own X position (`self+0`, Q8.8) is within `[0xa1, 0x18f]`
 * tiles of `gPlayer`'s (the player/camera object) own X
 * position, runs the same `__modsi3(gRoomFrameCount + a - b, a)`
 * "close enough" gate `UpdateEnemyShooter` (`actor_part121.c`) already uses
 * (here against `self+0x20`/`self+0x24`, the AABB corner fields
 * `SetEnemyTriggerBox` above sets), and on a pass fires
 * `_call_via_r4((void*)0xffff, (u16)selfX, (u16)(self->4 >> 8), 0)` -
 * the same "directional-target table trigger" primitive
 * `UpdateEnemyCtrl` state 11 and `HitEnemy` states 19-20
 * (`actor_part112.c`) already call directly. `self+0x1c` (the Y-axis
 * homing bound `sub_800CB60` below sets) is read here too but its
 * value is never used for anything - a genuine dead read the ROM's own
 * compiled output still performs (confirmed by the ROM's own `ldr r4,
 * [r4, #0x1c]` sitting right before the call with no further use of
 * `r4` after it).
 *
 * Needed [[matching_decomp_register_pinning]] in one spot: the dead
 * `self+0x1c` read had to be pinned to `r4` explicitly (the register
 * `self` itself was already using, and free again by this point) -
 * unpinned, gcc picked a spare `r3` for it instead, a harmless but
 * byte-different register choice from the ROM's own. */
void UpdatePeriodicSpawner(struct periodic_spawner *self)
{
    s32 selfX = self->base.x >> 8;
    s32 cameraX = gPlayer->x >> 8;

    if ((u32)(selfX - cameraX - 0xa1) <= 0xee) {
        s32 base = (s32)gRoomFrameCount;
        s32 period = self->period;
        s32 divCheck = __modsi3(base + period - self->phase, period);

        if (divCheck == 0) {
            void *arg0 = (void *)0xFFFF;
            u32 arg1 = ((u32)selfX << 16) >> 16;
            u32 arg2 = ((u32)self->base.y << 8) >> 16;
            register s32 dead asm("r4");

            dead = *(volatile s32 *)&self->unk_1c;
            (void)dead;
            _call_via_r4(arg0, arg1, arg2, 0);
        }
    }
}

extern u8 gEntityVtable[];
extern void OperatorDelete(void *self);

/* Sets `self+0x18`'s table pointer (the struct-actor-shaped "table"
 * field role, per this file's own banner comment) to
 * `gEntityVtable` - the same table `graphics.c`'s own
 * constructors already use - then, only if bit 0 of `flags` is set,
 * fires `OperatorDelete(self)`. */
void DestroyPeriodicSpawner(struct periodic_spawner *self, s32 flags)
{
    self->base.table = gEntityVtable;
    if (flags & 1) {
        OperatorDelete(self);
    }
}

extern struct actor *InitEntity(struct actor *self);
extern u8 gPeriodicSpawnerVtable[];

/* Calls `InitEntity(self)` (already matched, `graphics.c`) - its
 * return value discarded - then sets `self+0x18`'s table pointer to
 * `gPeriodicSpawnerVtable` and returns `self`. */
void *CreatePeriodicSpawner(struct periodic_spawner *self)
{
    InitEntity(&self->base);
    self->base.table = gPeriodicSpawnerVtable;
    return self;
}

/* `self+0x20`/`0x24` partial (position-only) setter - the same AABB
 * trigger-box fields `SetEnemyTriggerBox` above sets all four corners of. */
void SetPeriodicSpawnerPeriod(struct periodic_spawner *self, s32 a, s32 b)
{
    self->period = a;
    self->phase = b;
}
asm(".align 2, 0");

/* `self+0x1c` setter - the Y-axis homing bound `SetEnemyRangeYSpeed`/
 * `SetEnemyRangeX` (`actor_part122.c`) already write, and the field
 * `UpdatePeriodicSpawner` above reads (but never uses) via its own dead
 * `self+0x1c` load. */
void sub_800CB60(struct periodic_spawner *self, s32 a)
{
    self->unk_1c = a;
}

extern void *gEntityFlags;
extern void *_call_via_r1(void *addr, void *fn);

/* `self` (the first argument) is never read - only `other` matters.
 * Reads `other+0x18`'s own struct-actor-shaped table pointer, fires a
 * `_call_via_r1` hit-probe against its `+0x28`/`+0x2c` `{s16 offset,
 * void *fn}` pair (the exact same convention `src/system/game_loop8.c`'s
 * `UpdateRoomFrame` and `actor_part123.c`'s `UpdateEffectCtrl` both already
 * read from their own `table+0x28`/`+0x2c`), and - only when that
 * probe reports *no* hit - runs the "flag active + bitmap-set" idiom
 * on `other` (`other+0xc` |= bit 0; unless `other+8`'s id sentinel-
 * checks as `0xffff`, also sets bit `other+8 & 0x1f` of word
 * `other+8 >> 5` in the `gEntityFlags+0x108` bitmap) - the exact
 * idiom `actor_part27c.c`'s `UpdateOneShotAnimCtrl` already matches as real C.
 *
 * Needed the same `[[matching_decomp_register_pinning]]` treatment
 * `UpdateOneShotAnimCtrl` itself documents needing: `other` pinned to `r4`
 * (matching the ROM's own register choice, freed up again by the time
 * the bitmap-set idiom's own `0x108`-offset computation reuses it),
 * plus the same chain of `register ... asm("rN")` pins and the
 * `volatile` reload of `other+8` that function's own doc comment
 * already explains is needed to stop this compiler CSE-ing away the
 * ROM's own seemingly-redundant second `ldrh` and folding the
 * shift-setup pair into a single instruction. */
struct probe_vtable {
    u8 unk_00[0x28];
    struct actor_method m28;    // 0x28 - hit probe
};

void UpdateKnockedEnemyCtrl(void *selfArg, void *otherArg)
{
    register u8 *other asm("r4") = otherArg;
    struct probe_vtable *table = ((struct actor *)other)->table;
    s16 offset = table->m28.thisOffset;
    void *addr = other + offset;
    void *fn = table->m28.fn;

    (void)selfArg;

    if ((u8)(s32)_call_via_r1(addr, fn) == 0) {
        register s32 one asm("r0") = 1;
        register u8 flags asm("r1") = other[0xc];

        one |= flags;
        other[0xc] = one;

        {
            register s32 sentinel asm("r0") = 0xFFFF;
            register u16 val asm("r2") = *(u16 *)(other + 8);

            if (val != sentinel) {
                register u16 val2 asm("r3") = *(u16 volatile *)(other + 8);
                register u8 *base asm("r2") = gEntityFlags;
                register s32 idx asm("r0");
                s32 idxOffset;
                s32 *bitmap;
                register s32 bit asm("r0");

                asm("add %0, %1, #0\n\tasr %0, %0, #5" : "=r" (idx) : "r" (val2));
                idxOffset = idx * 4;
                bitmap = (s32 *)(base + 0x108);
                bitmap = (s32 *)((u8 *)bitmap + idxOffset);
                bit = val2 - (idx << 5);
                *bitmap |= 1 << bit;
            }
        }
    }
}

/* Genuine empty stub (`bx lr`) - `CreateKnockedEnemyCtrl`'s (`actor_part117.c`)
 * own tail-call hook, per that function's own doc comment. */
void nullsub_14(void *self)
{
}
asm(".align 2, 0");

extern u8 gKnockedEnemyCtrlVtable[];

/* Same "double-set" shape as `DestroyEnemyCtrl` above: sets `self+0xc`'s
 * table pointer to `gKnockedEnemyCtrlVtable` - the same fixed anchor
 * table `CreateKnockedEnemyCtrl` (`actor_part117.c`) itself re-points `self+0xc`
 * at - then tail-calls `DestroyCtrl`, which promptly resets `self+0xc`
 * right back to `gCtrlVtable` regardless (same harmless dead
 * store as `DestroyEnemyCtrl`). */
void DestroyKnockedEnemyCtrl(struct trigger_ctrl *self, s32 flags)
{
    self->vtable = gKnockedEnemyCtrlVtable;
    DestroyCtrl(self, flags);
}
