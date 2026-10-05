#ifndef GUARD_LEVEL_SELECT_PARTS_H
#define GUARD_LEVEL_SELECT_PARTS_H

/* Shared by src/graphics/actor_part_1da38.c and actor_part_1dfec.c
 * (GitHub issues #28/#29, ROM 0x0801DA38-0x0801E578): the level-select
 * screen's (actor_part_1b85c.c's `struct level_menu`) sub-objects - the
 * zooming BG2 picture, the per-level page entries and the cursor panel.
 * All of them own animated sprite parts built by sub_8008904. */

struct vmethod
{
    s16 thisOffset;
    u8 unk_2[2];
    void *fn;
};

/* The sprite part's method table (at part+0x18); +0x50 is the
 * destructor. */
struct sprite_vtable
{
    u8 unk_00[0x50];
    struct vmethod m50; // 0x50
};

/* One 28-byte animation record, `anim_table.records[animIndex]`. */
struct anim_record
{
    u8 unk_00[0x14];
    u8 tileRecord; // 0x14 - UnlockPalette/LockPalette tile-cache record
    u8 unk_15;
    u8 frameCount; // 0x16
    u8 unk_17[5];
};

struct anim_table
{
    struct anim_record *records;
};

/* The 0x40-byte animated sprite part `sub_8008904` constructs. */
struct sprite
{
    s32 x;                        // 0x00 - Q8
    s32 y;                        // 0x04 - Q8
    u8 unk_08[0x10];
    struct sprite_vtable *vtable; // 0x18
    u8 unk_1C[4];
    struct anim_table *anim;      // 0x20
    u8 unk_24[5];
    u8 palette:4;                 // 0x29
    u8 unk_29_4:4;
    u8 unk_2A[3];
    u8 animIndex;                 // 0x2D
    u8 unk_2E[2];
    s32 frame;                    // 0x30
    u8 unk_34[4];
    u8 animDone;                  // 0x38
    u8 unk_39[7];
};

COMPILE_TIME_ASSERT(sizeof(struct sprite) == 0x40);

/* One level entry on the level-select page (0x14 bytes, constructor
 * CreateLevelSelectEntry, destructor DestroyLevelSelectEntry, method table gLevelSelectEntryVtable).
 * `icon` shows the level's picture (or, past index 4, a per-world
 * animation), `frame` the surrounding box. */
struct level_item
{
    s32 id;                     // 0x00 - level id (sub_801DE2C)
    u8 selected;                // 0x04
    u8 unk_05[3];
    struct sprite *icon;        // 0x08
    struct sprite *frame;       // 0x0C
    u8 *vtable;                 // 0x10
};

extern void ***gUnknown_030012D0;
extern void *gPaletteCache;
extern u8 gLevelSelectEntryVtable[];

extern void *sub_8026EDC(u32 size);
extern void sub_8026ED0(void *p);
extern struct sprite *sub_8008904(void *mem);
extern void sub_80088D8(struct sprite *part, s32 value);
extern void sub_80087C0(struct sprite *part);
extern void sub_80087B4(struct sprite *part);
extern void sub_800872C(struct sprite *part, s32 arg);
extern s32 sub_800815C(struct sprite *part);
extern void sub_800737C(struct sprite *part, s32 x, s32 y);
extern void sub_8008890(struct sprite *part, s32 dx, s32 dy);
extern void sub_8008044(struct sprite *part);
extern void UnlockPalette(void *cache, u8 record);
extern void LockPalette(void *cache, u8 record);
extern s32 RandRange(s32 max);
extern void LoadTaggedAsset(void *asset, void *dest);

typedef void (*dtor_fn)(void *self, s32 flags);

/* `delete part;` - the part's virtual destructor with the "free" flag. */
#define DELETE_PART(p)                                                         \
    do                                                                         \
    {                                                                          \
        struct sprite *_p = (p);                                               \
        if (_p != NULL)                                                        \
        {                                                                      \
            struct vmethod *_m = &_p->vtable->m50;                             \
            ((dtor_fn)_m->fn)((u8 *)_p + _m->thisOffset, 3);                   \
        }                                                                      \
    } while (0)

/* The part's current animation record. A macro, not an inline: an
 * inline returning the record's address changes the load order. */
#define PART_RECORD(p) ((p)->anim->records[(p)->animIndex])

static inline struct anim_table *AnimTable(s32 offset)
{
    return (struct anim_table *)((u8 *)**gUnknown_030012D0 + offset);
}

/* Shows animation frame `frame`, clamped to the animation's last one. */
static inline void SetFrame(struct sprite *p, s32 frame)
{
    s32 n = PART_RECORD(p).frameCount;
    if (frame >= n)
        frame = n - 1;
    p->frame = frame;
}

static inline void SetAnim(struct sprite *p, s32 anim)
{
    p->animIndex = anim;
    sub_80087C0(p);
    sub_80087B4(p);
    sub_800872C(p, 0);
}

#endif // GUARD_LEVEL_SELECT_PARTS_H
