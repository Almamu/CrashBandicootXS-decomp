# Sprite banks by index: `gSpriteBankSet->Bank(n)` (#855)

A sprite's `bank` points at one entry of the sprite bank table
(`gSpriteBankSet->table->banks`, sprite_bank.h, 12 bytes per bank). The
code used to take it as a byte offset from the table's first byte:

```cpp
#define SPRITE_BANK_BASE (*(u8 *const *)gSpriteBankSet->table)  // globals.h
c->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x27C);
```

The offsets were spelled in several ways (`0x8d * 4`, `0xcc << 1`,
`0xd2 * 2`, `anim * sizeof(struct sprite_bank)`), and six helpers took
them as arguments: `AnimTable` (level_select.hpp), `BankAnim`
(spawn_enemies.cpp), `HUD_ANIM` (hud_init.cpp), `SetIconBank`
(menus.hpp), `SpawnTouchable` (spawn_pickups.cpp) and `new_row_icon`
(save_menu_draw.cpp). Every offset is a multiple of 12.

SpriteBankSet (sprite_obj.hpp) now has an inline `Bank(index)`,
`&table->banks[index]`, and every site uses it with the bank's index:
`c->bank = gSpriteBankSet->Bank(53);`. The 41 byte-offset casts and the
`SPRITE_BANK_BASE` macro are gone.

- `AnimTable`, `BankAnim` and `HUD_ANIM` only did the cast, so they went
  too, and their callers call `Bank` directly.
- `SetIconBank`, `SpawnTouchable` and `new_row_icon` keep their other
  work (the store through a pointer to the member, the spawn), and take
  a bank index.
- The comments that named banks by their offsets (`bank +0x21c`) give the
  index (`bank 45`).

Every offset is a constant or `index * 12`, and gcc folds both forms to
the same address before it allocates registers. Reading `table->banks`
instead of the table's first word as a `u8 *` loads the same word. Every
object is the same.

