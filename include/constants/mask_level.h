#ifndef GUARD_CONSTANTS_MASK_LEVEL_H
#define GUARD_CONSTANTS_MASK_LEVEL_H

/*
 * Values of `level_state.maskLevel`, the number of Aku Aku masks the
 * player has (SetMaskLevel, RaiseMaskLevel). A hit with a mask takes one
 * away (PlayerHandleEvent); the third makes the player invincible: it
 * plays SONG_DRUMS (SetMaskLevel, PlayRoomMusic) and hits do nothing.
 * Packed into two bits of the save stats (GameProgress::maskLevel,
 * level_state.h).
 */

#define MASK_LEVEL_NONE 0
#define MASK_LEVEL_ONE 1
#define MASK_LEVEL_TWO 2
#define MASK_LEVEL_INVINCIBLE 3

#endif /* GUARD_CONSTANTS_MASK_LEVEL_H */
