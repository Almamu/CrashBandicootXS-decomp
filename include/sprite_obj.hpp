#ifndef GUARD_SPRITE_OBJ_HPP
#define GUARD_SPRITE_OBJ_HPP

/* The entity and sprite object classes (#664, docs/cplusplus.md): the
 * classes behind gEntityVtable, gSpriteObjVtable, gMovingSpriteVtable and
 * gGroundSpriteVtable, and the subclasses built on them.
 *
 *   Entity        0x1C  gEntityVtable        (11 slots) src/gfx/graphics.cpp
 *   Sprite        0x40  gSpriteObjVtable     (13 slots) src/objects/sprite*.cpp
 *   UiSprite      0x40  gUiSpriteObjVtable              src/objects/sprite_anim.cpp
 *   MovingSprite  0x78  gMovingSpriteVtable  (15 slots) src/objects/moving_sprite*.c
 *   GroundSprite  0x80  gGroundSpriteVtable             src/objects/ground_sprite*.c
 *
 * The sizes are the ROM's: CreateEntity allocates 0x1C bytes,
 * CreateSpriteObj 0x40, CreateMovingSprite 0x78 and CreateGroundSprite
 * 0x80. The C names of Sprite's methods say "SpriteObj"
 * (InitSpriteObj, DestroySpriteObj, ...); cxx_symbols.txt maps them.
 *
 * SpriteObj is the name the controllers (ctrl.hpp and the rest) use for
 * the object they drive: an empty subclass of GroundSprite, so that it
 * has every field from the entity header to the ground sprite's `type`.
 * The controllers' methods take a `SpriteObj *` in their mangled names
 * (cxx_symbols.txt); the object is really a moving sprite or one of its
 * subclasses, and they move to MovingSprite in a later part.
 *
 * The C files keep their views of the same objects: actor.h's `struct
 * actor` (Entity), box_part.h's `struct box_part` and gfx_part.h's
 * `struct gfx_part` (the sprite fields), gobj_1a794.h's `struct gobj`
 * (a ground sprite) and player.h's `struct player` (a ground sprite with
 * the player's fields after it). Each class checks its size against
 * them below; the fields keep their offsets in comments.
 *
 * `#pragma interface`: no vtable is emitted for these (see ctrl.hpp). */
#pragma interface

#include "entity.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "aabb.h"
#include "hitbox_quad.h"
#include "sprite_bank.h"
#include "box_part.h"
#include "gfx_part.h"
#include "objects.h"
#include "gobj_1a794.h"
#include "gfx.h"
#include "vram_pool.h"
#include "globals.h"
}

class Ctrl;

/* The sprite object (src/objects/sprite_obj.cpp, sprite.cpp,
 * sprite_anim.cpp; gSpriteObjVtable): an entity with a sprite bank
 * animation. `bank` is the sprite bank (sprite_bank.h), `tag` the
 * animation in it, `frame` the step within the animation. */
class Sprite : public Entity
{
public:
    void *lastHitbox; // 0x1C - the last hitbox record (AnchorGroundSpriteHitbox)
    union {
        struct anim_table *anim;        // 0x20 - the sprite bank
        const struct sprite_bank *bank; // (the same pointer as sprite_bank.h has it)
    };
    u8 dir; // 0x24 - motion direction bits: 1 right, 2 left, 4 up, 8 down
    // 0x25 - 1: x/y are screen coordinates (DrawAt skips WorldToScreen;
    //        always counts as on screen)
    u8 screenSpace;
    u8 unk_26[2];
    /* 0x28: the OBJ mode, mosaic, 8bpp, mirror and priority bits of the
     * sprite's OAM entries, as a byte or as bitfields. */
    union {
        u8 mirror; // bit 4: X mirrored, bit 5: Y mirrored
        struct MirrorBits {
            u8 gfxMode:2; // OBJ mode (Get/SetSpriteGfxMode); 1: semi-transparent
            u8 mosaic:1;
            u8 colorMode:1; // 8bpp
            s32 flipX:1;    // signed: test them with `< 0` (`lsl #27`, a sign test)
            s32 flipY:1;
            u8 priority:2; // OBJ priority (Get/SetSpritePriority)
        } __attribute__((packed)) mirrorBits;
        /* The same bits unsigned, as box_part.h has them: read as
         * values (`lsl #27; lsr #31`), not tested. */
        struct MirrorFlags {
            u32 gfxMode:2;
            u32 mosaic:1;
            u32 colorMode:1;
            u32 mirrorX:1;
            u32 mirrorY:1;
            u32 priority:2;
        } __attribute__((packed)) mirrorFlags;
    } __attribute__((packed)); // one byte, not the 4 of an ARM union
    u8 palette:4;              // 0x29 - low nibble: the OBJ palette slot
    u8 unk_29_4:4;
    u8 unk_2A[2];
    u8 animating; // 0x2C - nonzero while the animation timer runs
    u8 tag;       // 0x2D - the animation (SetTargetAnim)
    u8 unk_2E[2];
    s32 frame;     // 0x30 - the step within the animation
    s32 stepTimer; // 0x34 - ticks spent on the current step
    u8 animDone;   // 0x38 - set once a non-looping animation ends
    u8 unk_39[3];
    u16 affine; // 0x3C - nonzero: DrawWithOffset draws the affine pieces
    u8 unk_3E[2];

