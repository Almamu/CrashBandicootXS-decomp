#include "game/level/bg_layer.hpp"
#include "game/crash/player.hpp"
#include "game/crates/crate.hpp"
#include "game/crates/crate_list.hpp"

extern "C" {
#include "game/level/level.h"
#include "globals.h"
#include "engine/util/math_util.h"
}

using Game::Crash::Player;
using Game::Crates::Crate;

/* Player::HasRoomForAnim (engine/include/game/crash/player.hpp; PlayerHasRoomForAnim): the
 * action controller's states ask it before an animation with a taller box
 * (a stand-up from the crawl, the crouch's jump). */

/* An animation's first box. Through an inline, the `+ 4` is its own add
 * before the copy into the variable, as in the ROM. */
static inline const struct hitbox_quad *AnimBox(const struct sprite_bank *bank, s32 anim)
{
    return bank->anims[anim].box;
}

/* Whether animation `anim`'s box fits where the player is: the terrain
 * probe from the player's position, raised by the box's offset, toward the
 * player's facing finds nothing, and no crate in the crate list would
 * touch the box (Crate::PlayerAnimWouldTouch).
 *
 * The position is one block copy (both words loaded, then both stored,
 * then `y` reloaded for `origY`), and the crate list's count is reloaded
 * for each test (a guarded do-while). The box offset is added on the left
 * (the ROM loads it first). */
u8 Player::HasRoomForAnim(s32 anim)
{
    const struct hitbox_quad *box;
    /* `h` is needed: inlined, the instructions differ */
    u8 h;
    struct vec2 pos;
    s32 origY;
    s32 probeDir;
    s32 i;

    box = AnimBox(bank, anim);
    h = box->h;
    pos = Pos();
    origY = y;
    if (mirrorBits.flipX < 0)
        probeDir = PART_DIR_LEFT;
    else
        probeDir = PART_DIR_RIGHT;
    pos.y = INT_TO_Q8(box->offY) + pos.y;
    Q8_TO_INT_INPLACE(pos.x);
    Q8_TO_INT_INPLACE(pos.y);
    if (gLevelLayers->Probe(probeDir, &pos, h, &origY))
        return 0;

    i = 0;
    if (i < RoomCrates()->count)
        do {
            Crate *c = RoomCrates()->slots[i];

            if (c->GetClassId() == 3 && c->PlayerAnimWouldTouch(anim) == 1)
                return 0;
            i++;
        } while (i < RoomCrates()->count);
    return 1;
}
