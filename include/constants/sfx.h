#ifndef GUARD_CONSTANTS_SFX_H
#define GUARD_CONSTANTS_SFX_H

/*
 * Sound-effect IDs: the index into gSfxTable (sound/sfx_table.json, 99
 * entries) that PlaySfx, PlayAmbientSfx and StopSfx take. Neither the
 * ROM nor the GAX sound-effect set (sound/gax_sfx_manifest.json) names
 * them, so each name comes from where the game plays it; the comment
 * says where. An ID whose sound isn't clear from its callers isn't
 * named yet, and its call sites keep the number
 * (`tools/magic_numbers.py --topic sfx` lists them).
 */

/* The player. */
#define SFX_JUMP 0xd            // the jump (A): on foot, hanging, on the polar bear
#define SFX_HIGH_JUMP 0xc       // from a crouch or a slide; the double jump
#define SFX_SPIN 0xa            // the spin (B); the jetpack's barrel roll
#define SFX_TORNADO_SPIN 0x57   // + the turn's variant 0-2 (StartActionCtrlTornadoSpin)
#define SFX_SLIDE 0x1a          // the slide (R) from a run
#define SFX_SKID 0x36           // the run's skid (ActionCtrlSetTargetAnim)
#define SFX_BODY_SLAM_LAND 0x19 // landing from a body slam (ActionCtrlStateAirborne)
#define SFX_PLAYER_HURT 0x1b    // KillPlayer, a masked hit, HurtPolarPlayer; a company logo
#define SFX_WARP 0x2c           // warping into or out of a bonus room or the gem path
#define SFX_BOUNCE 0x21         // bouncing on a part from above (CollidePartWithPlayer)

/* Aku Aku (maskLevel). */
#define SFX_AKU_AKU_GAIN 0x1 // OpenAkuAkuCrate, SpawnStartMarker, AddPolarAkuAkuMask
#define SFX_AKU_AKU_LOSE 0x0 // a hit takes a mask (PlayerHandleEvent, RemovePolarAkuAkuMask)

/* Pickups. */
#define SFX_WUMPA 0x8       // PickUpWumpa, DispenseJetpackWumpa, DispensePolarWumpa
#define SFX_EXTRA_LIFE 0x7  // PickUpExtraLife, life crates, jetpack rings
#define SFX_HUD_COLLECT 0xe // a wumpa or life reaches the HUD; the pause menu's volume test
#define SFX_CRYSTAL 0x1c    // player event 27
#define SFX_GEM 0x1f        // player events 29-34
#define SFX_CLOCK 0x18      // the stopwatch (StartTimeTrial), time crates (FreezeLevelClock)

/* Crates. */
#define SFX_CRATE_BREAK 0x3             // BreakCrate, the jetpack and polar crates
#define SFX_EXPLOSION 0x4               // TNT and nitro crates, boss parts, jetpack bombs
#define SFX_ARROW_CRATE_BOUNCE 0x2      // landing on an arrow crate (ApplyCrateCollision)
#define SFX_CHECKPOINT 0x17             // OpenCheckpointCrate, the jetpack/polar checkpoints
#define SFX_OUTLINE_CRATES_SOLIDIFY 0xf // the "!" crate fills the outlines in
#define SFX_SLOT_CRATE_SPIN 0x10        // UpdateSlotCrate
#define SFX_TNT_TICK 0x11               // LightTntCrate, each UpdateTntCountdown step

/* Enemies and bosses. */
#define SFX_ENEMY_KNOCKED_AWAY 0x5   // an enemy sent flying (HitEnemy, UpdatePolarPenguin)
#define SFX_ENEMY_HOP 0x14           // UpdateEnemyHop
#define SFX_HOVERCRAFT_PART_HIT 0x45 // a cannon, launcher or side gun hit but not destroyed

/* Vehicles. */
#define SFX_JETPACK_SHOOT 0x24   // ambient (JetpackPlayerStateFly)
#define SFX_CANNONBALL_FIRE 0x30 // SpawnJetpackCannonball
#define SFX_FIREBALL_LAUNCH 0x38 // SpawnHovercraftFireball, SpawnAirshipFireball
#define SFX_BOOST_PAD 0x28       // UpdatePolarBoostPad

/* Menus. */
#define SFX_MENU_MOVE 0x46               // a menu cursor moves
#define SFX_MENU_SELECT 0x49             // a menu choice is confirmed
#define SFX_MENU_BACK 0x47               // the save menu's B
#define SFX_MENU_ERROR 0x48              // a move or choice that isn't allowed
#define SFX_LEVEL_SELECT_CONFIRM 0x52    // LevelSelectConfirm
#define SFX_LEVEL_SELECT_NEXT_WORLD 0x55 // LevelSelectNextWorld
#define SFX_LEVEL_SELECT_PREV_WORLD 0x56 // LevelSelectPrevWorld
#define SFX_ZOOM_BG_IN 0x53              // the level-select picture zooms in (UpdateZoomBg)
#define SFX_ZOOM_BG_OUT 0x54             // ClearZoomBgPicture

/* A cutscene slide's "no sound effect" (struct cutscene_slide.sfx). */
#define SFX_NONE 0x63

#endif /* GUARD_CONSTANTS_SFX_H */