    Sprite();                                      // InitSpriteObj
    virtual s32 CheckPlayerContact();              // 1 CheckSpritePickup
    virtual const struct hitbox_quad *GetBounds(); // 2 GetSpriteObjHitbox
    virtual void Update();                         // 3 UpdateSpriteObj
    virtual void Draw();                           // 4 DrawSpriteObj
    virtual u8 IsOnScreen();                       // 5 IsSpriteObjOnScreen
    virtual s32 OverlapsRect(struct aabb *region); // 6 SpriteObjOverlapsRect
    virtual u8 IsNearCamera();                     // 7 IsSpriteObjNearCamera
    virtual s32 IsInsideRect(struct aabb *box);    // 8 IsSpriteObjInsideRect
    virtual s32 GetClassId();                      // 9 GetSpriteObjClassId
    virtual ~Sprite();                             // 10 DestroySpriteObj
    virtual s32 GetPriority();                     // 11 GetSpriteObjPriority
    virtual s32 ApplyVelocity();                   // 12 ApplySpriteObjVelocity

    /* CreateSpriteObj's `new Sprite(id, x, y)`: the constructor inlined,
     * then the spawn's id and position. */
    Sprite(u16 id, u16 px, u16 py)
    {
        Reset();
        this->id = id;
        x = INT_TO_Q8((s32)px);
        y = INT_TO_Q8((s32)py);
    }
    static Sprite *Create(u16 id, u16 x, u16 y, u16 unused); // CreateSpriteObj

    void Reset();                            // ResetSpriteObj
    struct aabb GetAnimBounds();             // GetSpriteBounds: the animation's box[1]
    struct aabb GetAnimHitbox();             // GetSpriteHitbox: the animation's box[0]
    struct aabb GetAttackBox();              // GetSpriteAttackBox: the frame's
    struct aabb GetBodyBox();                // GetSpriteBodyBox: the frame's
    void AdvanceAnim();                      // AdvanceSpriteAnim
    s32 HitboxOverlaps(struct aabb *region); // SpriteHitboxOverlaps
    s32 GetAnimPaletteSlot();
    s32 GetTileBase();
    const struct sprite_frame *GetFrame(); // GetSpriteFrame
    const struct sprite_point *GetFrameAnchor();
    const struct hitbox_quad *GetFrameThirdBox();
    const struct hitbox_quad *GetFrameAttackBox();
    const struct hitbox_quad *GetFrameBodyBox();
    const struct sprite_anim *GetAnim(); // GetSpriteAnimRecord
    void SetFrameIndex(s32 step);        // clamped to the animation's last step
    u8 GetScreenSpace();
    void SetScreenSpace(u8 value);
    s32 IsHidden(); // `blink`
    void ToggleHidden();
    s32 IsSolid(); // IsPartSolid, ...
    void ClearSolid();
    void SetSolid();
    s32 IsVulnerable(); // IsSpriteObjVulnerable, ...
    void ClearVulnerable();
    void SetVulnerable();
    void ResetAnimIndex(); // `tag`
    s32 IsCollisionEnabled();
    void DisableCollision();
    void EnableCollision();
    u8 GetAnimating();
    void SetAnimating(u8 value);
    void SetFlipX(u8 value);
    void SetFlipY(u8 value);
    void SetAnimDone(u8 value);
    u8 GetAnimPaletteId();
    s32 GetPalette();
    void SetPalette(s32 value);
    void SetBank(void *bank); // SetSpriteAnimTable
    void *GetBank();          // GetSpriteAnimTable
    u8 IsAnimLooping();
    u8 GetAnimFrameCount();
    u8 GetAnimDuration();
    void ResetFrameIndex();
    void SetFrameTimer(s32 value);
    void ResetFrameTimer();
    void SetAnimIndex(u8 value);
    void SetAnim(u8 index); // SetSpriteAnim: the animation, from its start
    void IncFrameIndex();
    void IncFrameTimer();
    void SetMoveAxes(u8 value); // `dir`
    u8 GetMoveAxes();
    s32 GetFrameIndex();
    s32 GetFrameTimer();
    u8 GetAnimIndex(); // GetSpriteAnim
    s32 GetGfxMode();
    void SetGfxMode(s32 value);
    s32 GetFlipX();
    s32 GetFlipY();
    u8 GetAnimDone();
    s32 GetMosaic();
    s32 GetOamPalette();
    s32 GetColorMode();
    u16 GetAffine();
    void SetAffine(u16 value);
    void DrawWithOffset(s32 dx, s32 dy);
    void SetPriority(s32 value);
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(Sprite) == 0x40);

