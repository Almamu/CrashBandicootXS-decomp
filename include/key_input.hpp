#ifndef GUARD_KEY_INPUT_HPP
#define GUARD_KEY_INPUT_HPP

/* The key input object (gInput, globals.h; src/system/key_input.cpp,
 * #759). Its methods read and write gKeys (globals.h), never the object:
 * every caller still loads gInput into r0 as `this`, as the C passed it
 * as the ignored `input` argument. cxx_symbols.txt maps them onto their
 * C names. It has no vtable and no destructor: level_cutscene.cpp's
 * `delete gInput` is a plain OperatorDelete call.
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
}

class KeyInput
{
public:
    u32 unused;

    KeyInput();            // ClearKeys
    s32 Update();          // UpdateKeys
    u8 GetDpadDirection(); // GetDpadDirection
};

COMPILE_TIME_ASSERT(key_input_hpp, sizeof(KeyInput) == 4);

#endif // GUARD_KEY_INPUT_HPP
