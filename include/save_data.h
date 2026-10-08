#ifndef __SAVE_DATA_H__
#define __SAVE_DATA_H__

#include "level_state.h"

/* One 0x70-byte save slot, as SaveData::ReadSlot/WriteSlot copy it:
 * the level state's packed progress block (struct game_progress;
 * PackSaveData/UnpackSaveData), then the current level and the sound
 * and music volumes (SaveMenu::SaveToSlot builds one,
 * SaveMenu::LoadInput restores one). */
struct save_slot {
    struct game_progress progress; /* 0x00 */
    u8 level;                      /* 0x68 */
    u16 sfxVolume;                 /* 0x6a */
    u16 musicVolume;               /* 0x6c */
};
COMPILE_TIME_ASSERT(save_data_h, offsetof(struct save_slot, level) == 0x68);
COMPILE_TIME_ASSERT(save_data_h, sizeof(struct save_slot) == 0x70);

/* The save data and the save transfer, as the C side sees them: tags.
 * Their layouts and code are C++, classes SaveData and SaveTransfer
 * (include/save_data.hpp). */
struct save_data;
struct save_transfer;

#endif /* __SAVE_DATA_H__ */
