#!/usr/bin/env python3
"""Applies expected/corrections.txt to an assembled target object in place.

See expected/corrections.txt and docs/decomp_dev.md for why this exists:
expected/code_3.s (and expected/legacy.s) are frozen, never-edited
historical disassembly, so any function boundary or name they never
anticipated (because later matching work discovered/renamed it) has to
be patched onto the assembled object instead - never onto the source.

Also used per-file (see tools/report_units.py) against small slices of
those frozen sources, one per matched src/*.c file - the full
corrections.txt is passed every time, and entries that don't apply to
a given slice (the renamed symbol isn't in it, or the split address
falls outside its range) are silently skipped rather than erroring, so
the same file doesn't need per-slice filtering maintained by hand.

After the corrections, every function symbol also gets an explicit ELF
size (see set_function_sizes() below and docs/decomp_dev.md) - the frozen
sources have no `.size` directives, and objdiff's own size inference
can't tell a function's trailing alignment padding from the zero upper
half of its last literal-pool word. `--base-object` runs only that size
pass (grow-only) on a copy of a unit's compiled base object.
"""
import re
import struct
import subprocess
import sys

OBJCOPY = "arm-none-eabi-objcopy"
NM = "arm-none-eabi-nm"
READELF = "arm-none-eabi-readelf"


def text_base_address(obj_path):
    """Lowest-address global function symbol's ROM address, i.e. what
    file offset 0 in .text corresponds to. Derived from the symbol's own
    name (this project names every not-yet-renamed function sub_<hex
    address>) rather than hardcoded, so this keeps working if
    expected/code_3.s is ever regenerated from a different commit."""
    out = subprocess.run([NM, obj_path], capture_output=True, text=True, check=True).stdout
    lowest = None
    for line in out.splitlines():
        parts = line.split()
        if len(parts) != 3 or parts[1] not in ("T", "t"):
            continue
        offset = int(parts[0], 16)
        m = re.fullmatch(r"sub_([0-9A-Fa-f]{7,8})", parts[2])
        if not m:
            continue
        address = int(m.group(1), 16)
        base = address - offset
        if lowest is None or base < lowest:
            lowest = base
    return lowest


def text_section_size(obj_path):
    out = subprocess.run([READELF, "-S", obj_path], capture_output=True, text=True, check=True).stdout
    for line in out.splitlines():
        m = re.search(r"\.text\s+PROGBITS\s+[0-9a-f]+\s+[0-9a-f]+\s+([0-9a-f]+)", line)
        if m:
            return int(m.group(1), 16)
    sys.exit(f"patch_expected_target: couldn't find .text section size in {obj_path}")


def existing_symbols(obj_path):
    out = subprocess.run([NM, obj_path], capture_output=True, text=True, check=True).stdout
    names = set()
    for line in out.splitlines():
        parts = line.split()
        if len(parts) == 3:
            names.add(parts[2])
        elif len(parts) == 2:
            names.add(parts[1])
    return names


def main():
    argv = sys.argv[1:]
    base_override = None
    base_object = False
    for arg in list(argv):
        if arg.startswith("--base="):
            base_override = int(arg[len("--base="):], 16)
            argv.remove(arg)
        elif arg == "--base-object":
            base_object = True
            argv.remove(arg)

    if base_object:
        # A copy of a unit's compiled src/*.c object (see report_units.py):
        # only its sizes need fixing, never its names or boundaries.
        if len(argv) != 1:
            sys.exit(f"usage: {sys.argv[0]} --base-object <base_copy.o>")
        set_function_sizes(argv[0], grow_only=True)
        return

    if len(argv) != 2:
        sys.exit(f"usage: {sys.argv[0]} [--base=0xADDR] <corrections.txt> <target.o>")
    corrections_path, obj_path = argv

    base = base_override if base_override is not None else text_base_address(obj_path)
    size = text_section_size(obj_path)
    symbols = existing_symbols(obj_path)

    objcopy_args = []
    unlabelled = set()
    with open(corrections_path) as f:
        for lineno, raw_line in enumerate(f, 1):
            line = raw_line.split("#", 1)[0].strip()
            if not line:
                continue
            parts = line.split()
            if parts[0] == "rename" and len(parts) == 3:
                _, old, new = parts
                if old not in symbols:
                    continue
                objcopy_args += ["--redefine-sym", f"{old}={new}"]
            elif parts[0] == "split" and len(parts) == 3:
                _, address, name = parts
                if base is None:
                    continue
                offset = int(address, 16) - base
                if not (0 <= offset < size):
                    continue
                objcopy_args += ["--add-symbol", f"{name}=.text:{offset:#x},function,global"]
            elif parts[0] == "unlabel" and len(parts) == 2:
                if parts[1] in symbols:
                    unlabelled.add(parts[1])
            elif parts[0] == "resolve" and len(parts) == 2:
                continue  # applied to the text slice, see slice_expected.py
            elif parts[0] in ("code", "data") and len(parts) == 2:
                if base is None:
                    continue
                offset = int(parts[1], 16) - base
                if 0 <= offset < size:
                    mapping = "$t" if parts[0] == "code" else "$d"
                    objcopy_args += ["--add-symbol", f"{mapping}=.text:{offset:#x},local"]
            else:
                sys.exit(f"{corrections_path}:{lineno}: malformed line: {raw_line!r}")

    for name in sorted(unlabelled):
        # Dropped outright (objdiff would otherwise still list a label
        # sitting in the padding after a function as a 1-2 byte function);
        # objcopy keeps a symbol a relocation names, and localizing covers
        # that case.
        objcopy_args += ["--strip-symbol", name, "--localize-symbol", name]
    if objcopy_args:
        subprocess.run([OBJCOPY, *objcopy_args, obj_path], check=True)

    set_function_sizes(obj_path, unlabelled=unlabelled)


