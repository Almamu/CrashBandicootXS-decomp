/* The pc platform's front end: an SDL2 window showing the PPU's picture,
 * the keyboard and gamepads as the GBA's keys, and the GBA's frame rate.
 *
 * Built with SDL when platform.mk finds it (HOST_SDL=1, pkg-config's
 * sdl2); without it the executable runs headless only, and this file is
 * the one function that says so.
 *
 * Each VBlank (irq.c, line 160) HostFrontendVBlank:
 * - shows gHostScreen, the picture the PPU just finished, scaled to the
 *   window (--scale N, 3 by default; the window can be resized, and keeps
 *   the GBA's 3:2 shape);
 * - reads the events: the window closed or Escape ends the run (`end
 *   quit`), F12 writes a screenshot, the keys and buttons held set the
 *   keys the game reads at its next frame (HostFrontendKeys, input.c);
 * - waits until the frame's time is up: the GBA shows 16777216 / 280896
 *   = 59.7275 frames a second. The time is kept against a running
 *   deadline, so short waits even out; when the host falls more than a few
 *   frames behind, the deadline starts again from now. Tab held runs
 *   without waiting.
 *
 * Keyboard: arrows the D-pad, X = A, Z = B, A = L, S = R, Enter = Start,
 * Backspace or right Shift = Select. A gamepad (SDL's game controller
 * layout, the Xbox one): the D-pad or the left stick, the right face
 * button (B on an Xbox pad) = A, the bottom one (A) = B, the shoulders
 * and triggers = L and R, Start and Back = Start and Select. These match
 * the GBA's buttons by position. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gba/gba.h"
#include "host_platform.h"

#ifndef HOST_SDL

int HostFrontendAvailable(void)
{
    return 0;
}

void HostFrontendInit(void)
{
}

void HostFrontendVBlank(void)
{
}

u16 HostFrontendKeys(void)
{
    return 0;
}

#else

#include <SDL.h>

/* The GBA's frame: 280896 cycles of 16.78 MHz. */
#define GBA_CLOCK      16777216
#define CYCLES_A_FRAME 280896
#define AXIS_DEADZONE  12000

static SDL_Window *sWindow;
static SDL_Renderer *sRenderer;
static SDL_Texture *sTexture;
static Uint64 sDeadline;
static Uint64 sFrameTicks; /* a frame in performance-counter ticks, << 8 */
static Uint64 sFraction;
static u16 sKeys;
static u32 sShots;

static const struct {
    SDL_Scancode key;
    u16 bit;
} sKeyMap[] = {
    { SDL_SCANCODE_X, A_BUTTON },
    { SDL_SCANCODE_Z, B_BUTTON },
    { SDL_SCANCODE_BACKSPACE, SELECT_BUTTON },
    { SDL_SCANCODE_RSHIFT, SELECT_BUTTON },
    { SDL_SCANCODE_RETURN, START_BUTTON },
    { SDL_SCANCODE_KP_ENTER, START_BUTTON },
    { SDL_SCANCODE_RIGHT, DPAD_RIGHT },
    { SDL_SCANCODE_LEFT, DPAD_LEFT },
    { SDL_SCANCODE_UP, DPAD_UP },
    { SDL_SCANCODE_DOWN, DPAD_DOWN },
    { SDL_SCANCODE_S, R_BUTTON },
    { SDL_SCANCODE_A, L_BUTTON },
};

static const struct {
    SDL_GameControllerButton button;
    u16 bit;
} sButtonMap[] = {
    { SDL_CONTROLLER_BUTTON_B, A_BUTTON },
    { SDL_CONTROLLER_BUTTON_A, B_BUTTON },
    { SDL_CONTROLLER_BUTTON_BACK, SELECT_BUTTON },
    { SDL_CONTROLLER_BUTTON_START, START_BUTTON },
    { SDL_CONTROLLER_BUTTON_DPAD_RIGHT, DPAD_RIGHT },
    { SDL_CONTROLLER_BUTTON_DPAD_LEFT, DPAD_LEFT },
    { SDL_CONTROLLER_BUTTON_DPAD_UP, DPAD_UP },
    { SDL_CONTROLLER_BUTTON_DPAD_DOWN, DPAD_DOWN },
    { SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, R_BUTTON },
    { SDL_CONTROLLER_BUTTON_LEFTSHOULDER, L_BUTTON },
};

int HostFrontendAvailable(void)
{
    return 1;
}

static void Fail(const char *what)
{
    fprintf(stderr, "%s: %s\n", what, SDL_GetError());
    exit(2);
}

void HostFrontendInit(void)
{
    u32 scale = gHostOptions.scale;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        Fail("SDL_Init");
    }
    sWindow = SDL_CreateWindow("Crash Bandicoot XS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               HOST_SCREEN_WIDTH * scale, HOST_SCREEN_HEIGHT * scale,
                               SDL_WINDOW_RESIZABLE);
    if (sWindow == NULL) {
        Fail("SDL_CreateWindow");
    }
    /* No vsync: the frames are paced to the GBA's rate, not the monitor's. */
    sRenderer = SDL_CreateRenderer(sWindow, -1, 0);
    if (sRenderer == NULL) {
        Fail("SDL_CreateRenderer");
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_RenderSetLogicalSize(sRenderer, HOST_SCREEN_WIDTH, HOST_SCREEN_HEIGHT);
    sTexture = SDL_CreateTexture(sRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING,
                                 HOST_SCREEN_WIDTH, HOST_SCREEN_HEIGHT);
    if (sTexture == NULL) {
        Fail("SDL_CreateTexture");
    }
    /* Pads plugged in now are opened by their SDL_CONTROLLERDEVICEADDED. */
    sFrameTicks = (SDL_GetPerformanceFrequency() << 8) * CYCLES_A_FRAME / GBA_CLOCK;
    sDeadline = SDL_GetPerformanceCounter();
}

