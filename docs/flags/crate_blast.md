# An explosive crate's blast state (#843)

`Crate::blastState` (+0x48, include/game/crates/crate.hpp) is the explosive kinds'
view of the per-kind word (the TNT and nitro crates, `gCrateKindExplosive`).
The names, `CRATE_BLAST_STATE_*`, sit next to the slot crate's
`CRATE_SLOT_*` in crate.hpp.

| Value | Name | Set by | Read by |
|---|---|---|---|
| 0 | `NONE` | the placement (the word starts at 0); ApplyCollision when a slot crate stops on its TNT face and turns into a TNT crate | DropAbove arms the crates it drops only while the crate it runs for is `NONE` |
| 1 | `ARMED` | DropAbove, on an explosive crate above that falls more than 0x1600 (Q8), except that the top crate of a stack is armed only if it is an idle nitro | UpdateFall: when the fall ends, an armed crate (or any nitro) explodes; an unarmed TNT crate is lit instead, unless it is the top of a stack |
| 0xFF | `BLASTED` | BlastNearby, at the end of each blast (FinishBroken calls it on animation frames 3 and 6) | DropAbove: a crate that has blasted doesn't arm the crates its break drops |

Every site compares or stores the names; the ROM stores `0xff` as a
plain word, so `BLASTED` is `0xff`, not -1.
