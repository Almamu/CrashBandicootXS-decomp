#ifndef GUARD_KEY_INPUT_HPP
#define GUARD_KEY_INPUT_HPP

/* The key input object (gInput, globals.h; engine/src/engine/sys/key_input.cpp,
 * #759). Its methods read and write gKeys (globals.h), never the object:
 * every caller still loads gInput into r0 as `this`, as the C passed it
 * as the ignored `input` argument. cxx_symbols.txt maps them onto their
 * C names. It has no vtable and no destructor: level_cutscene.cpp's
 * `delete gInput` is a plain OperatorDelete call.
 *
 * In the namespace Engine::Sys (#1033, docs/cplusplus.md "Namespaces").
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
}

/* GetDpadDirection's result (#843, docs/flags/input.md): the held D-pad
 * as one direction, through gDpadDirectionTable. Up and down together
 * read as down, left and right together as right. */
#define KEY_DIR_NONE 0
#define KEY_DIR_UP 1
#define KEY_DIR_DOWN 2
#define KEY_DIR_LEFT 3
#define KEY_DIR_RIGHT 4
#define KEY_DIR_UP_LEFT 5
#define KEY_DIR_UP_RIGHT 6
#define KEY_DIR_DOWN_LEFT 7
#define KEY_DIR_DOWN_RIGHT 8

/* gDpadDirectionTable's index: one bit per held D-pad button. */
#define KEY_DIR_INDEX_UP 1
#define KEY_DIR_INDEX_DOWN 2
#define KEY_DIR_INDEX_LEFT 4
#define KEY_DIR_INDEX_RIGHT 8

namespace Engine
{
namespace Sys
{

class KeyInput
{
public:
    u32 unused;

    KeyInput();            // ClearKeys
    s32 Update();          // UpdateKeys
    u8 GetDpadDirection(); // GetDpadDirection
};

COMPILE_TIME_ASSERT(key_input_hpp, sizeof(KeyInput) == 4);

} // namespace Sys
} // namespace Engine

#endif // GUARD_KEY_INPUT_HPP
