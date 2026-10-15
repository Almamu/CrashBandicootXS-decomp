#ifndef GUARD_GBA_DEFINES
#define GUARD_GBA_DEFINES

#ifndef GEN_CTX
#include <stddef.h>
#include <stdint.h> // for uint16_t
#endif

#define TRUE  1
#define FALSE 0

#define IWRAM_DATA __attribute__((section("iwram_data")))
#define EWRAM_DATA __attribute__((section("ewram_data")))

// TODO: Move these elsewhere?
// #ifdef __GNUC__
#define NAKED __attribute__((naked))
#define UNUSED __attribute__((unused))
//#define PACKED __attribute__((packed))
#define ALIGNED(n) __attribute__((aligned(n)))
// #endif

#define EWRAM_SIZE      0x40000
#define IWRAM_SIZE      0x7E00
#define PLTT_SIZE       0x400
#define BG_PLTT_SIZE    0x200
#define OBJ_PLTT_SIZE   0x200
#define PALETTE_SIZE_16 0x20 // one 16-colour palette
#define VRAM_SIZE       0x18000
#define BG_VRAM_SIZE    0x10000
#define BG_SCREEN_SIZE  0x800 // one screen block (BG_SCREEN_ADDR)
// The size of OBJ_VRAM0_SIZE depends on the "Mode" of the graphics hardware.
// The data is interpreted differently by the hardware in each mode.
#define OBJ_VRAM0_SIZE  0x8000
#define OBJ_VRAM1_SIZE  0x4000
#define OAM_ENTRY_COUNT 128

// A screen coordinate written into an OAM entry: attribute 1's 9-bit X
// and attribute 0's 8-bit Y (#979). The game code never truncates a
// screen coordinate itself; where the ROM masks one before packing it
// into the attributes, the mask is spelled with these, so a platform
// whose OAM has wider fields can define them as no-ops in its own
// gba/defines.h (docs/platforms.md).
#define OAM_X(x) ((x) & 0x1FF)
#define OAM_Y(y) ((y) & 0xFF)

// The GBA's screen. A build can define another size (the Makefile's
// `screen-check`, #979, docs/screen.md): that build only has to compile;
// only 240x160 matches.
#ifndef DISPLAY_WIDTH
#define DISPLAY_WIDTH  240
#endif
#ifndef DISPLAY_HEIGHT
#define DISPLAY_HEIGHT 160
#endif

// NOTE(Jace): I tried replacing these altogether,
//             but that resulted in a nonmatching ROM
//             (see notes above)
#define EWRAM_START 0x02000000
#define IWRAM_START 0x03000000

// The BIOS's words at the top of IWRAM, as offsets from IWRAM_START.
#define IWRAM_SOUND_INFO_OFFSET  0x7FF0
#define IWRAM_INTR_CHECK_OFFSET  0x7FF8
#define IWRAM_INTR_VECTOR_OFFSET 0x7FFC

#define SOUND_INFO_PTR (*(struct SoundMixerState **)(IWRAM_START + IWRAM_SOUND_INFO_OFFSET))
#define INTR_CHECK     (*(u16 *)(IWRAM_START + IWRAM_INTR_CHECK_OFFSET))
#define INTR_VECTOR    (*(void **)(IWRAM_START + IWRAM_INTR_VECTOR_OFFSET))

#define PLTT         0x5000000
#define BG_PLTT      PLTT
#define OBJ_PLTT      (PLTT + BG_PLTT_SIZE)

#define VRAM      0x6000000

#define BG_VRAM           VRAM
#define BG_CHAR_ADDR(n)   (BG_VRAM + ((n) << 14))
#define BG_CHAR_ADDR_FROM_BGCNT(bg)   ((u8*)BG_VRAM + ((gBgCntRegs[bg] & BGCNT_CHARBASE(0x3)) << 12))
// TODO: Maybe rename BG_SCREEN_ADDR
#define BG_SCREEN_ADDR(n) (BG_VRAM + (0x800 * (n)))
#define BG_TILE_ADDR(n)   (BG_VRAM + (0x80 * (n)))

// text-mode BG
#define OBJ_VRAM0      (u8*)(VRAM + 0x10000)

// bitmap-mode BG
#define OBJ_VRAM1      (u8*)(VRAM + 0x14000)

#define OAM      0x7000000
#define OAM_SIZE (OAM_ENTRY_COUNT*sizeof(OamData))

#define WIN_REG_SIZE 2
#define WIN_RANGE(a, b) (((a) << 8) | (b))
#define WIN_GET_LOWER(win_reg)  (((win_reg) & 0xFF00) >> 8)
#define WIN_GET_HIGHER(win_reg) (((win_reg) & 0x00FF) >> 0)
typedef uint16_t winreg_t;

#define TILE_SIZE_4BPP 32
#define TILE_SIZE_8BPP 64

// NOTE/TODO: Maybe this should somewhere else?
// NOTE: This appears to not match when using pointers, but with integers it's fine.
//       uintptr_t should always be defined to be as big as a pointer, so there should be no issues.
#define GET_TILE_NUM_COMMON(vramPtr, tileSize) (((uintptr_t)(vramPtr) - (uintptr_t)OBJ_VRAM0) / (tileSize))
#define GET_TILE_NUM(vramPtr) GET_TILE_NUM_COMMON((vramPtr), TILE_SIZE_4BPP)

#define TOTAL_OBJ_TILE_COUNT 1024

#define RGB16(r, g, b) ((r) | ((g) << 5) | ((b) << 10))
#define RGB16_REV(r, g, b) (((b) << 10) | ((g) << 5) | (r))

#define RGB_BLACK RGB16(0, 0, 0)
#define RGB_WHITE RGB16(31, 31, 31)
#define RGB_RED RGB16(31, 0, 0)
#define RGB_GREEN RGB16(0, 31, 0)
#define RGB_BLUE RGB16(0, 0, 31)
#define RGB_YELLOW RGB16(31, 31, 0)
#define RGB_MAGENTA RGB16(31, 0, 31)
#define RGB_CYAN RGB16(0, 31, 31)
#define RGB_WHITEALPHA (RGB_WHITE | 0x8000)

#define SYSTEM_CLOCK           (16 * 1024 * 1024)   // System Clock

// CpuSet (BIOS swi 0x0B) control-word bits; the low 21 bits are the unit count
#define CPU_SET_SRC_FIXED 0x01000000
#define CPU_SET_16BIT     0x00000000
#define CPU_SET_32BIT     0x04000000
#define CPU_SET_COUNT_MASK 0x1FFFFF // the unit count (bits 0-20)

#endif // GUARD_GBA_DEFINES
