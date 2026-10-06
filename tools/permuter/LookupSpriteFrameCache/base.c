typedef unsigned char u8;
typedef unsigned int u32;
typedef int s32;

struct sprite_frame_cache_node {
    struct sprite_frame_cache_node *next;
    struct sprite_frame_cache_node *prev;
    u8 *frame;
    void *vramAddr;
};

extern struct sprite_frame_cache_node gSpriteFrameCacheCurrent;
extern struct sprite_frame_cache_node gSpriteFrameCachePrevious;

static inline s32 ObjTileIndex(u32 vramAddr)
{
    vramAddr -= 0x07000000;
    vramAddr += 0xFF0000;
    return vramAddr >> 5;
}

s32 LookupSpriteFrameCache(u8 *frame)
{
    struct sprite_frame_cache_node *node;
    PERM_GENERAL(, s32 r;)

    PERM_GENERAL(
    for (node = gSpriteFrameCacheCurrent.next; node != &gSpriteFrameCacheCurrent; node = node->next) {
        if (node->frame == frame)
            PERM_GENERAL(return ObjTileIndex((u32)node->vramAddr);, goto hit1;, goto out;, { r = ObjTileIndex((u32)node->vramAddr); goto done; })
    }
    ,
    node = gSpriteFrameCacheCurrent.next;
    while (node != &gSpriteFrameCacheCurrent) {
        if (node->frame == frame)
            PERM_GENERAL(return ObjTileIndex((u32)node->vramAddr);, goto hit1;, goto out;, { r = ObjTileIndex((u32)node->vramAddr); goto done; })
        node = node->next;
    }
    ,
    node = gSpriteFrameCacheCurrent.next;
    if (node != &gSpriteFrameCacheCurrent) {
        do {
            if (node->frame == frame)
                PERM_GENERAL(return ObjTileIndex((u32)node->vramAddr);, goto hit1;, goto out;, { r = ObjTileIndex((u32)node->vramAddr); goto done; })
            node = node->next;
        } while (node != &gSpriteFrameCacheCurrent);
    }
    )
    for (node = gSpriteFrameCachePrevious.next; node != &gSpriteFrameCachePrevious; node = node->next) {
        if (node->frame == frame) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            node->prev = &gSpriteFrameCacheCurrent;
            node->next = gSpriteFrameCacheCurrent.next;
            gSpriteFrameCacheCurrent.next->prev = node;
            gSpriteFrameCacheCurrent.next = node;
            PERM_GENERAL(return ObjTileIndex((u32)node->vramAddr);, goto out;, { r = ObjTileIndex((u32)node->vramAddr); goto done; })
        }
    }
    PERM_GENERAL(return -1;, { r = -1; goto done; })
    PERM_GENERAL(, hit1: return ObjTileIndex((u32)node->vramAddr);)
    PERM_GENERAL(, out: return ObjTileIndex((u32)node->vramAddr);)
    PERM_GENERAL(, done: return r;)
}
