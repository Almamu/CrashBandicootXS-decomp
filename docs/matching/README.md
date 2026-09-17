# docs/matching/

One file per matching chunk/issue, written after `docs/matching.md`
(the old single-file log) was frozen because every PR appending to that
one shared file was the biggest source of merge conflicts between
parallel matching work - two PRs in this directory never touch the same
file, since each PR only ever *creates* a new one here, never edits an
existing entry.

**Naming:** `issue-<N>-<short-slug>.md`, where `<N>` is the GitHub
`decomp-chunk` (or `parked-function`/`cleanup`) issue number and
`<short-slug>` is a few words of the address range or subject (e.g.
`issue-73-0802fbf0-actor.md`). If there's no issue (a function matched
outside the chunk-issue workflow), use `<short-slug>-<address>.md`
instead.

**Content:** same style `docs/matching.md`'s entries always used - what
the function(s) do, the ROM addresses/files involved, any real
compiler-codegen gotcha found and how it was fixed, and (for parked
functions) exactly what was tried and why it didn't close. Cross-link
to the relevant `docs/status/<category>.md` page and back.

**Don't edit another PR's file here** unless you're specifically
correcting something you found wrong in it - this directory's whole
point is that each PR's file is its own, so nobody else's PR conflicts
with yours.
