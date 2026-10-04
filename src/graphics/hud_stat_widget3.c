#include "core.h"
#include "hud.h"

struct hud_pos
{
    s32 x;
    s32 y;
};

/* `struct hud_counter` viewed with the fields these three widgets use. */
struct hud_score
{
    s32 mode;                       /* +0x00 */
    s32 layout_value;               /* +0x04 */
    s32 mode_b;                     /* +0x08 - sub_8027D5C's mode */
    s32 layout_b;                   /* +0x0C */
    s32 mode_a;                     /* +0x10 - sub_8027940's mode */
    s32 layout_a;                   /* +0x14 */
    u8 unk_18[8];
    s32 value_c;                    /* +0x20 */
    s32 value_a;                    /* +0x24 */
    s32 value_b;                    /* +0x28 */
    u8 unk_2C[0xC];
    s32 value_d;                    /* +0x38 */
    s32 value_e;                    /* +0x3C */
    u8 unk_40[4];
    s32 shown_c;                    /* +0x44 */
    s32 shown_a;                    /* +0x48 */
    s32 shown_b;                    /* +0x4C */
    u8 unk_50[0xC];
    s32 shown_d;                    /* +0x5C */
    s32 shown_e;                    /* +0x60 */
    struct hud_digit_part *parts;   /* +0x64 */
};

/* The object `sub_8027E88` reads its percentage from, called through a
 * gcc 2.x pointer-to-member (delta + function) slot in its vtable. */
struct pct_vtable
{
    u8 unk_00[0x30];
    s16 delta;                      /* +0x30 */
    u16 unk_32;
    void *fn;                       /* +0x34 */
};

struct pct_source
{
    u8 unk_00[0x50];
    struct pct_vtable *vtable;      /* +0x50 */
};

extern s32 gUnknown_0300086C;
extern struct pct_source *gUnknown_03000884;
extern s32 _call_via_r1(void *self, void *fn);
extern s32 sub_80233B4(void *state);
extern s32 sub_8031784(void);
extern void sub_8008044(struct hud_digit_part *part);
extern void *gLevelState;
extern struct hud_pos gStaticData_08174C6C[];
extern void sub_80270E0(struct hud_digit_part *part, s32 x, s32 y);
extern s32 sub_8023414(void *state);
extern s32 GetWumpa(void *state);
extern s32 __divsi3(s32 a, s32 b);
extern s32 __modsi3(s32 a, s32 b);

static inline void SetPartPos(s32 x, s32 y, struct hud_digit_part *part)
{
    part->x = x << 8;
    part->y = y << 8;
}

/* Sets the part's desired frame, clamped to its animation's last one. */
#define CLAMP_FRAME(part, index, frame)                                   \
    {                                                                     \
        struct hud_digit_part *_p = (part);                               \
        s32 _f = (frame);                                                 \
        s32 _n = _p->anim_data->records[index].frame_count;               \
        if (_f >= _n)                                                     \
            _f = _n - 1;                                                  \
        _p->frame_index = _f;                                             \
    }

/* The remaining three callees of the HUD stat-widget dispatcher
 * (`sub_80274EC`, `hud_stat_widget.c`) - see `docs/matching/
 * issue-45-hud-stat-widget-dispatcher.md` for the family's full
 * background. Built with old_agbcc, like `hud_stat_widget2.c`.
 *
 * These were parked as NAKED on the belief that a second "r7 wrong-value
 * miscompile" broke every clamp site; under old_agbcc the plain clamp
 * (`CLAMP_FRAME`, part pointer bound before the frame value) reproduces
 * the ROM's `ldrb r7; ...; adds rN, r7, #0` sequence exactly. The other
 * things that mattered: a literal `-1` frame lets the compiler fold the
 * clamp into a bare store (the ROM does that in some branches), while a
 * `-1` held in a local keeps the compare; the "100%" branches read
 * `self->parts` into their own block-local so its register differs from
 * the digit branches (the ROM does not cross-jump them); and
 * `sub_8027940`'s second counter reads value, cached value, then
 * `self->parts`, in that order. */

