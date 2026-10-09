#include "bg_layer.hpp"

extern "C" {
#include "math_util.h"
#include "globals.h"
}

/* Built with old_agbcp - see docs/matching/archive/game-loop-old-agbcc.md. */

/* BgLayerBase (include/bg_layer.hpp), ROM 0x08024D74-0x08024F24: its
 * destructor, constructor, scroll clamps and the Q8 scale/accumulate step
 * (in src/cutscene/cutscene_player.cpp until #770; the destructor is the
 * key method, so g++ emits gBgLayerBaseVtable here), then its other
 * methods. The collision tile cache's lookups that follow in the ROM are
 * in tile_cache.cpp (here until #770). */

/* Destroys the layer's streamer (a virtual `delete`: slot 1 with 3). */
BgLayerBase::~BgLayerBase()
{
    delete streamer;
}

/* Creates the layer's streamer. `unused` is the BG index the subclass's
 * constructor passes on. */
BgLayerBase::BgLayerBase(s32 unused)
{
    streamer = new BgStreamer;
}

/* Clamps a scroll step to [-0x10, 0x10]. */
s32 BgLayerBase::ClampScrollStep(s32 step)
{
    LIMIT_MIN(step, -0x10);
    LIMIT_MAX(step, 0x10);
    return step;
}

/* Clamps `pos[0]`/`pos[1]` to the scroll limits. UNUSED: no caller
 * anywhere in the ROM. */
void BgLayerBase::ClampScrollMax(s32 *pos)
{
    s32 a = maxX;

    LIMIT_MAX(a, pos[0]);
    pos[0] = a;

    {
        s32 b = maxY;

        LIMIT_MAX(b, pos[1]);
        pos[1] = b;
    }
}

/* Applies the layer's per-axis scale (`scaleX`/`scaleY`) to `vec`,
 * dividing the Q8 product by 256 rounded toward zero (an arithmetic
 * `>> 8` alone rounds negative products toward negative infinity; the
 * `+0xff` bias before it makes it truncate, as `/ 256` would). Called by
 * bg_layer_base.cpp's `Scroll`/`Reset`. */
void BgLayerBase::ScaleScroll(s32 *vec)
{
    s32 v;

    v = vec[0] * scaleX;
    if (v >= 0) {
        v = Q8_TO_INT(v);
    } else {
        v = Q8_TO_INT(v + 0xff);
    }
    vec[0] = v;

    v = vec[1] * scaleY;
    if (v >= 0) {
        v = Q8_TO_INT(v);
    } else {
        v = Q8_TO_INT(v + 0xff);
    }
    vec[1] = v;
}

/* Moves the layer toward `target`, each axis by at most the step
 * ClampScrollStep (virtual) allows. */
void BgLayerBase::StepScroll(const s32 *target)
{
    s32 dx, dy;

    dx = ClampScrollStep(target[0] - x);
    dy = ClampScrollStep(target[1] - y);

    x += dx;
    y += dy;
}

/* Scales the move `delta` by the layer's parallax factors and steps the
 * layer's position toward the result. */
void BgLayerBase::Scroll(const s32 *delta)
{
    s32 scaled[2];
    s32 dx = delta[0];
    s32 dy = delta[1];

    scaled[0] = dx;
    scaled[1] = dy;
    ScaleScroll(scaled);
    StepScroll(scaled);
}

/* Sets the position to `pos` scaled by the parallax factors, and seeds
 * the streamer's window there. */
void BgLayerBase::Reset(const s32 *pos)
{
    s32 px = pos[0];
    s32 py = pos[1];

    x = px;
    y = py;
    ScaleScroll(&x);
    streamer->Fill(&x);
}

/* (Re)initializes the layer from `source`: caches its pixel
 * dimensions/scroll bounds, resets the accumulated position to the
 * origin, and re-populates the streamer from the same descriptor. Does
 * nothing (besides clearing the enabled flag) when `source` is NULL. */
void BgLayerBase::SetSource(const struct level_layer_desc *source)
{
    u8 *readyFlag;
    s32 zero;

    readyFlag = &enabled;
    zero = 0;
    *readyFlag = zero;

    if (source != NULL) {
        s32 w, h;

        w = source->widthTiles;
        widthTiles = w;
        h = source->heightTiles;
        heightTiles = h;

        w <<= 3;
        widthPx = w;
        h <<= 3;
        heightPx = h;
        w -= 0xf0;
        maxX = w;
        h -= 0xa0;
        maxY = h;
        scaleX = source->scaleX;
        scaleY = source->scaleY;
        x = zero;
        y = zero;

        streamer->SetSource(source);
        streamer->Fill(&x);

        *readyFlag = 1;
    }
}

/* The accessors below are UNUSED: no caller anywhere in the ROM. */
u8 BgLayerBase::IsEnabled()
{
    return enabled;
}

s32 BgLayerBase::GetY()
{
    return y;
}

s32 BgLayerBase::GetX()
{
    return x;
}

s32 BgLayerBase::GetHeightTiles()
{
    return heightTiles;
}

s32 BgLayerBase::GetWidthTiles()
{
    return widthTiles;
}

s32 BgLayerBase::GetHeight()
{
    return heightPx;
}

s32 BgLayerBase::GetWidth()
{
    return widthPx;
}
