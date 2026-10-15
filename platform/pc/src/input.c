/* The pc platform's input: REG_KEYINPUT, scripted, and the checkpoints.
 *
 * The game reads REG_KEYINPUT in one place, KeyInput::Update
 * (key_input.cpp), once a game frame; the platform's gba/io_reg.h makes
 * that read a call of HostReadKeys. So a game frame here is what it is in
 * tools/boottest.py, which sets the keys at a breakpoint right after that
 * read: frame 1 is the first read. Each frame:
 *
 * - the keys are the key file's (--keys) for that frame;
 * - every --checkpoint-every frames (60, boottest's CHECKPOINT_EVERY), a
 *   checkpoint: the SHA-1 of palette RAM, VRAM and OAM, in that order, as
 *   boottest hashes mGBA's, and the picture hash, the same with the OAM
 *   bits the display ignores cleared (boottest's picture_hash: the game
 *   leaves some as its stack's garbage), printed as `checkpoint FRAME HASH
 *   PICTURE` (their first 16 hex digits) and, with --dump DIR, those bytes
 *   in DIR/FRAME.bin;
 * - after --frames N frames the run ends (`done at frame N+1`, as
 *   boottest ends when the script has no keys for a frame).
 *
 * The key file: a line `FRAME KEYS` holds KEYS from that frame until the
 * next line's. KEYS is `-` (none), a number (C syntax, the active-high
 * bits of REG_KEYINPUT) or key names joined by `+`: A B SELECT START RIGHT
 * LEFT UP DOWN R L. `#` starts a comment. Without a key file no key is
 * ever held. */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "gba/gba.h"
#include "host_platform.h"

struct KeyRun {
    u32 frame;
    u16 keys;
};

static struct KeyRun *sRuns;
static u32 sRunCount;
static u32 sRun;
static u32 sFrame;

static const struct {
    const char *name;
    u16 bit;
} sKeyNames[] = {
    { "A", A_BUTTON },         { "B", B_BUTTON },       { "SELECT", SELECT_BUTTON },
    { "START", START_BUTTON }, { "RIGHT", DPAD_RIGHT }, { "LEFT", DPAD_LEFT },
    { "UP", DPAD_UP },         { "DOWN", DPAD_DOWN },   { "R", R_BUTTON },
    { "L", L_BUTTON },
};

static int ParseKeys(const char *text, u16 *keys)
{
    char buf[128];
    char *tok;
    char *save;
    char *end;
    u32 i;

    if (strcmp(text, "-") == 0) {
        *keys = 0;
        return 1;
    }
    if (text[0] >= '0' && text[0] <= '9') {
        unsigned long v = strtoul(text, &end, 0);

        *keys = (u16)(v & KEYS_MASK);
        return *end == '\0';
    }
    snprintf(buf, sizeof(buf), "%s", text);
    *keys = 0;
    for (tok = strtok_r(buf, "+", &save); tok != NULL; tok = strtok_r(NULL, "+", &save)) {
        for (i = 0; i < sizeof(sKeyNames) / sizeof(sKeyNames[0]); i++) {
            if (strcasecmp(tok, sKeyNames[i].name) == 0) {
                *keys |= sKeyNames[i].bit;
                break;
            }
        }
        if (i == sizeof(sKeyNames) / sizeof(sKeyNames[0])) {
            return 0;
        }
    }
    return 1;
}

void HostInputInit(void)
{
    FILE *f;
    char line[256];
    u32 lineNo = 0;
    u32 cap = 0;

    if (gHostOptions.keys == NULL) {
        return;
    }
    f = fopen(gHostOptions.keys, "r");
    if (f == NULL) {
        fprintf(stderr, "%s: %s\n", gHostOptions.keys, strerror(errno));
        exit(2);
    }
    while (fgets(line, sizeof(line), f) != NULL) {
        char keys[128];
        unsigned long frame;
        char *hash = strchr(line, '#');
        int n;

        lineNo++;
        if (hash != NULL) {
            *hash = '\0';
        }
        n = sscanf(line, "%lu %127s", &frame, keys);
        if (n <= 0) {
            continue;
        }
        if (sRunCount == cap) {
            cap = cap ? cap * 2 : 64;
            sRuns = realloc(sRuns, cap * sizeof(*sRuns));
        }
        if (n != 2 || !ParseKeys(keys, &sRuns[sRunCount].keys) ||
            (sRunCount != 0 && frame <= sRuns[sRunCount - 1].frame)) {
            fprintf(stderr, "%s:%u: expected `FRAME KEYS`, frames in order\n", gHostOptions.keys,
                    lineNo);
            exit(2);
        }
        sRuns[sRunCount].frame = (u32)frame;
        sRunCount++;
    }
    fclose(f);
}