/* The two-digit score-style counter: two independent 3-digit displays
 * (`self->field_24`/`self->field_48` change-detection pair at slots
 * 0xc0/0x80/0xa0, `self->field_28`/`self->field_4c` pair at slots
 * 0xc0*2/0xe0/0x80*4), each branching 3-digit vs. 2-digit vs. 1-digit
 * (hiding the unused leading slot(s) via a desired frame of -1, exactly
 * like `sub_8027838`'s own single-digit case), plus one more icon
 * (`gStaticData_08174C6C`-positioned, slot at `self->parts + 0xa0*4`)
 * whose x/y table index is picked from a 3-way digit-count check on the
 * first counter's value. */
void sub_8027940(struct hud_counter *selfArg)
{
    struct hud_score *self = (struct hud_score *)selfArg;
    struct hud_digit_part *parts;
    s32 v;
    s32 digits;
    s32 w;
    s32 off;

    if (self->mode_a == 0)
        return;
    self->value_a = sub_8023414(gLevelState);
    if (self->mode_a == 1 || self->mode_a == 3)
        gUnknown_0300086C = self->layout_a * 2 - 0x28;
    else
        gUnknown_0300086C = 0;

    v = self->value_a;
    if (v != self->shown_a)
    {
        if (v > 99)
        {
            s32 f = __divsi3(v, 100);

            parts = self->parts;
            CLAMP_FRAME(&parts[3], parts[3].anim_index, f);
            f = __modsi3(__divsi3(self->value_a, 10), 10);
            CLAMP_FRAME(&parts[4], parts[4].anim_index, f);
            f = __modsi3(self->value_a, 10);
            CLAMP_FRAME(&parts[5], parts[5].anim_index, f);
        }
        else if (v > 9)
        {
            s32 f = __divsi3(v, 10);

            parts = self->parts;
            CLAMP_FRAME(&parts[3], parts[3].anim_index, f);
            f = __modsi3(self->value_a, 10);
            CLAMP_FRAME(&parts[4], parts[4].anim_index, f);
            CLAMP_FRAME(&parts[5], parts[5].anim_index, -1);
        }
        else
        {
            s32 f;

            parts = self->parts;
            CLAMP_FRAME(&parts[3], parts[3].anim_index, v);
            f = -1;
            CLAMP_FRAME(&parts[4], parts[4].anim_index, f);
            CLAMP_FRAME(&parts[5], parts[5].anim_index, f);
        }
    }
    sub_80270E0(&self->parts[3], 0, 0);
    sub_80270E0(&self->parts[4], 0, 0);
    sub_80270E0(&self->parts[5], 0, 0);

    if (self->value_a > 99)
        digits = 2;
    else if (self->value_a > 9)
        digits = 1;
    else
        digits = 0;
    off = digits * 15;

    v = self->value_b;
    w = self->shown_b;
    parts = self->parts;
    if (v != w)
    {
        if (v > 99)
        {
            s32 f = __divsi3(v, 100);

            CLAMP_FRAME(&parts[6], parts[6].anim_index, f);
            f = __modsi3(__divsi3(self->value_b, 10), 10);
            CLAMP_FRAME(&parts[7], parts[7].anim_index, f);
            f = __modsi3(self->value_b, 10);
            CLAMP_FRAME(&parts[8], parts[8].anim_index, f);
        }
        else if (v > 9)
        {
            s32 f = __divsi3(v, 10);

            CLAMP_FRAME(&parts[6], parts[6].anim_index, f);
            f = __modsi3(self->value_b, 10);
            CLAMP_FRAME(&parts[7], parts[7].anim_index, f);
            CLAMP_FRAME(&parts[8], parts[8].anim_index, -1);
        }
        else
        {
            s32 f;

            CLAMP_FRAME(&parts[6], parts[6].anim_index, v);
            f = -1;
            CLAMP_FRAME(&parts[7], parts[7].anim_index, f);
            CLAMP_FRAME(&parts[8], parts[8].anim_index, f);
        }
    }
    sub_80270E0(&parts[6], off, 0);
    sub_80270E0(&self->parts[7], off, 0);
    sub_80270E0(&self->parts[8], off, 0);

    {
        struct hud_digit_part *part;

        SetPartPos(gStaticData_08174C6C[10].x + off, gStaticData_08174C6C[10].y, (part = &self->parts[10]));
        CLAMP_FRAME(part, self->parts[10].anim_index, 0);
        sub_80270E0(part, 0, 0);
    }
    sub_80270E0(&self->parts[9], 0, 0);
    self->shown_b = self->value_b;
    self->shown_a = self->value_a;
}