/* A sprite on the HUD or a menu (gUiSpriteObjVtable;
 * src/objects/sprite_anim.cpp): its OBJ priority is its own. */
class UiSprite : public Sprite
{
public:
    UiSprite();                // InitUiSpriteObj
    virtual ~UiSprite();       // DestroyUiSpriteObj
    virtual s32 GetPriority(); // GetSpritePriority: the mirror byte's
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(UiSprite) == 0x40);

/* The moving sprite (src/objects/moving_sprite*.c, gMovingSpriteVtable):
 * a sprite with a controller (`mover`), per-axis speeds and their ramps,
 * and the terrain probe's state. Its methods are still C. */
class MovingSprite : public Sprite
{
public:
    s32 unk_40;              // 0x40 - ResetMovingSprite clears it; nothing reads it
    Ctrl *mover;             // 0x44 - its controller
    struct speed_ramp rampX; // 0x48 - speedX's ramp
    struct speed_ramp rampY; // 0x54 - speedY's ramp
    s32 speedX;              // 0x60
    s32 speedY;              // 0x64
    u8 hitAxes;              // 0x68 - the collision axes the terrain probe resolved
    u8 probeTries;           // 0x69
    u8 unk_6A[2];
    s32 prevX;   // 0x6C - the previous position (Q8), cached by ApplyVelocity
    s32 prevY;   // 0x70
    s32 hitMask; // 0x74 - the probe axes hit this frame

    virtual s32 CheckPlayerContact();                       // 1 CollideMovingSprite
    virtual void Update();                                  // 3 UpdateMovingSprite
    virtual s32 GetClassId();                               // 9 GetMovingSpriteClassId
    virtual ~MovingSprite();                                // 10 DestroyMovingSprite
    virtual s32 ApplyVelocity();                            // 12 ApplySpriteVelocity
    virtual void HandleEvent(s32 from, s32 event, s32 arg); // 13 HitMovingSprite
    virtual void TouchPlayer();                             // 14 CheckPlayerContact
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(MovingSprite) == 0x78);
COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(MovingSprite) == sizeof(struct box_part));
// gfx_part.h's view stops at prevY
COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(struct gfx_part) <= sizeof(MovingSprite));

/* The ground sprite (src/objects/ground_sprite*.c, gGroundSpriteVtable):
 * a moving sprite that probes the terrain under it. Its methods are
 * still C. struct gobj (gobj_1a794.h) is its C view. */
class GroundSprite : public MovingSprite
{
public:
    s32 type; // 0x78 - the platform type (CreatePlatform)
    u8 unk_7C[4];

    virtual s32 CheckPlayerContact(); // 1 CollideGroundSprite
    virtual void Update();            // 3 UpdateGroundSprite
    virtual void Draw();              // 4 DrawGroundSprite
    virtual s32 GetClassId();         // 9 GetGroundSpriteClassId
    virtual ~GroundSprite();          // 10 DestroyGroundSprite
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(GroundSprite) == sizeof(struct gobj));

/* The object a controller drives, as the controllers' methods name it
 * (see the top of this file). */
class SpriteObj : public GroundSprite
{
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(SpriteObj) == sizeof(struct gobj));

/* The player (gPlayer; its own code is still C) as the ground sprite it
 * is. Each call reads gPlayer again. */
static inline GroundSprite *PlayerSprite()
{
    return (GroundSprite *)gPlayer;
}

/* A list of sprite objects (box_part.h's struct part_list is its C view):
 * Update compacts `items` and fills `visible`, the parts on screen, which
 * Collide walks. Its other methods are still C (src/objects/part_list.c,
 * part_list_cull.c, part_collide.c). */
class PartList
{
public:
    s32 capacity;           // 0x00
    s32 count;              // 0x04
    s32 visibleCount;       // 0x08
    MovingSprite **items;   // 0x0C
    MovingSprite **visible; // 0x10

