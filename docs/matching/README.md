# docs/matching/

Write-ups from the matching campaign, the phase that turned the
disassembly into byte-exact C. That phase is over: every function
except the two parked IWRAM ARM ones (#553) is matched C.

## Top level: still-current references

These files describe something the build still depends on or work that
is still open. They are kept up to date.

- [iwram-image.md](./iwram-image.md): the IWRAM image (`IntrMain`, the
  ARM routines, the IWRAM data), including the two parked ARM
  functions (#553) and why they stay assembly.
- [per-file-flags-investigation.md](./per-file-flags-investigation.md):
  whether the original build used per-file optimization flags, and the
  evidence for the Makefile's `NO_STRENGTH_REDUCE_OBJS`.
- [eeprom-sdk-o1.md](./eeprom-sdk-o1.md): why the Nintendo EEPROM
  library (`lib/agb_eeprom/`) is built at `-O1` (`O1_OBJS`).

The general reference for the matching techniques (old_agbcc, per-file
flags, register pins and holds, the `asm` nudges and their
[include/match.h](../../include/match.h) macros, and the like) is
[docs/matching_techniques.md](../matching_techniques.md). It links the
archive cases below that each technique came from.

## archive/: the per-pass logs

[archive/](./archive/) holds the per-pass logs: one file per chunk
issue, parked function or retry pass, written while that work was done.
They record what each pass tried, what closed and what didn't. They are
a frozen historical record:

- don't edit them, except to fix a reference that no longer resolves;
- don't add new ones. A new write-up, for example when #553 closes, goes
  next to the files above, or in the PR description if it is short.

Source comments, `docs/status/` and the Makefile still cite archive
files by path (`docs/matching/archive/<name>.md`) as the evidence for
a particular workaround or build flag. The file names follow the old
convention: `issue-<N>-<slug>.md` for a GitHub chunk or parked-function
issue `<N>`, and `<slug>.md` for passes that weren't tied to one issue.
Function and file names inside the older files may be the pre-rename
ones (`sub_XXXX` labels, old `src/` paths);
[tools/file_layout_plan.tsv](../../tools/file_layout_plan.tsv) maps the
old file names to the current ones.

The older single-file log, [docs/matching.md](../matching.md), was
frozen when this directory was created. It is a historical record too.
