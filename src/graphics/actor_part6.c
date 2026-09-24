#include "core.h"
#include "actor.h"

extern void *gUnknown_03001308;

/* If `gUnknown_03001308+0x2b` is nonzero, returns
 * `(gUnknown_03001308's sub-object)+0x34`'s low 2 bits minus 1;
 * otherwise returns those same low 2 bits unmodified. Same
 * `gUnknown_03001308` sub-object convention used throughout this ROM
 * region (see `sub_8007F78`/`sub_8006FE4`). */
s32 sub_8008408(void)
{
    if (*((u8 *)gUnknown_03001308 + 0x2b) == 0) {
        void *subObj = *(void **)((u8 *)gUnknown_03001308 + 0x10);
        u8 byte2 = *((u8 *)subObj + 0x34);
        u32 result = ((u32)byte2 << 30) >> 30;
        return result;
    } else {
        void *subObj = *(void **)((u8 *)gUnknown_03001308 + 0x10);
        u8 byte2 = *((u8 *)subObj + 0x34);
        u32 result = ((u32)byte2 << 30) >> 30;
        return result - 1;
    }
}

extern void *sub_8026EDC(s32 size);
extern struct actor *sub_800725C(struct actor *self);
extern void sub_8007AB4(void *arg0);
extern u8 gStaticData_087E3C44[];

/* Allocates a new `struct actor`-shaped object (`sub_8026EDC`),
 * initializes it via `sub_800725C` (already matched in graphics.c -
 * wires up `gStaticData_087E3BEC` and clears flags), then overwrites
 * its table with `gStaticData_087E3C44` instead and clears its
 * part-object fields via `sub_8007AB4` (already matched in
 * actor_part.c). `arg0` becomes `field_08`, `arg1`/`arg2` become the
 * Q8 `x`/`y` position. */
struct actor *sub_8008434(u16 arg0, u16 arg1, u16 arg2)
{
    struct actor *part = sub_8026EDC(0x40);

    sub_800725C(part);
    part->table = gStaticData_087E3C44;
    sub_8007AB4(part);
    part->field_08 = arg0;
    part->x = (s32)arg1 << 8;
    part->y = (s32)arg2 << 8;
    return part;
}

/* Always-true stub. */
s32 sub_8008480(void)
{
    return 1;
}

extern void sub_8026ED0(void *arg0);
extern u8 gStaticData_087E3BEC[];

/* Same `gStaticData_087E3BEC`/conditional-`sub_8026ED0` shape as
 * `sub_80073BC` (already matched in `graphics.c`). */
void sub_8008484(struct actor *self, u32 arg1)
{
    self->table = gStaticData_087E3BEC;
    if (arg1 & 1) {
        sub_8026ED0(self);
    }
}

/* Same `sub_800725C`/table-swap/`sub_8007AB4` shape as `sub_8008434`
 * above, but re-initializes an existing `self` instead of allocating
 * a new one. */
struct actor *sub_80084A4(struct actor *self)
{
    sub_800725C(self);
    self->table = gStaticData_087E3C44;
    sub_8007AB4(self);
    return self;
}

extern void *sub_80083B8(void *part);
extern u8 gStaticData_0816B300[];

/* Looks up `part`'s keyframe record via `sub_80083B8` (already parked
 * as `NON_MATCHING` in `actor_part5.c`), then picks a pointer off it
 * per the record's `+4` byte's upper nibble: 0 -> `info+0x24`, 6 ->
 * `info+0x14`, anything else (1-5, or above 6) -> the fixed fallback
 * table `gStaticData_0816B300`. Needed the case labels scattered
 * out of numeric order (rather than grouped into the obvious
 * contiguous "0 / 1-5 / 6" ranges) to get gcc to emit a real jump
 * table instead of a compare chain - this compiler only builds a
 * jump table when the case-to-block mapping can't be expressed as a
 * few simple range checks, so a source-level shape that *looks*
 * needlessly scattered is what is needed to match the ROM's own
 * jump table here. */
