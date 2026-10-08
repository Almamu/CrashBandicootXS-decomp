#ifndef GUARD_MATCH_H
#define GUARD_MATCH_H

/* Named spellings of the matching workarounds (#576).
 *
 * Matching the ROM byte for byte with agbcc (gcc 2.9) sometimes needs a
 * statement that emits no code but changes what the compiler is allowed
 * to assume: an empty `asm` with operands, or a local variable pinned to
 * a hard register. Each macro below is exactly one of the spellings the
 * matching phase used, token for token, so replacing the spelled-out
 * form with the macro never changes the object code. The macros don't
 * make the C any less of a workaround; they name it and point here.
 *
 * Rules for using them:
 * - Every use still gets a short comment saying what it fixes (which
 *   register, which reload, which constant). The macro says how; the
 *   comment says why.
 * - Don't add one to new code unless a byte comparison shows it's
 *   needed, and check the whole object, not just the function.
 * - docs/matching_techniques.md is the reference for when each idiom
 *   applies, and for the techniques that aren't macros (old_agbcc,
 *   per-object flags, loop shapes, struct-by-value arguments, ...).
 *   tools/match_idioms.py counts the sites of each idiom, and its
 *   --check (run by CI) fails if one of them is spelled out by hand
 *   instead of with its macro.
 *
 * On "volatile": gcc 2.9 treats an asm with no outputs as volatile
 * anyway. An asm with outputs is an ordinary insn: loop optimization
 * can hoist it, CSE can merge two identical ones, and flow deletes it
 * if its output is dead. `asm volatile` stops all three and also makes
 * the insn a scheduling barrier (sched.c treats a volatile asm as using
 * and clobbering every register and all of memory). The _VOLATILE
 * variants are for the sites where the plain form was moved or merged;
 * MATCH_USE and MATCH_CONST have none since the C++ conversion (#664) left
 * no site that needed one. */

/* MATCH_HOLD_REG(T, name, reg): declare `name` as a local register
 * variable pinned to the hard register `reg` (r0-r12, written without
 * quotes), the "register pin":
 *
 *     MATCH_HOLD_REG(u8 *, p, r1) = &self->field;
 *
 * is `register u8 *p asm("r1") = &self->field;`. At -O2 gcc 2.9 gives
 * the variable that register for its whole life (its pseudo is replaced
 * by the hard register before allocation), so the value is computed into
 * it, and nothing else can use that register while the variable is live.
 * Use it when the ROM keeps a value in a different register from the one
 * gcc picks, or when a value must stay in one register across a span
 * (see MATCH_HOLD). It is a declarator, so T can't be an array or
 * function-pointer type that needs the name inside it. */
#define MATCH_HOLD_REG(T, name, reg) register T name asm(#reg)

/* MATCH_BARRIER(): `asm("")`, an empty statement. Emits nothing, but it
 * is a volatile insn: the scheduler moves nothing across it, cross-
 * jumping won't merge a tail containing it with one that doesn't, and
 * it counts toward the insn-count heuristics (loop.c's invariant-motion
 * threshold, the GCSE hash-table size, which sets the order of the
 * GCSE temporaries' stack slots). It doesn't make gcc forget any value
 * it already holds in a register. Use it to keep a load or store on the
 * ROM's side of a statement, to keep two tails apart, or as padding
 * where an insn count matters (docs/matching_techniques.md). */
#define MATCH_BARRIER() asm("")

/* MATCH_USE(x): `asm("" : : "r"(x))`. Emits nothing, but `x` must be
 * live, in a register, at this point. Use it to extend a live range (so
 * the value stays in its register instead of being reloaded or
 * rematerialized after it), to give gcc an extra reference that changes
 * a register-allocation priority, and to end a MATCH_HOLD. Volatile
 * (no outputs), so it is also a scheduling barrier. */
#define MATCH_USE(x) asm("" : : "r"(x))

/* MATCH_USE2(a, b): `asm("" : : "r"(a), "r"(b))`, MATCH_USE of two
 * values in one asm. It isn't the same as two MATCH_USEs: those are two
 * insns, which the scheduler, the insn-count heuristics and reload all
 * see separately. Use it where the ROM needs both values live in
 * registers at the same point. */
#define MATCH_USE2(a, b) asm("" : : "r"(a), "r"(b))
#define MATCH_USE2_VOLATILE(a, b) asm volatile("" : : "r"(a), "r"(b))

/* MATCH_KEEP(x): `asm("" : "+r"(x))`. Emits nothing, but after it gcc
 * no longer knows what `x` holds: it can't fold `x` into an immediate
 * or an addressing mode, can't share it with an equal expression (CSE),
 * and can't rematerialize it from its definition. Use it when the ROM
 * computes or reloads a value that gcc would otherwise reuse or
 * constant-propagate, or to force a copy into a register at this point.
 * The _VOLATILE form can't be hoisted out of a loop or merged with an
 * identical one. */
