#include "core.h"
#include "actor.h"

struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

extern void SetAabbPos(void *buf, s32 arg1, s32 arg2);
extern void SetAabbSize(void *buf, s32 arg1, s32 arg2);
extern void *GetSpriteFrame(void *part);
extern u8 gStaticData_0816B2F8[];

/* A third AABB-for-keyframe builder (see sub_8007B00/sub_8007B98 in
 * src/graphics/actor_part.c), this time selecting its 6-byte
 * `{s16 x, s16 y, u8 w, u8 h}` record via a `GetSpriteFrame(part)`-derived
 * "info" struct rather than `part`'s own keyframe table pointer:
 * `info+4` points to a byte whose upper nibble (0-15, but only 0-6
 * handled - anything above 6 and unhandled 1/2/6 fall through to the
 * same default) selects one of `info+0x14`, `info+0xc`, or the fixed
 * fallback table `gStaticData_0816B2F8`. */
void *sub_8007C30(void *dest, void *pt)
{
    register void *part asm("r6") = pt;
    struct aabb buf_;
    void *info;
    void *rec;
    s32 offX, offY;
    s32 w, h;
    s32 x, y;
    u8 type;

    info = GetSpriteFrame(part);
    type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    switch (type) {
    case 0:
    case 3:
    case 4:
        rec = (u8 *)info + 0x14;
        break;
    case 1:
    case 2:
    case 6:
        rec = gStaticData_0816B2F8;
        break;
    case 5:
        rec = (u8 *)info + 0xc;
        break;
    default:
        rec = gStaticData_0816B2F8;
        break;
    }

    x = *(s32 *)part >> 8;
    offX = *(s16 *)((u8 *)rec + 0);
    y = *(s32 *)((u8 *)part + 4) >> 8;
    offY = *(s16 *)((u8 *)rec + 2);
    w = *((u8 *)rec + 4);
    h = *((u8 *)rec + 5);

    offX = offX + x;
    offY = offY + y;
    SetAabbPos(&buf_, offX, offY);
    SetAabbSize(&buf_, w, h);

    {
        u8 *flagsAddr = (u8 *)part + 0x28;
        register s32 flags asm("r1");
        register s32 shifted asm("r0");

        flags = *flagsAddr;
        shifted = flags << 27;
        if (shifted < 0) {
            buf_.field_0 = (*(s32 *)part >> 8) * 2 - (buf_.field_0 + buf_.field_8);
        }
        {
            register s32 addr asm("r3") = (s32)flagsAddr;
            asm("ldrb %1, [%1]\n\tlsl %0, %1, #0x1a" : "=r" (shifted), "+r" (addr));
        }
        if (shifted < 0) {
            buf_.field_4 = (*(s32 *)((u8 *)part + 4) >> 8) * 2 - (buf_.field_4 + buf_.field_c);
        }
    }

    *(struct aabb *)dest = buf_;
    return dest;
}

/* Same shape as sub_8007C30 above, with a simpler switch: only
 * `info+0xc` or the `gStaticData_0816B2F8` fallback are ever selected
 * (cases 0/2/3/4/6 to `info+0xc`; cases 1/5 and the out-of-range
 * default all to the fallback). */
void *sub_8007CF8(void *dest, void *pt)
{
    register void *part asm("r6") = pt;
    struct aabb buf_;
    void *info;
    void *rec;
    s32 offX, offY;
    s32 w, h;
    s32 x, y;
    u8 type;

    info = GetSpriteFrame(part);
    type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    switch (type) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
        rec = (u8 *)info + 0xc;
        break;
    case 1:
    case 5:
        rec = gStaticData_0816B2F8;
        break;
    default:
        rec = gStaticData_0816B2F8;
        break;
    }

    x = *(s32 *)part >> 8;
    offX = *(s16 *)((u8 *)rec + 0);
    y = *(s32 *)((u8 *)part + 4) >> 8;
    offY = *(s16 *)((u8 *)rec + 2);
    w = *((u8 *)rec + 4);
    h = *((u8 *)rec + 5);

    offX = offX + x;
    offY = offY + y;
    SetAabbPos(&buf_, offX, offY);
    SetAabbSize(&buf_, w, h);

    {
        u8 *flagsAddr = (u8 *)part + 0x28;
        register s32 flags asm("r1");
        register s32 shifted asm("r0");

        flags = *flagsAddr;
        shifted = flags << 27;
        if (shifted < 0) {
            buf_.field_0 = (*(s32 *)part >> 8) * 2 - (buf_.field_0 + buf_.field_8);
        }
        {
            register s32 addr asm("r3") = (s32)flagsAddr;
            asm("ldrb %1, [%1]\n\tlsl %0, %1, #0x1a" : "=r" (shifted), "+r" (addr));
        }
        if (shifted < 0) {
            buf_.field_4 = (*(s32 *)((u8 *)part + 4) >> 8) * 2 - (buf_.field_4 + buf_.field_c);
        }
    }

    *(struct aabb *)dest = buf_;
    return dest;
}