void *sub_80084C4(void *part)
{
    void *info = sub_80083B8(part);
    u8 type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    void *result;

    switch (type) {
    case 0:
        result = (u8 *)info + 0x24;
        break;
    case 3:
    case 4:
        result = gStaticData_0816B300;
        break;
    case 1:
    case 2:
        result = gStaticData_0816B300;
        break;
    case 5:
        result = gStaticData_0816B300;
        break;
    case 6:
        result = (u8 *)info + 0x14;
        break;
    default:
        result = gStaticData_0816B300;
        break;
    }
    return result;
}

extern u8 gStaticData_0816B2F8[];

/* Same `sub_80083B8`-derived-record-nibble-switch shape as
 * `sub_80084C4` above, with a different result mapping: 0 and 4
 * select `info+0x1c`, anything else falls back to
 * `gStaticData_0816B2F8`. Unlike `sub_80084C4`, no case-scattering
 * trick was needed here - 0 and 4 are already non-adjacent, which is
 * enough on its own to make gcc emit a jump table instead of a
 * compare chain. */
void *sub_8008518(void *part)
{
    void *info = sub_80083B8(part);
    u8 type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    void *result;

    switch (type) {
    case 0:
        result = (u8 *)info + 0x1c;
        break;
    case 1:
    case 2:
    case 3:
        result = gStaticData_0816B2F8;
        break;
    case 4:
        result = (u8 *)info + 0x1c;
        break;
    case 5:
    case 6:
        result = gStaticData_0816B2F8;
        break;
    default:
        result = gStaticData_0816B2F8;
        break;
    }
    return result;
}

/* Same `sub_80083B8`-derived-record-nibble `switch` shape again, with
 * the exact same case-to-block mapping as `sub_8007C30` (already
 * matched in `actor_part2.c`) - 0/3/4 select `info+0x14`, 5 selects
 * `info+0xc`, and 1/2/6/anything-above-6 fall back to
 * `gStaticData_0816B2F8`. That mapping is non-contiguous on its own,
 * so plain ascending case order was enough for a jump table here too,
 * no scattering needed. */
void *sub_8008564(void *part)
{
    void *info = sub_80083B8(part);
    u8 type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    void *result;

    switch (type) {
    case 0:
    case 3:
    case 4:
        result = (u8 *)info + 0x14;
        break;
    case 1:
    case 2:
    case 6:
        result = gStaticData_0816B2F8;
        break;
    case 5:
        result = (u8 *)info + 0xc;
        break;
    default:
        result = gStaticData_0816B2F8;
        break;
    }
    return result;
}

/* Same `sub_80083B8`-derived-record-nibble `switch` shape once more -
 * 0/2/3/4/6 select `info+0xc`, 1/5/anything-above-6 fall back to
 * `gStaticData_0816B2F8`. */
void *sub_80085B8(void *part)
{
    void *info = sub_80083B8(part);
    u8 type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    void *result;

    switch (type) {
    case 0:
        result = (u8 *)info + 0xc;
        break;
    case 1:
        result = gStaticData_0816B2F8;
        break;
    case 2:
    case 3:
    case 4:
        result = (u8 *)info + 0xc;
        break;
    case 5:
        result = gStaticData_0816B2F8;
        break;
    case 6:
        result = (u8 *)info + 0xc;
        break;
    default:
        result = gStaticData_0816B2F8;
        break;
    }
    return result;
}

/* Same keyframe-record lookup used throughout this ROM region (see
 * `sub_8008394`) - `part`'s `+0x20` table pointer dereferenced twice,
 * indexed by the `+0x2d` frame index, times the record size (0x1c). */
void *sub_8008604(struct actor *part)
{
    register void **tablePtr asm("r2") = *(void ***)((u8 *)part + 0x20);
    register u8 idx asm("r3") = *((u8 *)part + 0x2d);
    s32 offset = idx * 0x1c;
    void *table = *tablePtr;
    return (u8 *)table + offset;
}

/* Clamps `frame` to `part`'s current keyframe record's duration
 * (`+0x16`) minus one if it's out of range, then stores the result
 * into `part+0x30` (the frame index also read/written by
 * `sub_80083B8`). Needed explicit register pins on the whole
 * tablePtr/idxAddr/table/idx chain to get the ROM's `r5` (rather than
 * a tighter, naturally-reused register) - `idx` genuinely outlives
 * `table`'s own register here. The final `rec = table + offset` add
 * also hit the resistant "which operand goes first" canonicalization
 * documented at length for `sub_8008188`/`sub_8008200`/
 * `sub_8008278`/`sub_80083B8` - but unlike those (which were inside a
 * `switch` and had to be parked to avoid breaking case-block merging),
 * this function has no such constraint, so a one-instruction inline
 * `asm` anchor for just this add gets a fully byte-exact match. */
