#include "core.h"
#include "memory.h"

/* Genuine no-op stub sitting between the still-raw `sub_802E058` (VRAM
 * pattern generator) and `sub_802E0A4` - see
 * docs/matching/issue-54-actor-d3a8.md. */
void nullsub_27(void)
{
}

asm(".align 2, 0");
