#include "gax_internal.h"

/* Word-wrap text/console-tile renderer used by the fatal-error screen
 * (GaxFatalError): writes `str`'s characters as tile indices into BG
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
 * `tile` is a copy of the character (the ROM maps it in r1 from r6),
 * and each subtraction is truncated back to a byte, which agbcc does in
 * r0 before moving it to r1. The final `else if (tile > 0x40)` (no
 * trailing `else`) is always true there, but the ROM re-tests it (a
 * dead `bls` to the store), so it's kept. */
void GaxDrawText(u32 col, u32 row, const char *str)
{
    const u8 *s = (const u8 *)str;
    u32 base = col + BG_SCREEN_ADDR(0);
    u8 *dst = (u8 *)(col + base + row * 64);
    s32 x;
    s32 len;
    u32 c;
    u32 first;
    u32 peek;
    u32 tile;

    first = *s;
    if (first == 0)
        return;
    do {
        len = 0;
        x = ((u32)dst & 0x3f) >> 1;
        c = *s;
        if (x <= 0x1f) {
            /* The loop is entered at its test, with the byte already
             * read for the outer loop's condition. */
            peek = first;
            goto check;
            for (;;) {
                x++;
                len++;
                if (x > 0x1f)
                    break;
                peek = s[len];
            check:
                if (peek == 0 || peek == ' ' || peek == '\n')
                    break;
                if (x > 0x1d) {
                    dst = (u8 *)(((s32)dst & ~0x3f) + 0x40);
                    break;
                }
            }
        }
        tile = c;
        s++;
        if (tile == '_')
            tile = 0x5d;
        if (tile == ':')
            tile = 0x5c;
        if (tile == '.')
            tile = 0x5b;
        if (tile == '\n')
            dst = (u8 *)(((s32)dst & ~0x3f) + 0x3f);
        if (tile == ' ')
            tile = 0;
        else if (tile <= 0x40)
            tile = (u8)(tile - 0x2f);
        else if (tile > 0x60)
            tile = (u8)(tile - 0x56);
        else if (tile > 0x40)
            tile = (u8)(tile - 0x36);
        *(u16 *)dst = tile;
        dst += 2;
        first = *s;
    } while (first != 0);
}