#define MATCH_KEEP(x) asm("" : "+r"(x))
#define MATCH_KEEP_VOLATILE(x) asm volatile("" : "+r"(x))

/* MATCH_HOLD(x): `asm("" : "=r"(x))`. Emits nothing; `x` is defined
 * here with an unknown value. With `x` pinned by MATCH_HOLD_REG this
 * occupies that hard register from here to the last use of `x`
 * (usually a MATCH_USE(x)), which steers the other values in the span
 * into different registers: the "hard-register hold" used to reproduce
 * the ROM's register choice or its reload round-robin. Without a pin,
 * it makes a variable deliberately undefined (the ROM uses whatever the
 * register held). */
#define MATCH_HOLD(x) asm("" : "=r"(x))
#define MATCH_HOLD_VOLATILE(x) asm volatile("" : "=r"(x))

/* MATCH_CONST(v, K): `asm("" : "=r"(v) : "0"(K))`, the "constant-init".
 * Loads K into v's register right here (the input is reloaded into the
 * register the "0" constraint ties to the output, so `movs rN, #K` or a
 * literal-pool load appears at this point), but gcc doesn't know that
 * v == K afterwards. So it can't CSE v with another load of K, hoist it,
 * sink it to its uses, or turn a store of v into a store of an immediate
 * or of another register that happens to hold K, and v doesn't get the
 * doubled live range `v = K` plus a MATCH_KEEP would give it. Use it
 * when the ROM loads a constant at a specific point, once per use, or
 * into a specific register. K can be any expression (a variable, an
 * address, a cast); the point is the same: an opaque copy made here. */
#define MATCH_CONST(v, K) asm("" : "=r"(v) : "0"(K))

/* MATCH_CLOBBER(reg): `asm("" : : : "reg")` (r0-r12, written without
 * quotes). Emits nothing, but tells gcc the hard register `reg` is
 * clobbered here. For a callee-saved register (r4-r11) that makes the
 * function save and restore it, so use it when the ROM's prologue
 * pushes a register the C otherwise never touches. It also ends any
 * value gcc was keeping in that register, forcing a reload after it.
 * The _VOLATILE form is token-for-token the `asm volatile` spelling; an
 * asm without outputs is volatile anyway. */
#define MATCH_CLOBBER(reg) asm("" : : : #reg)
#define MATCH_CLOBBER_VOLATILE(reg) asm volatile("" : : : #reg)

/* MATCH_MEMORY_BARRIER(): `asm volatile("" : : : "memory")`, the usual
 * compiler memory barrier. Emits nothing; gcc must assume any memory
 * changed here, so a value loaded from memory before it is loaded
 * again after it, and no load or store is moved across it. Use it where
 * the ROM reloads a global or a field that gcc would otherwise reuse
 * from a register. Being a volatile insn it is also everything
 * MATCH_BARRIER() is (e.g. it keeps two otherwise identical tails from
 * being cross-jumped). */
#define MATCH_MEMORY_BARRIER() asm volatile("" : : : "memory")

/* MATCH_KEEP_MEM(x): `asm("" : "+m"(x))`. Emits nothing, but `x` must
 * be in memory here (it's stored if it was only in a register), and gcc
 * no longer knows what that memory holds, so a later read of `x` is a
 * real load. Use it when the ROM stores a value and reads it back.
 * MATCH_USE_MEM(x): `asm("" : : "m"(x))`, the same but read-only: `x`
 * must be in memory here, without its value being forgotten, e.g. to
 * keep a variable in a stack slot across a call. */
#define MATCH_KEEP_MEM(x) asm("" : "+m"(x))
#define MATCH_USE_MEM(x) asm("" : : "m"(x))

/* MATCH_KEEP_EXPR(T, e): the value of `e`, converted to T, passed through
 * MATCH_KEEP: `({ T _p = (e); asm("" : "+r"(_p)); _p; })`. Each use is
 * its own opaque value, so two uses of the same address are computed
 * twice instead of being held in one register across calls. */
#define MATCH_KEEP_EXPR(T, e) ({ T _p = (e); asm("" : "+r"(_p)); _p; })

/* BOX_ADDR(a): the address of a stack `struct aabb`, as its own pseudo
 * at each use (MATCH_KEEP_EXPR). The crate collision code passes the
 * same stack box to several calls and the ROM recomputes `add rN, sp,
 * #K` before each one; without this, cse keeps the address in a callee-
 * saved register across the calls. See docs/matching/archive/
 * sp-box-retry.md. */
#define BOX_ADDR(a) MATCH_KEEP_EXPR(struct aabb *, a)

#endif /* GUARD_MATCH_H */