/* F12: the window's picture as the renderer drew it, in
 * crashbandicootxs-N.png (the first N not taken). */
static void Screenshot(void)
{
    int w;
    int h;
    u32 *pixels;
    char path[64];
    FILE *f;

    SDL_GetRendererOutputSize(sRenderer, &w, &h);
    pixels = malloc((size_t)w * h * 4);
    if (pixels == NULL || w > 1024 ||
        SDL_RenderReadPixels(sRenderer, NULL, SDL_PIXELFORMAT_RGB888, pixels, w * 4) != 0) {
        /* Wider than png.c's rows, or no read-back: the GBA's picture. */
        free(pixels);
        pixels = NULL;
    }
    do {
        snprintf(path, sizeof(path), "crashbandicootxs-%u.png", ++sShots);
        f = fopen(path, "rb");
        if (f != NULL) {
            fclose(f);
        }
    } while (f != NULL);
    if (pixels != NULL ? HostWritePng(path, pixels, w, h)
                       : HostWritePng(path, gHostScreen, HOST_SCREEN_WIDTH, HOST_SCREEN_HEIGHT)) {
        printf("screenshot %s before frame %u\n", path, HostFrame() + 1);
        fflush(stdout);
    }
    free(pixels);
}

static u16 PadKeys(void)
{
    u16 keys = 0;
    int n = SDL_NumJoysticks();
    int i;
    u32 b;

    for (i = 0; i < n; i++) {
        SDL_GameController *pad;
        Sint16 x;
        Sint16 y;

        if (!SDL_IsGameController(i)) {
            continue;
        }
        pad = SDL_GameControllerFromInstanceID(SDL_JoystickGetDeviceInstanceID(i));
        if (pad == NULL) {
            continue;
        }
        for (b = 0; b < sizeof(sButtonMap) / sizeof(sButtonMap[0]); b++) {
            if (SDL_GameControllerGetButton(pad, sButtonMap[b].button)) {
                keys |= sButtonMap[b].bit;
            }
        }
        if (SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > AXIS_DEADZONE) {
            keys |= L_BUTTON;
        }
        if (SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > AXIS_DEADZONE) {
            keys |= R_BUTTON;
        }
        x = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTX);
        y = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTY);
        if (x < -AXIS_DEADZONE) {
            keys |= DPAD_LEFT;
        } else if (x > AXIS_DEADZONE) {
            keys |= DPAD_RIGHT;
        }
        if (y < -AXIS_DEADZONE) {
            keys |= DPAD_UP;
        } else if (y > AXIS_DEADZONE) {
            keys |= DPAD_DOWN;
        }
    }
    return keys;
}

static void Pace(int fast)
{
    Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 now;

    sFraction += sFrameTicks;
    sDeadline += sFraction >> 8;
    sFraction &= 0xFF;
    now = SDL_GetPerformanceCounter();
    if (fast || now > sDeadline + 4 * (sFrameTicks >> 8)) {
        /* Running fast, or far behind (a slow host, a stop in a
         * debugger): start again from now. */
        sDeadline = now;
        return;
    }
    while (now < sDeadline) {
        Uint64 left = (sDeadline - now) * 1000 / freq;

        if (left > 2) {
            SDL_Delay((Uint32)(left - 1));
        }
        now = SDL_GetPerformanceCounter();
    }
}

void HostFrontendVBlank(void)
{
    const Uint8 *state;
    SDL_Event e;
    u32 i;
    int shot = 0;

    SDL_UpdateTexture(sTexture, NULL, gHostScreen, HOST_SCREEN_WIDTH * 4);
    SDL_RenderClear(sRenderer);
    SDL_RenderCopy(sRenderer, sTexture, NULL, NULL);
    /* The picture game frame N reads its keys under (--screens's FRAME.png),
     * finished before that frame. */
    if (gHostOptions.screenshotAt != 0 && HostFrame() + 1 == gHostOptions.screenshotAt) {
        shot = 1;
    }

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT:
            HostExit(0, "quit");
            break;
        case SDL_KEYDOWN:
            if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                HostExit(0, "quit");
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_F12 && !e.key.repeat) {
                shot = 1;
            }
            break;
        case SDL_CONTROLLERDEVICEADDED:
            SDL_GameControllerOpen(e.cdevice.which);
            break;
        case SDL_CONTROLLERDEVICEREMOVED:
            {
                SDL_GameController *pad = SDL_GameControllerFromInstanceID(e.cdevice.which);

                if (pad != NULL) {
                    SDL_GameControllerClose(pad);
                }
                break;
            }
        default:
            break;
        }
    }
    if (shot) {
        Screenshot();
    }
    SDL_RenderPresent(sRenderer);

    state = SDL_GetKeyboardState(NULL);
    sKeys = PadKeys();
    for (i = 0; i < sizeof(sKeyMap) / sizeof(sKeyMap[0]); i++) {
        if (state[sKeyMap[i].key]) {
            sKeys |= sKeyMap[i].bit;
        }
    }
    Pace(state[SDL_SCANCODE_TAB]);
}

u16 HostFrontendKeys(void)
{
    return sKeys;
}

#endif /* HOST_SDL */