/* A smaller sibling of `sub_8027940` above: one 2-digit display
 * (`self->field_20`/`self->field_44` change-detection pair, slots
 * `0xb0*4`/`0xc0*4`), sourced from `GetWumpa` (`sub_8027940` used
 * `sub_8023414` for its own primary counter) rather than a mode/layout
 * pair like the dispatcher's other callees - always refreshes one more
 * slot (`self->parts + 0xd0*4`) up front via `sub_8008044`/
 * `sub_80270E0` regardless of whether the value changed. */
void sub_8027D5C(struct hud_counter *selfArg)
{
    struct hud_score *self = (struct hud_score *)selfArg;
    struct hud_digit_part *parts;
    s32 v;

    if (self->mode_b == 0)
        return;
    if (self->mode_b == 1 || self->mode_b == 3)
        gUnknown_0300086C = self->layout_b * 2 - 0x28;
    else
        gUnknown_0300086C = 0;
    self->value_c = GetWumpa(gLevelState);
    sub_8008044(&self->parts[13]);
    sub_80270E0(&self->parts[13], 0, 0);

    v = self->value_c;
    if (v != self->shown_c)
    {
        if (v > 9)
        {
            s32 f = __divsi3(v, 10);

            parts = self->parts;
            CLAMP_FRAME(&parts[11], parts[11].anim_index, f);
            f = __modsi3(self->value_c, 10);
            CLAMP_FRAME(&parts[12], parts[12].anim_index, f);
        }
        else
        {
            parts = self->parts;
            CLAMP_FRAME(&parts[11], parts[11].anim_index, v);
            CLAMP_FRAME(&parts[12], parts[12].anim_index, -1);
        }
    }
    sub_80270E0(&self->parts[11], 0, 0);
    sub_80270E0(&self->parts[12], 0, 0);
    self->shown_c = self->value_c;
}

/* The percentage-counter widget (`docs/rom_map.md`'s "fx" investigation
 * already named it this way from the `cmp r1, #0x64` special case
 * below): a single 3-digit-or-percent display sourced from
 * `_call_via_r1(gUnknown_03000884's own x-position field + a halfword
 * read off a nested struct, y-position field)` rather than any of the
 * mode/layout-value or `sub_80233B4`-family sources the rest of the
 * dispatcher's callees use - this is the only counter in the family
 * driven by something resembling a screen coordinate.
 *
 * A value of exactly 100 (`0x64`) skips the digit split entirely and
 * shows a single dedicated "100%" icon (slot `0xc8*8`, tens/ones slots
 * still get the fixed values `1`/`-1` to hide them). Otherwise the usual
 * 2-digit-vs-1-digit split runs (slots `0xc8*8`/`0xd0*8`/`0xd8*8`,
 * hiding the leading digit via `-1` past `0xe0*8` when unused). Runs a
 * second, independent instance of the same shape right after (guarded
 * by its own `sub_8031784`/`self->field_3c`/`self->field_60`
 * change-detection triple, slots `0xe8*8` fixed-icon plus
 * `0xf0*8`/`0xf8*8`/`0x80<<4`/`0x84<<4` digit slots) - two independent
 * percent-style readouts sharing one function body. */