def set_function_sizes(obj_path, unlabelled=(), grow_only=False):
    """Gives every function symbol in .text an explicit st_size, in place.

    Target objects (assembled from the frozen expected/*.s, which has no
    `.size` directives at all) otherwise leave objdiff to infer each size
    from the next symbol, after which it trims trailing zero bytes as if
    they were alignment padding. That is wrong whenever a function ends
    with a literal-pool word whose upper halfword is zero (`.4byte
    0x000001FF`): the target loses its last two bytes, that word decodes
    as a `.hword`, and a byte-exact function scores 99.9% - which objdiff
    counts as not matched at all.

    Each function runs to the next function symbol (or the end of .text),
    minus a trailing zero halfword when that halfword really is padding:
    it has to sit at offset 2 mod 4 and must not be the upper half of a
    literal-pool word. The assembler's mapping symbols tell the two
    apart - `.align 2, 0` padding opens its own `$d` region (or sits in a
    `$t` region, where the disassembly wrote it as `movs r0, r0`), while a
    pool word's `$d` region started at the word, two bytes earlier.

    `unlabelled` names (the `unlabel` entries in expected/corrections.txt,
    already made local by objcopy) lose their function type: labels the
    frozen disassembly treated as a function start that the C source
    doesn't have (a return point in the middle of a function, or a
    `non_word_aligned_thumb_func_start` stub that is only padding), so
    their bytes go to the function before.

    `grow_only` is for base objects (report_units.py patches a copy of
    each unit's compiled src/*.c object): agbcc's own `.size` is right for
    plain C, but it stops before anything the assembler emitted after the
    function - the literal pool of an inline-asm `ldr rN, =sym`, or bytes
    a NAKED function wrote with `.byte` after its last label - and a
    NAKED function's own labels have no `.size` at all. Those sizes are
    only ever widened to the next function, never shrunk.
    """
    with open(obj_path, "rb") as f:
        data = bytearray(f.read())

    if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 1:
        sys.exit(f"patch_expected_target: {obj_path} is not a 32-bit little-endian ELF")
    e_shoff, = struct.unpack_from("<I", data, 0x20)
    e_shentsize, e_shnum, e_shstrndx = struct.unpack_from("<HHH", data, 0x2E)
    sections = [struct.unpack_from("<IIIIIIIIII", data, e_shoff + i * e_shentsize) for i in range(e_shnum)]

    def c_string(offset):
        return bytes(data[offset:data.index(b"\0", offset)]).decode()

    text_index = next((i for i, sh in enumerate(sections)
                       if c_string(sections[e_shstrndx][4] + sh[0]) == ".text"), None)
    symtab = next((sh for sh in sections if sh[1] == 2), None)  # SHT_SYMTAB
    if text_index is None or symtab is None:
        return
    text_offset, text_size = sections[text_index][4], sections[text_index][5]
    strtab_offset = sections[symtab[6]][4]

    functions = []  # (.text offset, symtab entry offset, current st_size)
    mapping = {}  # .text offset -> "$d"/"$t"/"$a"
    for entry in range(symtab[4], symtab[4] + symtab[5], symtab[9]):
        st_name, st_value, st_size, st_info, _, st_shndx = struct.unpack_from("<IIIBBH", data, entry)
        if st_shndx != text_index:
            continue
        name = c_string(strtab_offset + st_name)
        if name in unlabelled:
            struct.pack_into("<B", data, entry + 12, st_info & 0xF0)  # STT_NOTYPE
        elif st_info & 0xF == 2:  # STT_FUNC
            functions.append((st_value & ~1, entry, st_size))
        elif name in ("$d", "$t", "$a"):
            mapping[st_value] = name

    mapping_starts = sorted(mapping)

    def is_padding(pad):
        if pad % 4 != 2 or data[text_offset + pad:text_offset + pad + 2] != b"\0\0":
            return False
        region = [s for s in mapping_starts if s <= pad]
        return not region or mapping[region[-1]] != "$d" or region[-1] == pad

    starts = sorted({value for value, _, _ in functions})
    for value, entry, st_size in functions:
        later = [s for s in starts if s > value]
        end = later[0] if later else text_size
        if end - value >= 2 and is_padding(end - 2):
            end -= 2
        size = end - value
        if grow_only and size <= st_size:
            continue
        struct.pack_into("<I", data, entry + 8, size)

    with open(obj_path, "wb") as f:
        f.write(data)


if __name__ == "__main__":
    main()