    void Update();                                                  // UpdatePartList
    void Collide(struct aabb box, s32 unused, MovingSprite *other); // CollidePartList
    void CollideWithPlayer(struct aabb box, MovingSprite *part);    // CollidePartWithPlayer
    void CollideWithObject(struct aabb box, MovingSprite *part,
                           MovingSprite *other); // CollidePartWithObject
    /* part 7c (include/part_list.hpp; src/objects/part_list.cpp,
     * part_list_cull.cpp) */
    PartList(s32 capacity);          // InitPartList
    ~PartList();                     // DestroyPartList
    void Draw();                     // DrawPartList
    void Remove(MovingSprite *part); // RemoveFromPartList
    void RemoveAt(s32 index);        // RemovePartListAt
    void Add(MovingSprite *part);    // AddToPartList
    void Cull();                     // CullPartList
    void Clear();                    // ClearPartList
    void CollideClass(s32 classId);  // CollidePartsOfClass
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(PartList) == sizeof(struct part_list));

/* The sprite graphics managers (src/gfx/graphics.cpp). */

/* The OAM shadow buffer (gOamBuffer; gfx.h's struct oam_shadow_buffer is
 * its C view): `count` entries of the 128-entry shadow table are in use,
 * `base` of them kept from frame to frame (MarkBase, Rewind), and
 * `matrixCount` affine matrices handed out this frame. Matrix `m`'s
 * pa/pb/pc/pd are the affine parameters of entries 4m..4m+3. */
class OamBuffer
{
public:
    s32 count;                          // 0x00
    s32 base;                           // 0x04
    s32 matrixCount;                    // 0x08
    union oam_shadow_entry table[0x80]; // 0x0C

    OamBuffer();                              // InitOamBuffer
    ~OamBuffer();                             // DestroyOamBuffer
    void SetAffineScales(u16 *scales, s32 n); // SetOamAffineScales
    void Append(void *entries, s32 n);        // AppendOamEntries
    void HideUnused();                        // HideUnusedOamEntries
    void Rewind();
    void MarkBase();
    void Reset();
    void Commit();
    void Add(const void *entry); // AddOamEntry
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(OamBuffer) == sizeof(struct oam_shadow_buffer));

/* The OBJ VRAM upload cursor (gObjVramCursor; vram_pool.h's struct
 * vram_upload_cursor): uploads tiles to OBJ VRAM from tile `baseTile` on,
 * `offset` bytes in so far, `mark` the offset Rewind goes back to. */
class ObjVramCursor
{
public:
    u32 mark;     // 0x00
    u32 offset;   // 0x04
    s32 baseTile; // 0x08

    ObjVramCursor(s32 baseTile); // InitObjVramCursor
    ~ObjVramCursor();            // DestroyObjVramCursor
    void Rewind();
    void Mark();
    s32 GetFreeBytes();
    s32 GetTile();
    void Reset();
    s32 Reserve(s32 size);
    s32 Upload(void *src, s32 size);
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(ObjVramCursor) == sizeof(struct vram_upload_cursor));

/* The OBJ palette cache (gPaletteCache; vram_pool.h's struct
 * palette_cache): maps the `count` palettes of `palettes` onto the 16
 * OBJ palette banks. */
class PaletteCache
{
public:
    u16 count; // 0x00
    u8 pad_02[2];
    const u8 *palettes;           // 0x04
    u8 *slotOf;                   // 0x08 - the bank palette `id` is in, or 0xFF
    u8 isFree[16];                // 0x0C
    u8 locked[16];                // 0x1C
    u8 slots[16][TILE_SIZE_4BPP]; // 0x2C - each bank's colours
    u8 dirty;                     // 0x22C
    u8 pad_22d[3];

    PaletteCache();  // InitPaletteCache
    ~PaletteCache(); // DestroyPaletteCache
    void LoadSlot(s32 slot, s32 id);
    void BindSlot(s32 slot, s32 id);
    s32 ClaimSlot(s32 slot);
    void Unlock(s32 id);
    void Lock(s32 id);
    void UploadSlot(s32 slot);
    void Upload();
    u8 GetSlot(s32 id); // GetPaletteSlot
    s32 FreeSlot(s32 slot);
    void FreeUnlockedSlots();
    void SetSource(u16 count, const u8 *palettes);
    void Clear();
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(PaletteCache) == sizeof(struct palette_cache));

/* The sprite bank set (gSpriteBankSet; globals.h's struct
 * sprite_bank_set): InitLevelState points it at gSpriteBankTable. */
class SpriteBankSet
{
public:
    const struct sprite_bank_table *table;

    SpriteBankSet();  // InitSpriteBankSet
    ~SpriteBankSet(); // DestroySpriteBankSet
};

/* The sprite renderer (gSpriteRenderer, an empty object InitLevelState
 * allocates): draws a sprite's OAM pieces. */
class SpriteRenderer
{
public:
    SpriteRenderer();  // InitSpriteRenderer
    ~SpriteRenderer(); // DestroySpriteRenderer
    void DrawAt(Sprite *part, s32 x, s32 y);
    void Draw(Sprite *part);
};

#endif /* !GUARD_SPRITE_OBJ_HPP */
