#include "enemy_ctrl.hpp"
#include "spawners.hpp"
#include "player.hpp"

extern "C" {
#include "match.h"
#include <libgcc.h>
#include "level.h"
#include "globals.h"
#include "math_util.h"
#include "player.h"
}

/* The enemy controller's small methods (include/enemy_ctrl.hpp), ROM
 * 0x0800C8AC-0x0800CACC: EnemyCtrl's mode triggers, oscillators,
 * constructor, destructor and setters. The periodic spawner and the
 * knocked controller follow in periodic_spawner.cpp and
 * knocked_enemy_ctrl.cpp. EnemyCtrl's Update and HandleEvent are in
 * enemy_ctrl_update.cpp, its per-state updaters in enemy_attack.cpp,
 * enemy_motion.cpp, enemy_patrol.cpp and enemy_shooter.cpp. */

/* Sets the motion mode: starts the motion set's Y record `mode` on the
 * target, through Ctrl's method called directly (not through the
 * vtable). */
void EnemyCtrl::SetMotionY(s32 mode)
{
    modeA = mode;
    Ctrl::StartTargetMotionYFromSet(target, mode);
}

/* The same with the X record. */
void EnemyCtrl::SetMotionX(s32 mode)
{
    modeB = mode;
    Ctrl::StartTargetMotionXFromSet(target, mode);
}

/* Sets the animation mode: plays `anims[mode]` on the target, through
 * the SetTargetAnim slot. */
void EnemyCtrl::SetAnimMode(s32 m)
{
    mode = m;
    SetTargetAnim(target, anims[m]);
}

/* The three oscillators move the target along a sine wave
 * (gSineTable) around baseX/baseY, `amplitude` high. The phase is
 * gRoomFrameCount scaled by `period` (UpdateOscillateX/Y) or at half
 * rate (UpdateBob), offset by `phase`.
 *
 * Their pins are about register allocation, not the C++: the products
 * and the -0x100 phase bias come out in the ROM's registers only with
 * them (issue #9-#11 NAKED retry, issue #10 retry). The phase bias goes
 * through Wave's parameter to keep the ROM's `phase + 0xFFFFFF00`
 * literal instead of a folded `+ 0x100`.
 *
 * #662 round 2: in UpdateOscillateX, `baseX` read into a local before
 * the target gives the ROM's registers, but loads it before the target
 * (agbcp has no scheduling pass to reorder them); reading the target
 * first gives other registers. The permuter on the C++ reached the ROM
 * only with `do { } while (0)` wrappers or `x++; x--;` no-ops. Locals
 * read before UpdateOscillateY's division (target, table, phase,
 * amplitude, baseY, in any combination) don't give its unused r8.
 *
 * #662 round 3, from the RTL dumps (each function is one basic block, so
 * local-alloc decides everything):
 * - UpdateOscillateX: mulsi3's output is earlyclobber, so the product is
 *   never tied to an input; it gets the first free register when its
 *   quantity is allocated. The target load and the product rank equal
 *   (2 references over 4 insns) and the tie goes to the older quantity,
 *   the product, which takes the dying w's r1. The ROM's `mov r2, r1;
 *   mul r2, r0` needs the target (r1) and baseX (r0) allocated first,
 *   i.e. the target ranked above the product, which only an extra
 *   reference or a shorter life (a different load order) gives.
 * - UpdateBob: the -0x100 is a reload (the add can't take it as an
 *   immediate), and reload takes the first unused call-saved register,
 *   r5, in the plain function. The ROM's r6 and its saved-but-unused r5
 *   mean a pseudo sat in r5 at that insn and its code was gone by the
 *   end (e.g. a copy that reload_cse made redundant and flow2 deleted);
 *   no natural spelling of the body creates one. Holding `this` in r5
 *   doesn't either (r0 doesn't keep `this` long enough for reload_cse).
 * - UpdateOscillateY: local-alloc ranks the table's quantity above the
 *   target's, so they get r5 and r6 the other way round; the saved r8
 *   is again a register nothing in the final code uses.
 * No -f flag, alone or in pairs, and no change of the fields' or Wave's
 * types (u32 fields, an s32 or int Wave, a macro) moves any of them. */
static inline s16 Wave(const s16 *table, s32 t, s32 phase)
{
    return table[(t - phase) & 0xff];
}

/* The product goes into a fresh `v` pinned to r2 (the ROM's `mov r2,
 * r1; mul r2, r0`), which leaves r1 for the target. */
void EnemyCtrl::UpdateOscillateX()
{
    const s16 *table = gSineTable;
    s32 t = __udivsi3(INT_TO_Q8(gRoomFrameCount), period);
    MATCH_HOLD_REG(s32, v, r2);
    s32 w;
    MovingSprite *part;

    w = Wave(table, t, phase - 0x100);
    v = w * amplitude;
    part = target;
    part->x = baseX + v;
}

/* UpdateBob and UpdateOscillateY: the ROM saves a callee-saved register
 * neither body uses (r5 in UpdateBob, r8 in UpdateOscillateY), which an
 * empty asm clobbering it reproduces without emitting code. In UpdateBob
 * the target is pinned to r3 and the -0x100 bias created in r6 through a
 * constant-init asm, which keeps it from being folded and loaded early;
 * in UpdateOscillateY the table is pinned to r6, which puts the target
 * in r5. */
