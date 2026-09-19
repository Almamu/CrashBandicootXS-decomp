#include "core.h"
#include "gba/defines.h"

/* Word-wrap text/console-tile renderer used by the fatal-error screen
 * (sub_80392E0): writes `str`'s characters as tile indices into BG
 * screen block 0's tilemap, starting at (col, row) in 8x8-tile units
 * (`BG_SCREEN_ADDR(0) + col*2 + row*64`, 64 bytes/row = 32 tiles *
 * 2 bytes each).
 *
 * Before placing each character it scans ahead - up to the rest of the
 * current 32-column line - for the next space/newline/NUL; if the
 * current "word" (the run up to and including that terminator) would
 * still be running past column 29 with none found, the cursor wraps to
 * the start of the next tile row before the character is placed.
 *
 * ASCII is then remapped onto this font's tile order: `'.'`/`':'`/`'_'`
 * are first special-cased onto three tile indices normally reserved for
 * higher ASCII values (0x5b/0x5c/0x5d) so they fall through the same
 * general-purpose bucket below; `'\n'` re-aligns the cursor to the
 * current row's start plus one odd byte (not a clean "next row" - kept
 * exactly as the ROM computes it, not "fixed", since this string
 * renderer's only two real callers never actually feed it a literal
 * newline). The general bucket then maps ' ' -> tile 0, '0'-'9' ->
 * tiles 1-10, 'A'-'Z' -> tiles 11-36 (and anything -0x20 of that, i.e.
 * lowercase, folds onto the same range - this font has no separate
 * lowercase glyphs), and the three remapped punctuation marks above
 * land at tiles 37-39 right after 'Z'.
 *
 * Every local here is register-pinned: this function has no calls at
 * all (nothing to save across), and the ROM keeps every one of `dst`/
 * `colIdx`/`s`/`c`/`peekSeed`/`len`/`mask`/`peek` alive simultaneously
 * across the word-wrap scan, genuinely filling every low register plus
 * `ip` - an unconstrained build reliably picks different registers
 * (verified by removing each pin in turn and rebuilding) even though
 * the generated *shape* is identical either way. `colIdx` does double
 * duty as the ROM's own r1 does: the word-wrap column index while
 * scanning ahead, then (once that's dead) reused for the char being
 * remapped into a tile index - matching the ROM's own register reuse,
 * not a naming accident. The final `else if (colIdx > 0x40)` (no
 * trailing `else`) is provably always true there (the two branches
 * above it already establish `colIdx > 0x40`), but the ROM re-tests it
 * anyway with its own dead `bls`-to-store branch that skips both the
 * subtraction and the shared truncation tail below - kept exactly as
 * the ROM has it rather than simplified to a plain `else`, since that
 * changes gcc's cross-jump merging of the three arithmetic branches'
 * shared `(u8)`-truncation tail. */
void sub_8039214(u32 col, u32 row, const u8 *str)
{
    register u8 *dst asm("r3");
    register s32 colIdx asm("r1");
    register const u8 *s asm("r5");
    register u32 c asm("r6");
    register s32 peekSeed asm("r2");
    register s32 len asm("r4");
    register s32 mask asm("ip");
    u32 base;

    s = str;
    base = col + BG_SCREEN_ADDR(0);
    dst = (u8 *)(col + base + row * 64);

    peekSeed = *s;
    if (peekSeed == 0) {
        return;
    }

    mask = -0x40;

    do {
        const u8 *next;

        len = 0;
        {
            /* Same r0-first routing as everywhere else in this
             * function - the ROM loads the 0x3f mask into r0 and ANDs
             * dst into it there, only shifting into colIdx (r1) as a
             * separate last step. */
            register s32 t asm("r0") = 0x3f;
            t &= (s32)dst;
            colIdx = t >> 1;
        }
        c = s[0];
        next = s + 1;

        if (colIdx <= 0x1f) {
            register s32 peek asm("r0") = peekSeed;

            /* The ROM re-tests `colIdx > 0x1f` at the top of every
             * lookahead iteration (not just once on entry, which the
             * `if` above already covers) - dead in practice since the
             * `colIdx > 0x1d` break below always fires first, but kept
             * exactly as a `goto`-driven first-iteration skip to match
             * gcc's own loop-rotated codegen byte-for-byte. */
            goto term_check;
            for (;;) {
                colIdx++;
                len++;
                if (colIdx > 0x1f) {
                    break;
                }
                peek = s[len];
term_check:
                if (peek == 0 || peek == ' ' || peek == '\n') {
                    break;
                }
                if (colIdx > 0x1d) {
                    /* `dst &= mask;` alone lets gcc's copy propagation
                     * compute the AND directly into r3 (`and r3,r3,r0`)
                     * - the ROM instead routes it through r0 first
                     * (`mov r0,ip; ands r0,r3; adds r3,r0,#0`); `dst`
                     * is genuinely modified by this, so it's an output
                     * operand rather than just a clobber. */
                    asm volatile("mov r0, %1\n\t"
                                 "and r0, %0\n\t"
                                 "add %0, r0, #0"
                                 : "+r"(dst)
                                 : "r"(mask)
                                 : "r0");
                    dst += 0x40;
                    break;
                }
            }
        }

        colIdx = c;
        s = next;

        if (colIdx == '_') {
            colIdx = 0x5d;
        }
        if (colIdx == ':') {
            colIdx = 0x5c;
        }
        if (colIdx == '.') {
            colIdx = 0x5b;
        }
        if (colIdx == '\n') {
            /* Same "route the AND through r0 first" quirk as the
             * word-wrap case above. */
            asm volatile("mov r0, %1\n\t"
                         "and r0, %0\n\t"
                         "add %0, r0, #0"
                         : "+r"(dst)
                         : "r"(mask)
                         : "r0");
            dst += 0x3f;
        }

        /* Each arithmetic branch below computes into `tmp` (r0), a
         * genuinely different register from `colIdx` (r1) - the ROM
         * always copies `colIdx` into r0 before subtracting there,
         * then truncates back into r1 on the way out, rather than ever
         * subtracting r1 in place. */
        if (colIdx == ' ') {
            colIdx = 0;
        } else if ((u32)colIdx <= 0x40) {
            register s32 tmp asm("r0") = colIdx - 0x2f;
            colIdx = (u8)tmp;
        } else if ((u32)colIdx > 0x60) {
            register s32 tmp asm("r0") = colIdx - 0x56;
            colIdx = (u8)tmp;
        } else if ((u32)colIdx > 0x40) {
            register s32 tmp asm("r0") = colIdx - 0x36;
            colIdx = (u8)tmp;
        }

        *(u16 *)dst = colIdx;
        dst += 2;

        peekSeed = *s;
    } while (peekSeed != 0);
}
asm(".align 2, 0");