extern void *sub_8007B98(void *dest, void *part);
extern u8 sub_8001688(void *buf1, void *buf2);
extern void _call_via_r4(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void *sub_8025BAC(void *pool, s32 arg1, s32 kind, s32 x, s32 y, s32 arg5);
extern struct actor *gUnknown_030012D8;
extern void *gEntitySpawner;
extern void *gEntityFlags;

/* `part` (a `struct actor`, same layout used throughout this ROM
 * region) collides with the player (`gUnknown_030012D8`, tested via
 * two `sub_8007B98` AABBs and `sub_8001688`) and, if so, plays a sound
 * at the player's position (the `table+0x68` offset/dead-read idiom
 * matches sub_8007048's `_call_via_r4` call exactly, just keyed off
 * `part->field_0A` instead of `self->field_0A`) and marks itself
 * "collected" (`gEntityFlags` bitmap, same convention as
 * sub_80072D8). `part->field_0A - 0x1b` (0-7) then selects a "kind" to
 * spawn via `sub_8025BAC` at `part`'s own position - case 1 and any
 * out-of-range value spawn nothing. If something spawned, its
 * `+0x28`/`+0xc` flag bytes get tagged - kept as raw offsets since the
 * spawned object's own type isn't established yet.
 *
 * Real C under old_agbcc (issue #9-#11 NAKED retry; the whole file
 * matches under it, so actor_part2.o is in OLD_AGBCC_OBJS - old_agbcc
 * is also what puts the cached player global in r7). Two details: the
 * flag tests' constant 1 is a variable pinned to r6 and assigned inside
 * the first test (`& (one = 1)`), and the `gone` OR uses it (`orrs r0,
 * r6`) - as a plain constant CSE rematerializes it; and the spawned
 * part's `mode = 1` goes through a `u32` local, which materializes the
 * 1 before the `-4` mask as the ROM does. */

struct collect_method {
    s16 thisOffset;
    u8 unk_02[2];
    void (*fn)(void *self, s32 a, s32 b, s32 c);
};

struct collect_part {
    s32 x;
    s32 y;
    u16 id;             // 0x08
    u8 kind;            // 0x0A
    u8 unk_0B;
    u8 gone:1;          // 0x0C
    u8 unk_0C_1:1;
    u8 visible:1;
    u8 hit:1;
    u8 unk_0C_4:3;
    u8 solid:1;
    u8 unk_0D[0xb];
    u8 *vtable;         // 0x18
    u8 unk_1C[0xc];
    u8 mode:2;          // 0x28
    u8 unk_28_2:6;
};

#define COLLECT_FLAGS(p) (*((u8 *)(p) + 0xc))

static inline struct collect_part *SpawnPickup(s32 kind, s32 x, s32 y)
{
    return sub_8025BAC(gEntitySpawner, 0x2b, kind, x, y, 0);
}

s32 sub_8007DBC(struct collect_part *part)
{
    struct aabb a, b;
    struct collect_part *player;
    struct collect_part *spawned;
    u32 flags = COLLECT_FLAGS(part) << 24;
    register u32 one asm("r6");

    if (!((flags >> 27) & (one = 1)) && ((flags >> 26) & one)) {
        sub_8007B98(&a, part);
        if (COLLECT_FLAGS(gUnknown_030012D8) >> 7) {
            sub_8007B98(&b, gUnknown_030012D8);
            if (sub_8001688(&b, &a)) {
                COLLECT_FLAGS(part) |= 8;
                player = (struct collect_part *)gUnknown_030012D8;
                {
                    struct collect_method *m = (struct collect_method *)(player->vtable + 0x68);
                    m->fn((u8 *)player + m->thisOffset, 0, part->kind, 0);
                }
                COLLECT_FLAGS(part) |= one;
                if (part->id != 0xFFFF) do {
                    s32 id = part->id;
                    u8 *base = gEntityFlags;
                    s32 word = id / 32;
                    s32 off = word * 4;
                    u32 *slot = (u32 *)(base + 0x108);

                    slot = (u32 *)((u8 *)slot + off);
                    *slot |= 1 << (id - word * 32);
                } while (0);

                spawned = NULL;
                switch (part->kind) {
                case 0x1d:
                case 0x1e:
                    spawned = SpawnPickup(1, part->x >> 8, part->y >> 8);
                    break;
                case 0x21:
                    spawned = SpawnPickup(6, part->x >> 8, part->y >> 8);
                    break;
                case 0x1f:
                    spawned = SpawnPickup(5, part->x >> 8, part->y >> 8);
                    break;
                case 0x22:
                    spawned = SpawnPickup(0, part->x >> 8, part->y >> 8);
                    break;
                case 0x20:
                    spawned = SpawnPickup(3, part->x >> 8, part->y >> 8);
                    break;
                case 0x1b:
                    spawned = SpawnPickup(4, part->x >> 8, part->y >> 8);
                    break;
                }
                if (spawned) {
                    u32 m1 = 1;

                    spawned->mode = m1;
                    spawned->visible = 0;
                }
            }
        }
    }
    return 0;
}