void EnemyCtrl::UpdateBob()
{
    MATCH_HOLD_REG(MovingSprite *, part, r3) = target;
    const s16 *table = gSineTable;
    u32 t;
    s32 ph;
    MATCH_HOLD_REG(s32, k, r6);

    /* Empty: marks r5 as used so the prologue saves it, as in the ROM. */
    MATCH_CLOBBER(r5);
    t = gRoomFrameCount >> 1;
    ph = phase;
    /* Emits only the `ldr r6, =0xFFFFFF00`; see above. */
    MATCH_CONST(k, -0x100);
    part->y = baseY + Wave(table, t, ph + k) * amplitude;
}

void EnemyCtrl::UpdateOscillateY()
{
    MovingSprite *part = target;
    MATCH_HOLD_REG(const s16 *, table, r6) = gSineTable;
    s32 t = __udivsi3(INT_TO_Q8(gRoomFrameCount), period);

    /* Empty: marks r8 as used so the prologue saves it, as in the ROM. */
    MATCH_CLOBBER(r8);
    part->y = baseY + Wave(table, t, phase - 0x100) * amplitude;
}

/* Launches a harmful effect part (state 18's floating popup, the
 * shooters' shots): LaunchEffectPart from gEntitySpawner, then flags
 * bit 2 set and bit 6 cleared on the new part.
 *
 * The OR and the mask are separate locals, assigned in the ROM's order
 * (`flags = 4; flags |= obj[0xc]; mask = -0x41`): a single expression
 * swaps the loads and loads the mask positive. LaunchEffectPart reads
 * five arguments after the pool; the sixth, `f`, is only stored to the
 * stack, as in the ROM. */
MovingSprite *LaunchHarmfulEffectPart(s32 a, s32 b, s32 c, s32 d, s32 e, MovingSprite *src)
{
    MovingSprite *obj;
    s32 flags;
    s32 mask;

    obj = gEntitySpawner->LaunchEffectPart(a, b, c, d, e, src);
    flags = 4;
    flags |= obj->f.flags;
    mask = -0x41;
    obj->f.flags = flags & mask;
    return obj;
}

/* Takes the part to steer. The level spawners attach it through this
 * slot. */
void EnemyCtrl::Attach(MovingSprite *part)
{
    target = part;
}

/* A sound's volume at (x, y), in pixels: full (0x100) within 0x20
 * pixels of the player on both axes, falling to 0 at 0xA0 pixels on the
 * farther axis (EnemyCtrl::Update's state 18 computes the same inline).
 * The C needed six register pins for this under agbcc; it is
 * old_agbcc's code, and old_agbcp gives it as written. */
s32 GetSfxVolumeAt(s32 x, s32 y)
{
    Player *p = gPlayer;
    s32 dx = ABS_BRANCHLESS(x - Q8_TO_INT(p->x));
    s32 d = ABS_BRANCHLESS(y - Q8_TO_INT(p->y));

    LIMIT_MIN(d, dx);
    d = CLAMP_MIN(d, 0x20);
    LIMIT_MAX(d, 0xa0);
    return 0x100 - (d - 0x20) * 2;
}

/* No target, no mode table, no popup, and the enemy motion set. */
void EnemyCtrl::Reset()
{
    target = 0;
    anims = 0;
    animSet = &gEnemyCtrlMotionSet;
    popup = 0;
}

/* g++ sets the vtable pointer back to gEnemyCtrlVtable, then calls ~Ctrl
 * (DestroyCtrl) with the same flags. */
EnemyCtrl::~EnemyCtrl()
{
}

/* Ctrl() (InitCtrl), the vtable pointer, then Reset. */
EnemyCtrl::EnemyCtrl()
{
    Reset();
}

/* The oscillator: the phase's period and offset, and the amplitude. */
void EnemyCtrl::SetOscillator(s32 newPeriod, s32 newPhase, s32 newAmplitude)
{
    period = newPeriod;
    phase = newPhase;
    amplitude = newAmplitude;
}

/* The shooter's period and phase (UpdateShooter). */
void EnemyCtrl::SetShotPeriod(s32 newPeriod, s32 newPhase)
{
    shotPeriod = newPeriod;
    shotPhase = newPhase;
}

/* The attack cycle (UpdateAttackCycle, SetState) - the same stores as
 * spawn_enemies.cpp's inline SetAttackCycle, which the enemy spawners
 * use instead.
 * UNUSED - no caller anywhere in the ROM (checked every src/ and lib/ .c
 * file and every word-aligned Thumb pointer in baserom.gba). */
void EnemyCtrl::SetAttackTiming(s32 newIdleTime, s32 newAttackTime, s32 newCycleOffset)
{
    idleTime = newIdleTime;
    attackTime = newAttackTime;
    cycleOffset = newCycleOffset;
}

/* The trigger box (UpdateTriggerBox), relative to the target. The fourth
 * corner arrives on the stack. */
void EnemyCtrl::SetTriggerBox(s32 l, s32 t, s32 r, s32 b)
{
    boxL = l;
    boxR = r;
    boxT = t;
    boxB = b;
}

/* The per-mode animation table (SetAnimMode). */
void EnemyCtrl::SetModeTable(const s32 *newAnims)
{
    anims = newAnims;
}

/* The enemy kind; the level spawners store `kind` directly.
 * UNUSED - no caller anywhere in the ROM (checked every src/ and lib/ .c
 * file and every word-aligned Thumb pointer in baserom.gba). */
void EnemyCtrl::SetKind(s32 newKind)
{
    kind = newKind;
}
