/* The launch pad (LaunchPad, gLaunchPadVtable; include/level_select.hpp):
 * a 0x78-byte moving sprite with a factory, a player-contact check that
 * sends the player EVENT_LAUNCH_PAD, a constructor and a destructor: the
 * green pad of entity type 0x3D (sprite bank 28). The event makes the
 * action controller launch Crash upward in an air spin with a full tornado
 * charge (Y motion 0x13, or 0x14 with A held). Its key method is here, so
 * g++ emits its vtable here (ldscript.txt places it).
 *
 * UNUSED - no `bl`/`.4byte` reference in asm/, expected/ or src/: the
 * out-of-line constructor (Spawn has it inlined). Matched anyway.
 *
 * Split from menus/level_select.cpp (GitHub issue #26) in #767, with its
 * flags: old_agbcp (Makefile OLD_AGBCC_OBJS), as its C was old_agbcc, and
 * -fno-implement-inlines (NO_IMPLEMENT_INLINES_OBJS). */

#include "level_select.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "match.h"
#include <agb_syscall.h>
#include <libgcc.h>
#include "text.h"
#include "level.h"
#include "math_util.h"
}

/* The factory (the constructor inlined): places the pad at (x, y) pixels
 * with record id `id`, adds it to the collidable list, and starts
 * animation 0 of the set at `**gSpriteBankSet + 0x150`, unmirrored. Called
 * from the level spawners (spawn_objects.c's SpawnLaunchPadEntity). */
LaunchPad *LaunchPad::Spawn(u16 id, u16 x, u16 y, u16 unused)
{
    LaunchPad *obj = new LaunchPad(id, x, y);

    CollidableList()->Add(obj);
    obj->bank = AnimTable(0x150);
    StartAnim(obj, 0);
    obj->mirrorFlags.mirrorX = 0;
    obj->mirrorFlags.mirrorY = 0;
    {
        const struct sprite_anim *anims = obj->bank->anims;
        const struct sprite_anim *rec = &anims[obj->tag];
        u32 slot = gPaletteCache->GetSlot(rec->paletteId);

        obj->palette = slot;
    }
    return obj;
}

/* Slot 14: if the player collides (flags bit 7) and overlaps the pad's
 * hitbox, sends the player EVENT_LAUNCH_PAD. */
void LaunchPad::TouchPlayer()
{
    if (gPlayer->f.flags >> 7) {
        struct aabb box = GetAnimHitbox();

        if (box.w != 0 && gPlayer->TouchesBox(&box))
            gPlayer->HandleEvent(0, EVENT_LAUNCH_PAD, 0);
    }
}

/* Slot 10. */
LaunchPad::~LaunchPad()
{
}

/* Clears flags bit 6, the "vulnerable" bit (the launch pad's own
 * out-of-line copy of Sprite::ClearVulnerable; the constructor calls it). */
void LaunchPad::ClearVulnerable()
{
    f.b.vulnerable = 0;
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, data/, src/ and a
 * whole-ROM Thumb-pointer scan; Spawn inlines it instead). */
LaunchPad::LaunchPad()
{
    ClearVulnerable();
}
