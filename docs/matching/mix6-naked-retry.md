# Mixed NAKED retry (mix-6): 0 of 9 closed

This pass tried the `"+r"` escape from #484 (`BOX_ADDR`, "stack address
recomputed per use") on nine parked drafts, starting with the smallest gaps.
Nothing closed. Every function stays NAKED. This pass changed only the
one-line notes on the drafts.

| Function | File | Start | Result |
|---|---|---|---|
| RunRoom (#37) | `src/system/game_loop56.c` | 15 hw (old_agbcc) | Not closed. The one-byte struct stack argument still materializes the constant before `add rN, sp, #4`. The `"=r"/"0"` and `"+r"` escapes, other field shapes, a `u8` prototype and an inline wrapper all leave it the same or worse. The file is agbcc (57 hw there). |
| DrawPauseMenu (#7) | `src/graphics/settings_menu21.c` | 20 hw | Not closed. Nudges or escapes on x/y/m/width stay at 20 hw or go higher. The ROM also picks r4 for the next `ldrsh` constant, so the difference is broader than x/y priority. |
| SpawnFlamethrowerLabAssistant (#31) | `src/graphics/graphics_loading_1feec.c` | 62 hw (old) | Not closed. Register allocation: the draft gives part+0x28 a low callee-saved register where the ROM gives it to arg3. Nudges on arg3 and alternate flip spellings are the same or worse. |
| DrawPauseFraction (#7) | `src/graphics/settings_menu16.c` | 73 hw | Not closed. A fresh `"=r"/"0"` 0x114 offset matches the first half (one register off). The second half's `r6 = r7; r7 += 4` posY derivation has not been reproduced. Best was 62 hw, 4 bytes long. |
| ReceiveSaveTransferChunk (#5) | `src/graphics/settings_menu8a2.c` | 104 hw | Not closed. `"+r"` on the index, the channel, the session or the offset does not bring back the second `muls` (104-115 hw). The wrap loop's per-iteration `movs r0,#0` is a separate difference. |
| sub_801AB98 (#25), sub_801A114 (#24), CreateCrate (#13), CreateWumpa (#15) | | 95-656 hw | Not attempted, because of the time budget (the larger gaps came last). |

The helper scripts (`var.py` variant specs) are in the session scratchpad,
under `mix6/`.
