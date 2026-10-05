#!/usr/bin/env python3
"""Extracts a contiguous, address-bounded text slice out of one of the
frozen expected/*.s sources (never edits the source itself - see
docs/decomp_dev.md and tools/report_units.py, which drives this to build
one objdiff unit per matched src/*.c file so decomp.dev can show
per-category progress).

Slicing works at the text level (find the label line for the start
address, find the label line for the end address, take everything in
between) rather than on assembled bytes, so relocations/literal pools
stay intact - the same technique expected/legacy.s itself was carved
out with.
"""
import re
import sys

HEADER = '.include "asm/macros.inc"\n\n.syntax unified\n.arm\n\n'


def find_label_line(lines, address):
    """Line index of the *start* of the function at `address`: the label
    line itself (`<name>: @ 0x<address>`), backed up one line further if
    that's a `thumb_func_start`/`arm_func_start <name>` directive - those
    set .global/.thumb_func/.type, and cutting a slice right at the bare
    label (leaving the directive out) silently makes the first function
    in every slice local instead of global, which objdiff then drops
    from its function list entirely."""
    pattern = re.compile(rf"^(\S+):\s*@\s*0x0*{address:X}\b", re.IGNORECASE)
    for i, line in enumerate(lines):
        m = pattern.match(line)
        if not m:
            continue
        name = m.group(1)
        if i > 0 and re.search(rf"\b(?:thumb|arm)_func_start\s+{re.escape(name)}\b", lines[i - 1]):
            return i - 1
        return i
    return None


def main():
    argv = sys.argv[1:]
    resolve = [a[len("--resolve="):] for a in argv if a.startswith("--resolve=")]
    argv = [a for a in argv if not a.startswith("--resolve=")]
    if len(argv) not in (2, 3):
        sys.exit(f"usage: {sys.argv[0]} [--resolve=NAME...] <source.s> <start_addr_hex> [<end_addr_hex>]")
    source_path = argv[0]
    start_addr = int(argv[1], 16)
    end_addr = int(argv[2], 16) if len(argv) == 3 else None

    with open(source_path) as f:
        lines = f.readlines()

    start_line = find_label_line(lines, start_addr)
    if start_line is None:
        sys.exit(f"slice_expected: no label for address {start_addr:#x} in {source_path}")

    if end_addr is not None:
        end_line = find_label_line(lines, end_addr)
        if end_line is None:
            # Not covered by this source (crosses into the next frozen
            # file, or into still-raw asm) - take the rest of the file.
            end_line = len(lines)
    else:
        end_line = len(lines)

    sliced = normalize_pool_padding(lines[start_line:end_line])
    sliced = decode_code_words(sliced, pool_references(lines))
    sys.stdout.write(HEADER)
    sys.stdout.writelines(sliced)

    # `resolve` entries in expected/corrections.txt: a function right after
    # this slice that the base object calls through a local `.set` alias
    # of a symbol it defines itself (the libgcc `__udivsi3` alias, back
    # when libgcc2.c and __udivsi3 were one object and before that
    # function was named __udivsi3 itself), so the
    # assembler resolved those `bl`s with no
    # relocation. A local label at the slice's end, where that function
    # starts, makes the target's `bl`s resolve the same way.
    if end_addr is not None and end_line < len(lines):
        end_label = re.match(r"^\s*(?:thumb_func_start|arm_func_start)\s+(\S+)", lines[end_line])
        end_name = end_label.group(1) if end_label else lines[end_line].split(":", 1)[0].strip()
        if end_name in resolve:
            sys.stdout.write(f"{end_name}:\n")


NUMERIC_WORD = re.compile(r"^(_[0-9A-Fa-f]{8}):\s*\.4byte\s+(0x[0-9A-Fa-f]+|\d+)\s*$")
POOL_LOAD = re.compile(r"^\s*(?:ldr|adr|add)\b.*\b(_[0-9A-Fa-f]{8})\b")


def pool_references(lines):
    """Every local label some `ldr`/`adr`/`add` loads from, across the whole
    frozen source (a slice can load from a pool its own range doesn't
    cover only in theory, but checking the whole file costs nothing)."""
    refs = set()
    for line in lines:
        m = POOL_LOAD.match(line)
        if m:
            refs.add(m.group(1))
    return refs


def decode_code_words(lines, refs):
    """Re-emits a numeric `.4byte` that nothing loads as the two Thumb
    halfwords it really is. The disassembly lost sync in a few places
    (all in the actor_anim.c region around 0x0803B4C0-0x0803B83C) and wrote
    real instructions - `push {lr}; adds r3, r0, #0` as `.4byte 0x1c03b500`
    and so on - as data. The bytes are right, but the `.4byte` puts them in
    a data mapping region, so objdiff shows one `.word` where the base
    object has two instructions. A real literal-pool word is always the
    target of some pc-relative load, so an unloaded numeric word can only
    be misread code. A zero upper halfword stays data (`.2byte 0`): it's
    the alignment padding in front of a pool, which agbcc emits as data
    too."""
    out = []
    for line in lines:
        m = NUMERIC_WORD.match(line)
        if not m or m.group(1) in refs:
            out.append(line)
            continue
        value = int(m.group(2), 0)
        lo, hi = value & 0xFFFF, value >> 16
        out.append(f"{m.group(1)}: .inst.n {lo:#06x}\n")
        out.append(f"\t.inst.n {hi:#06x}\n" if hi else "\t.2byte 0\n")
    return out


MOVS_R0_R0 = re.compile(r"^\s*movs\s+r0,\s*r0\s*$")
POOL_WORD = re.compile(r"^_([0-9A-Fa-f]{8}):\s*\.4byte\b")


def normalize_pool_padding(lines):
    """Rewrites a `movs r0, r0` that is really the zero padding halfword in
    front of a literal pool as `.align 2, 0`. The bytes are the same, but
    the disassembly sometimes wrote that padding as an instruction, so the
    assembler put it in a code (`$t`) mapping region. objdiff then decodes
    it as `lsl r0, #0x0` while agbcc's own `.align 2, 0` gives the base
    object a data halfword, and the function misses 100% on that one row.
    Only a `movs r0, r0` right before a word-aligned pool word counts."""
    out = list(lines)
    for i, line in enumerate(out):
        if not MOVS_R0_R0.match(line):
            continue
        j = i + 1
        while j < len(out) and not out[j].strip():
            j += 1
        m = POOL_WORD.match(out[j]) if j < len(out) else None
        if m and int(m.group(1), 16) % 4 == 0:
            out[i] = "\t.align 2, 0\n"
    return out


if __name__ == "__main__":
    main()
