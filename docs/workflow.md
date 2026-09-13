# The per-function matching loop

Every function that goes from `asm/code_3_*.s` into real C **must**
follow this same loop, in order, end to end, every time - including the
cleanup step (step 7), which in earlier sessions happened as an
occasional separate pass over a batch of already-matched functions (see
the two "Cleanup pass" entries near the end of
[matching.md](./matching.md)) but is now part of matching each single
function, not something deferred and done later in bulk. This is not
optional style guidance - skipping straight from "it compiles and
matches" to the next function, without the cleanup step, is an
incomplete pass and should be finished before moving on.

1. Read the function's disassembly, work out what it does, write C that
   should produce the same logic. Use whatever's already been reversed
   about neighboring functions/globals (structs, named constants,
   register-macro headers) rather than starting from raw offsets again.
2. Compile just that translation unit with the project's real compiler
   and compare the output instruction-by-instruction against the
   original disassembly (`arm-none-eabi-cpp` + `tools/agbcc/bin/agbcc`
   with the same flags `Makefile`'s `C_BUILDDIR` rule uses - see there
   for the exact invocation).
3. Iterate on the C until it's byte-exact - register-variable pins,
   inline-asm anchors, and a trailing `asm(".align 2, 0")` for padding
   mismatches are all fair game (see `matching_decomp_register_pinning`/
   `matching_decomp_alignment_fix` memory for the established
   techniques, and matching.md's alignment-padding gotcha for one
   specific recurring case). If a genuine gap resists every technique
   tried, park it under the `NON_MATCHING` build toggle instead of
   leaving raw asm or giving up on the C reconstruction entirely - see
   `matching_decomp_non_matching_toggle` memory and the "Parked, not
   matched" entries in matching.md for the pattern and when to stop
   iterating.
4. Once it matches (or is deliberately parked), cut that function's
   block out of whichever `asm/code_3_*.s` file currently holds it (it
   becomes two files: everything before, and everything after), add the
   C version to the right `.c` file, and add an entry to `ldscript.txt`'s
   `ROM :` block placing the new object file exactly where the removed
   asm block used to sit in link order - the linker concatenates
   whatever's listed there in that literal order, so this is what keeps
   the function at its original ROM address. **One `.c` file per
   contiguous ROM region, not one per "topic"** - a function whose real
   address isn't adjacent to an existing matched file's functions needs
   its own new `.c` file instead (see `GetAnimFrameBaseOffset`'s entry
   in matching.md for a worked example).
5. Rename every remaining `bl <old_name>`/`.4byte <old_name>` reference
   to the function elsewhere in the still-asm files to match (the linker
   will fail with "undefined reference" if any are missed - a useful
   safety net, not just a cosmetic step).
6. Full clean `make compare` (and `make NON_MATCHING=1 <rom-target>` too,
   if the function ended up parked).
7. **Cleanup pass, on this function alone, right now - do not defer it
   to a later batch pass:**
   - Replace every raw hardware address (`0x040000xx`/`0x0500...`/
     `0x0600...`/`0x0700...`) with the matching `REG_*`/`OAM`/`PLTT`/
     `DMA_*` macro from `include/gba/io_reg.h`/`defines.h`/
     `include/gba/dma_macros.h`. If the register genuinely isn't covered
     yet, add a new macro there (following the existing naming) rather
     than leaving raw hex in the function.
   - Replace raw pointer-arithmetic field access (`*(u32 *)((u8 *)base +
     0x10)`) with a real struct and a named field, wherever the shape is
     already known or can be worked out here. **Check whether a struct
     for the same object already exists elsewhere first** - a different
     function operating on the same global/parameter almost always means
     the same layout, and should extend/reuse that struct (moving it to
     a shared header if the two translation units link far apart) rather
     than get a second, differently-named struct for the same bytes.
   - Only keep raw pointer arithmetic, inline asm, or a register-variable
     pin where a plain struct/field-access rewrite has actually been
     tried and demonstrably changes the generated code (gcc's CSE,
     instruction scheduling, or register choice) - confirm this by
     rebuilding, not by assumption. When something has to stay low-level
     for this reason, leave a one-line comment saying so (pointing at
     this doc) instead of leaving it unexplained.
   - Replace magic numeric constants with an existing named constant
     when one already covers this exact value/meaning elsewhere in the
     project (`OAM_ENTRY_COUNT`, `DMA_ENABLE`, and so on); don't invent a
     new named constant for a value whose meaning isn't actually
     understood yet - an unexplained `0x2D` is more honest than a
     confidently-named constant that's really a guess.
   - **Rebuild and re-run `make compare` (and `make NON_MATCHING=1
     <rom-target>`, if parked) after each individual cleanup edit**, not
     just once at the end. A cleanup edit that's supposed to be a
     behavior-preserving no-op can still silently change codegen (the
     classic culprit: a struct-typed access lets gcc merge/reorder an
     address computation that the raw-pointer version kept separate,
     exactly like `sub_8006864`/`sub_8006820`'s inline-asm-guarded reads,
     documented in matching.md) - any such regression gets reverted
     immediately, not left in on the theory that it's "close enough."
8. Update matching.md's per-function log (a short paragraph like the
   existing entries is enough; a subtler fix may deserve its own
   explained bullet like the "Cleanup pass" entries there) and
   `README.md`'s status list, then commit.