void sub_8008618(struct actor *part, s32 frame)
{
    register void **tablePtr asm("r0") = *(void ***)((u8 *)part + 0x20);
    register u8 *idxAddr asm("r2") = (u8 *)part + 0x2d;
    register void *table asm("r1") = *tablePtr;
    register u8 idx asm("r5") = *idxAddr;
    register s32 offset asm("r0") = idx * 0x1c;
    void *rec;

    asm("add %0, %0, %1" : "+r" (offset) : "r" (table));
    rec = (void *)offset;

    {
        u8 duration = *((u8 *)rec + 0x16);

        if (frame >= duration) {
            frame = duration - 1;
        }
        *(s32 *)((u8 *)part + 0x30) = frame;
    }
}

/* `part+0x25` accessor pair - plain byte get/set, no other logic. */
u8 sub_8008640(void *part)
{
    return *((u8 *)part + 0x25);
}

void sub_8008648(void *part, u8 val)
{
    *((u8 *)part + 0x25) = val;
}

/* `part+0xd` bit-2 getter. */
s32 sub_8008650(void *part)
{
    return (*((u8 *)part + 0xd) >> 2) & 1;
}

/* Toggles `part+0xd` bit 2. Needed the bit-flip (`(byte>>2)^1)&1`)
 * done via genuinely separate `eor`+`and` instructions instead of the
 * single `bic` this compiler normally folds that pattern into -
 * forced via a two-instruction inline `asm` block, whose "one" input
 * also needed marking `+r` (read-write) even though its value never
 * changes, purely to stop the compiler from constant-propagating its
 * value 1 past the asm block and computing the later mask (`-5`) as
 * `1 - 6` off of it instead of the ROM's fresh `movs r1, #5; negs r1,
 * r1`. Also needed the shifted-bit computed before (not after) the
 * mask, matching the ROM's own instruction order. */
void sub_800865C(void *part)
{
    register u32 byte asm("r3") = *((u8 *)part + 0xd);
    register u32 shifted asm("r2") = byte >> 2;
    register u32 one asm("r1") = 1;
    register u32 bit asm("r2");
    register u32 shiftedBit asm("r2");
    register s32 mask asm("r1");
    register s32 result asm("r1");

    asm("eor %0, %0, %2\n\tand %0, %0, %2" : "=r" (bit), "+r" (one) : "1" (one), "0" (shifted));
    shiftedBit = bit << 2;

    mask = -5;
    result = mask & byte;
    result |= shiftedBit;
    *((u8 *)part + 0xd) = result;
}

/* `part+0xd` bit-3 getter - same shape as `sub_8008650` above, one
 * bit over. */
s32 sub_8008674(void *part)
{
    return (*((u8 *)part + 0xd) >> 3) & 1;
}

/* Clears `part+0xd` bit 3. Needed the mask register-pinned to a
 * literal `-9` (computed via `movs r1, #9; negs r1, r1`, same
 * `-(N+1) == ~N` trick as `sub_800865C`'s `-5` mask above) instead of
 * `~8`, which this compiler folds directly into a single `mov #0xf7`
 * immediate load. */
void sub_8008680(void *part)
{
    register s32 mask asm("r1") = -9;
    register s32 byte asm("r2") = *((u8 *)part + 0xd);
    register s32 result asm("r1");

    result = mask & byte;
    *((u8 *)part + 0xd) = result;
}

/* Sets `part+0xd` bit 3. Needed the mask register-pinned and computed
 * before the byte load (matching the ROM's own instruction order) -
 * the natural allocation loads the byte first. Same accumulator-
 * register pattern used for every AND/OR accessor below. */
void sub_800868C(void *part)
{
    register s32 mask asm("r1") = 8;
    register s32 byte asm("r2") = *((u8 *)part + 0xd);
    register s32 result asm("r1");

    result = mask | byte;
    *((u8 *)part + 0xd) = result;
}

/* `part->flags` bit-6 getter. */
s32 sub_8008698(struct actor *part)
{
    return (part->flags >> 6) & 1;
}

