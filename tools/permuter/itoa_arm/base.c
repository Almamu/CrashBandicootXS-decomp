#pragma _permuter latedefine start
#pragma _permuter define MATCH_HOLD_REG(T, name, reg) register T name asm(#reg)
#pragma _permuter define MATCH_CONST(v, K) asm("" : "=r"(v) : "0"(K))
#pragma _permuter define BIOS_DIV(num, digit) asm("swi 0x60000" : "=r"(num), "=r"(digit) : "0"(num), "1"(digit) : "r3")
#pragma _permuter latedefine end
typedef unsigned char u8;
typedef int s32;

s32 itoa_arm(s32 value, u8 *buf, s32 base)
{
    PERM_IGNORE(MATCH_HOLD_REG(s32, num, r0);)
    PERM_IGNORE(MATCH_HOLD_REG(s32, digit, r1);)
    PERM_IGNORE(MATCH_HOLD_REG(u8 *, b, r6);)
    PERM_IGNORE(MATCH_HOLD_REG(s32, len, r5);)
    PERM_IGNORE(MATCH_HOLD_REG(s32, neg, r4);)
    PERM_IGNORE(MATCH_HOLD_REG(s32, divisor, ip);)
    PERM_PRETEND(s32 num; s32 digit; u8 *b; s32 len; s32 neg; s32 divisor;)
    PERM_RANDOMIZE(
    s32 j;
    s32 ten;

    MATCH_CONST(len, 0);
    do {
        num = value;
        if (num < 0) {
            neg = 1;
            num = -num;
        } else {
            neg = 0;
        }
        b = buf;
    } while (0);
    divisor = base;
    if (base != 16) {
        do {
            digit = divisor;
            PERM_IGNORE(BIOS_DIV(num, digit);)
            b[len] = digit + '0';
            len++;
        } while (num != 0);
    } else {
        do {
            digit = num & 15;
            num >>= 4;
            if (digit >= (ten = 10))
                digit += 'A' - 10;
            else
                digit += '0';
            b[len] = digit;
            len++;
        } while (num != 0);
    }
    if (neg) {
        neg = '-';
        b[len] = neg;
        len++;
        neg = 0;
    }
    b[len] = neg;
    j = len - 1;
    do {
        u8 t = b[j];
        b[j] = b[neg];
        b[neg] = t;
        neg++;
        j--;
    } while (neg < j);
    return len;
    )
}
