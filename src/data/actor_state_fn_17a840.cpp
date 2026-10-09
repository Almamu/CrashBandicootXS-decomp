#include "yeti.hpp"

/*
 * ROM 0x0817A840-0x0817A850. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The yeti's states (gYetiStateFuncs), called as `stateFuncs[state]()`
 * by Yeti::Update (yeti_update.cpp). Static member functions, so plain
 * 4-byte function pointers. */
const Yeti::StateFunc Yeti::stateFuncs[4] = {
    StateChase,
    StateCharge,
    StateCaught,
    StateStop,
};