/* Clears `part->flags` bit 6. */
void sub_80086A4(struct actor *part)
{
    register s32 mask asm("r1") = -0x41;
    register s32 byte asm("r2") = part->flags;
    register s32 result asm("r1");

    result = mask & byte;
    part->flags = result;
}

/* Sets `part->flags` bit 6. */
void sub_80086B0(struct actor *part)
{
    register s32 mask asm("r1") = 0x40;
    register s32 byte asm("r2") = part->flags;
    register s32 result asm("r1");

    result = mask | byte;
    part->flags = result;
}

/* Resets `part`'s frame index (`+0x2d`) to 0. */
void sub_80086BC(void *part)
{
    *((u8 *)part + 0x2d) = 0;
}

/* `part->flags` bit-7 getter - no mask needed since the shift already
 * leaves only that bit in position 0 of an 8-bit value. */
s32 sub_80086C4(struct actor *part)
{
    return part->flags >> 7;
}

/* Clears `part->flags` bit 7. */
void sub_80086CC(struct actor *part)
{
    register s32 mask asm("r1") = 0x7f;
    register s32 byte asm("r2") = part->flags;
    register s32 result asm("r1");

    result = mask & byte;
    part->flags = result;
}

/* Sets `part->flags` bit 7. */
void sub_80086D8(struct actor *part)
{
    register s32 mask asm("r1") = 0x80;
    register s32 byte asm("r2") = part->flags;
    register s32 result asm("r1");

    result = mask | byte;
    part->flags = result;
}

/* `part+0x2c` byte get/set pair. */
u8 sub_80086E4(void *part)
{
    return *((u8 *)part + 0x2c);
}

void sub_80086EC(void *part, u8 val)
{
    *((u8 *)part + 0x2c) = val;
}

/* Sets `part+0x28` bit 4 to `value & 1`. Needed the low-bit extraction
 * done via a two-instruction inline `asm` AND (rather than this
 * compiler's own `& 1`, which produces the same result but as three
 * instructions once the u8 parameter's mandatory entry truncation is
 * folded in) - and, as with `sub_800865C`, the "1" input needed
 * marking `+r` to stop the mask constant `-0x11` from being computed
 * relative to that leftover register value instead of freshly. */
void sub_80086F4(void *part, u8 value)
{
    register s32 truncVal asm("r1") = value;
    register u8 *addr asm("r0") = (u8 *)part + 0x28;
    register s32 one asm("r2") = 1;
    register s32 shiftedBit asm("r1");
    register s32 mask asm("r2");
    register s32 byte asm("r3");
    register s32 result asm("r2");

    asm("and %0, %0, %1" : "+r" (truncVal), "+r" (one));
    shiftedBit = truncVal << 4;
    mask = -0x11;
    byte = *addr;
    result = mask & byte;
    result |= shiftedBit;
    *addr = result;
}

/* Same shape as `sub_80086F4` immediately above, sets `part+0x28` bit
 * 5 instead. */
void sub_8008710(void *part, u8 value)
{
    register s32 truncVal asm("r1") = value;
    register u8 *addr asm("r0") = (u8 *)part + 0x28;
    register s32 one asm("r2") = 1;
    register s32 shiftedBit asm("r1");
    register s32 mask asm("r2");
    register s32 byte asm("r3");
    register s32 result asm("r2");

    asm("and %0, %0, %1" : "+r" (truncVal), "+r" (one));
    shiftedBit = truncVal << 5;
    mask = -0x21;
    byte = *addr;
    result = mask & byte;
    result |= shiftedBit;
    *addr = result;
}

/* `part+0x38` ("done" flag, also read/written by `sub_80083B8`)
 * setter. */
void sub_800872C(void *part, u8 val)
{
    *((u8 *)part + 0x38) = val;
}

/* Same keyframe-record lookup used throughout this ROM region (see
 * `sub_8008394`/`sub_8008604`), returning the record's `+0x14` byte
 * instead of the record pointer itself. The final `rec = table +
 * offset` add hit the same resistant operand-order gap as
 * `sub_8008618` - fixed the same way, with a one-instruction inline
 * `asm` anchor. */