/* OAM with the bits the display ignores cleared (boottest's
 * picture_hash): a hidden entry's attributes, not its affine parameter
 * (part of a matrix), and a regular entry's matrix bits (attr1 9-11). */
static void PictureOam(u16 *oam)
{
    u32 i;

    memcpy(oam, gHostOam, HOST_OAM_SIZE);
    for (i = 0; i < HOST_OAM_SIZE / 2; i += 4) {
        u32 mode = (oam[i] >> 8) & 3;

        if (mode == 2) {
            oam[i] = 0x200;
            oam[i + 1] = 0;
            oam[i + 2] = 0;
        } else if (mode == 0) {
            oam[i + 1] &= ~0xE00;
        }
    }
}

static void PrintHash(const u8 *digest)
{
    u32 i;

    for (i = 0; i < 8; i++) {
        printf("%02x", digest[i]);
    }
}

static void Checkpoint(void)
{
    static const struct {
        const unsigned char *data;
        u32 size;
    } regions[] = {
        { gHostPltt, HOST_PLTT_SIZE },
        { gHostVram, HOST_VRAM_SIZE },
        { gHostOam, HOST_OAM_SIZE },
    };
    struct HostSha1 sha;
    u8 digest[20];
    u8 picture[20];
    u16 oam[HOST_OAM_SIZE / 2];
    u32 i;

    HostSha1Init(&sha);
    for (i = 0; i < 3; i++) {
        HostSha1Update(&sha, regions[i].data, regions[i].size);
    }
    HostSha1Final(&sha, digest);
    PictureOam(oam);
    HostSha1Init(&sha);
    HostSha1Update(&sha, gHostPltt, HOST_PLTT_SIZE);
    HostSha1Update(&sha, gHostVram, HOST_VRAM_SIZE);
    HostSha1Update(&sha, oam, HOST_OAM_SIZE);
    HostSha1Final(&sha, picture);
    if (!gHostOptions.quiet) {
        printf("checkpoint %u ", sFrame);
        PrintHash(digest);
        printf(" ");
        PrintHash(picture);
        printf("\n");
        fflush(stdout);
    }
    if (gHostOptions.dumpDir != NULL) {
        char path[4096];
        FILE *f;

        snprintf(path, sizeof(path), "%s/%05u.bin", gHostOptions.dumpDir, sFrame);
        f = fopen(path, "wb");
        if (f == NULL) {
            fprintf(stderr, "%s: %s\n", path, strerror(errno));
            exit(2);
        }
        for (i = 0; i < 3; i++) {
            fwrite(regions[i].data, 1, regions[i].size, f);
        }
        fclose(f);
    }
}

u32 HostFrame(void)
{
    return sFrame;
}

u16 HostReadKeys(void)
{
    u16 keys = 0;

    sFrame++;
    if (gHostOptions.frames != 0 && sFrame > gHostOptions.frames) {
        HostExit(0, "done");
    }
    while (sRun + 1 < sRunCount && sRuns[sRun + 1].frame <= sFrame) {
        sRun++;
    }
    if (sRunCount != 0 && sRuns[sRun].frame <= sFrame) {
        keys = sRuns[sRun].keys;
    }
    if (gHostOptions.checkpointEvery != 0 && sFrame % gHostOptions.checkpointEvery == 0) {
        Checkpoint();
    }
    /* REG_KEYINPUT is active low. */
    return (u16)(~keys & KEYS_MASK);
}