void sub_8027E88(struct hud_counter *selfArg)
{
    struct hud_score *self = (struct hud_score *)selfArg;
    struct hud_digit_part *parts;
    struct hud_digit_part *part;
    s32 v;

    gUnknown_0300086C = 0;
    SetPartPos(gStaticData_08174C6C[24].x, gStaticData_08174C6C[24].y, (part = &self->parts[24]));
    CLAMP_FRAME(part, self->parts[24].anim_index, 0);
    sub_80270E0(part, 0, 0);

    {
        struct pct_source *src = gUnknown_03000884;
        struct pct_vtable *vt = src->vtable;

        v = _call_via_r1((u8 *)src + vt->delta, vt->fn);
    }
    self->value_d = v;
    if (v != self->shown_d)
    {
        if (v == 100)
        {
            struct hud_digit_part *p = self->parts;

            CLAMP_FRAME(&p[25], p[25].anim_index, 1);
            CLAMP_FRAME(&p[26], p[26].anim_index, 0);
            CLAMP_FRAME(&p[27], p[27].anim_index, 0);
            CLAMP_FRAME(&p[28], p[28].anim_index, 10);
        }
        else if (v > 9)
        {
            s32 f = __divsi3(v, 10);

            parts = self->parts;
            CLAMP_FRAME(&parts[25], parts[25].anim_index, f);
            f = __modsi3(self->value_d, 10);
            CLAMP_FRAME(&parts[26], parts[26].anim_index, f);
            CLAMP_FRAME(&parts[27], parts[27].anim_index, 10);
            CLAMP_FRAME(&parts[28], parts[28].anim_index, -1);
        }
        else
        {
            s32 f;

            parts = self->parts;
            CLAMP_FRAME(&parts[25], parts[25].anim_index, v);
            CLAMP_FRAME(&parts[26], parts[26].anim_index, 10);
            f = -1;
            CLAMP_FRAME(&parts[27], parts[27].anim_index, f);
            CLAMP_FRAME(&parts[28], parts[28].anim_index, f);
        }
    }
    sub_80270E0(&self->parts[25], 0, 0);
    sub_80270E0(&self->parts[26], 0, 0);
    sub_80270E0(&self->parts[27], 0, 0);
    sub_80270E0(&self->parts[28], 0, 0);
    self->shown_d = self->value_d;

    if (sub_80233B4(gLevelState) != -1)
        return;
    if ((self->value_e = sub_8031784()) == -1)
        return;

    SetPartPos(gStaticData_08174C6C[29].x, gStaticData_08174C6C[29].y, (part = &self->parts[29]));
    CLAMP_FRAME(part, self->parts[29].anim_index, 0);
    sub_80270E0(part, 0, 0);

    v = self->value_e;
    if (v != self->shown_e)
    {
        if (v == 100)
        {
            struct hud_digit_part *p = self->parts;

            CLAMP_FRAME(&p[30], p[30].anim_index, 1);
            CLAMP_FRAME(&p[31], p[31].anim_index, 0);
            CLAMP_FRAME(&p[32], p[32].anim_index, 0);
            CLAMP_FRAME(&p[33], p[33].anim_index, 10);
        }
        else if (v > 9)
        {
            s32 f = __divsi3(v, 10);

            parts = self->parts;
            CLAMP_FRAME(&parts[30], parts[30].anim_index, f);
            f = __modsi3(self->value_e, 10);
            CLAMP_FRAME(&parts[31], parts[31].anim_index, f);
            CLAMP_FRAME(&parts[32], parts[32].anim_index, 10);
            f = -1;
            CLAMP_FRAME(&parts[33], parts[33].anim_index, f);
        }
        else
        {
            s32 f;

            parts = self->parts;
            CLAMP_FRAME(&parts[30], parts[30].anim_index, v);
            CLAMP_FRAME(&parts[31], parts[31].anim_index, 10);
            f = -1;
            CLAMP_FRAME(&parts[32], parts[32].anim_index, f);
            CLAMP_FRAME(&parts[33], parts[33].anim_index, f);
        }
    }
    sub_80270E0(&self->parts[30], 0, 0);
    sub_80270E0(&self->parts[31], 0, 0);
    sub_80270E0(&self->parts[32], 0, 0);
    sub_80270E0(&self->parts[33], 0, 0);
    self->shown_e = self->value_e;
}