u8 sub_8008734(struct actor *part)
{
    register void **tablePtr asm("r1") = *(void ***)((u8 *)part + 0x20);
    register u8 *idxAddr asm("r0") = (u8 *)part + 0x2d;
    register void *table asm("r2") = *tablePtr;
    register u8 idx asm("r3") = *idxAddr;
    register s32 offset asm("r1") = idx * 0x1c;
    void *rec;

    asm("add %0, %0, %1" : "+r" (offset) : "r" (table));
    rec = (void *)offset;
    return *((u8 *)rec + 0x14);
}

/* `part+0x29` low-nibble getter. */
s32 sub_8008748(void *part)
{
    u32 byte = *((u8 *)part + 0x29);
    return (byte << 0x1c) >> 0x1c;
}

/* Sets `part+0x29`'s low nibble to `value & 0xf`. Needed the
 * parameter typed `s32` rather than `u8` - the `& 0xf` mask on a `u8`-
 * typed parameter compiles to a much longer defensive shift-based
 * sequence in this compiler (confirmed in isolation), which the ROM
 * doesn't have. The mask constant also needed the same `+r`-on-the-
 * other-operand fix as `sub_800865C`/`sub_80086F4` to stop it being
 * computed relative to the leftover "0xf" register value. */
void sub_8008754(void *part, s32 value)
{
    register u8 *addr asm("r0") = (u8 *)part + 0x29;
    register s32 value_ asm("r1") = value;
    register s32 fifteen asm("r2") = 0xf;
    register s32 lowNibble asm("r1");
    register s32 mask asm("r2");
    register s32 byte asm("r3");
    register s32 result asm("r2");

    asm("and %0, %0, %1" : "+r" (value_), "+r" (fifteen));
    lowNibble = value_;

    mask = -0x10;
    byte = *addr;
    result = mask & byte;
    result |= lowNibble;
    *addr = result;
}

/* `part+0x20` table-pointer get/set pair. */
void sub_8008768(void *part, void *val)
{
    *(void **)((u8 *)part + 0x20) = val;
}

void *sub_800876C(void *part)
{
    return *(void **)((u8 *)part + 0x20);
}

/* Same keyframe-record lookup as `sub_8008734` above, testing the
 * record's `+0x17` flags bit 1 and returning it as a plain 0/1 value.
 * Matched after the NAKED transcription this function briefly used
 * (see git history and docs/matching.md's "Parked, not matched:
 * sub_8008770" entry for that account): every instruction here
 * matches the ROM up through the `ands r0, r1` on its own, but the
 * ROM's two trailing byte-truncation instructions (`lsls r0, r0,
 * #0x18; lsrs r0, r0, #0x18`, narrowing the AND result to the `u8`
 * return type) got optimized away by this compiler every time it
 * could prove the AND result (mask is the visible constant 2) already
 * fits in a byte. Closed with an empty `asm volatile("" : "+r"(test))`
 * barrier right after the `and`, making `test`'s value opaque to the
 * optimizer so it can no longer prove the automatic `s32`-to-`u8`
 * return-value truncation is redundant - the barrier itself emits no
 * instructions, it just forces the *existing* implicit truncation
 * back in. An explicit asm block emitting the shift pair directly was
 * tried first and also produced byte-exact output up through those
 * two instructions, but always duplicated them (the compiler still
 * inserted its own separate return-value truncation afterward,
 * regardless of whether the asm's output was typed `s32` or `u8`) -
 * the empty-barrier form avoids that by leaving the actual truncation
 * to the compiler's own return-conversion codegen. */
u8 sub_8008770(struct actor *part)
{
    register void **tablePtr asm("r1") = *(void ***)((u8 *)part + 0x20);
    register u8 *idxAddr asm("r0") = (u8 *)part + 0x2d;
    register void *table asm("r2") = *tablePtr;
    register u8 idx asm("r3") = *idxAddr;
    register s32 offset asm("r1") = idx * 0x1c;
    void *rec;
    register s32 mask asm("r0");
    register s32 flags asm("r1");
    register s32 test asm("r0");

    asm("add %0, %0, %1" : "+r" (offset) : "r" (table));
    rec = (void *)offset;

    mask = 2;
    flags = *((u8 *)rec + 0x17);
    test = mask & flags;
    asm volatile("" : "+r" (test));
    return test;
}
asm(".align 2, 0");
