#ifndef GUARD_CONSTANTS_ATTACK_KINDS_H
#define GUARD_CONSTANTS_ATTACK_KINDS_H

/*
 * How the player hits a crate: QueueCratePlayerCollision takes the kind
 * from gActionCtrlStateAttackKinds[action state] (or INVINCIBLE while
 * the mask level is MASK_LEVEL_INVINCIBLE), and it picks the column of
 * gCrateHitResponse[crate kind][attack kind] and the entry of
 * gAttackKindBreakLimited. ApplyCrateCollision and the collision queue
 * (struct collision_candidate.kind) carry it on.
 */

#define ATTACK_KIND_NONE 0      // ACTION_STATE_DYING, ACTION_STATE_WARP_IN
#define ATTACK_KIND_TOUCH 1     // most other states: walking, crouching, crawling, hanging
#define ATTACK_KIND_JUMP 2      // the jumps, the fall, FLIP_BODY_SLAM_START: bounces off a crate's top
#define ATTACK_KIND_SLIDE 3     // ACTION_STATE_SLIDE
#define ATTACK_KIND_SPIN 4      // the spins, ACTION_STATE_HANG_SPIN
#define ATTACK_KIND_BODY_SLAM 5 // the body slam and its landing; a reinforced crate needs it (or INVINCIBLE)
#define ATTACK_KIND_INVINCIBLE 6 // the invincibility mask, whatever the state

#endif /* GUARD_CONSTANTS_ATTACK_KINDS_H */
