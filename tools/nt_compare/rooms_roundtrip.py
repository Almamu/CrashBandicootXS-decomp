#!/usr/bin/env python3
"""Run tools/levels.py's room codec over another ROM's rooms.

Walks gLevelTable -> room lists -> level_room -> level_desc with the XS
layouts, and for every room: decodes each layer's chunks, re-encodes the
layer with levels.py (split_chunks + encode_section) and compares the
bytes; checks the grid, the entity-list invariants of docs/levels.md
("Entities", "ROM layout") and records which entity types are used.

Usage: python3 tools/nt_compare/rooms_roundtrip.py <rom.gba> <gLevelTable> [levels]

Without a level count, the table ends at the first entry that isn't a
level (its room list isn't a ROM pointer to 1-8 rooms).
"""
import collections, struct, sys
sys.path.insert(0, 'tools')
import levels as L

rom = L.Rom(sys.argv[1]); LT = int(sys.argv[2], 16)
N = int(sys.argv[3]) if len(sys.argv) > 3 else 1 << 10
isrom = lambda p: 0x08000000 <= p < 0x08000000 + len(rom.data)

rooms = {}
for i in range(N):
    rl = rom.w(LT + i * 0x24 + 0x20)
    if not isrom(rl) or not 1 <= rom.w(rl) <= 8 or not isrom(rom.w(rl + 4)):
        break
    cnt, lst, x1, x2 = rom.w(rl), rom.w(rl + 4), rom.w(rl + 8), rom.w(rl + 12)
    for r in [rom.w(lst + 4 * k) for k in range(cnt)] + [x1, x2]:
        if isrom(r) and rom.w(r + 4):
            rooms.setdefault(r, (i, rom.w(r), rom.w(r + 4), rom.w(r + 8)))

tot = collections.Counter(); types = collections.Counter(); kinds = collections.Counter()
for r, (lvl, pal, desc, kind) in sorted(rooms.items(), key=lambda x: x[1][2]):
    kinds[kind] += 1
    D = [rom.w(desc + 4 * k) for k in range(9)]
    packed = rom.b(desc + 0x18)
    problems = []
    if rom.bytes(desc + 0x19, 3) != b'\0\0\0' or any(rom.bytes(desc + 0x24, 12)):
        problems.append('desc padding')
    asset = L.lz77_decompress(rom.bytes(D[5], 0x40000)) if packed else rom.bytes(D[5], 0x80000)
    layer_ok = 0; nlayers = 0; chunks_total = 0; end = 0
    for slot in range(5):
        p = D[slot]
        if not p: continue
        nlayers += 1
        grid_addr, off = rom.w(p), rom.w(p + 4)
        cw, ch = rom.h(p + 0x16), rom.h(p + 0x18)
        grid = [rom.h(grid_addr + 2 * k) for k in range(cw * ch)]
        n = max(grid) + 1
        table = struct.unpack_from('<%dH' % n, asset, off)
        chunks = []
        try:
            for k in range(n):
                cells, e = L.decode_chunk(asset, off + 4 * table[k])
                chunks.append(cells); end = max(end, e)
        except Exception as ex:
            problems.append('slot %d decode: %s' % (slot, ex)); continue
        chunks_total += n
        width = cw * L.CHUNK_W
        cells = [0] * (width * ch * L.CHUNK_H)
        for gy in range(ch):
            for gx in range(cw):
                c = chunks[grid[gy * cw + gx]]
                for y in range(L.CHUNK_H):
                    o = (gy * L.CHUNK_H + y) * width + gx * L.CHUNK_W
                    cells[o:o + L.CHUNK_W] = c[y * L.CHUNK_W:(y + 1) * L.CHUNK_W]
        re_chunks, re_grid = L.split_chunks(cells, cw, ch)
        if re_grid != grid:
            problems.append('slot %d grid not first-appearance order' % slot); continue
        sec = L.encode_section(re_chunks)
        orig = asset[off:off + len(sec)]
        if sec == orig:
            layer_ok += 1
        else:
            # how many chunks differ only in the leftover pad bytes
            bad = sum(1 for k in range(n) if L.encode_chunk(list(re_chunks[k]))[0] !=
                      asset[off + 4 * table[k]: off + 4 * table[k] + len(L.encode_chunk(list(re_chunks[k]))[0])])
            diff = sum(1 for a, b in zip(sec, orig) if a != b)
            problems.append('slot %d re-encode: %d bytes differ, %d/%d chunk streams differ' % (slot, diff, bad, n))
    # entities
    el = D[7]
    count, gcount = rom.h(el), rom.h(el + 2)
    groups, poffs, params, tcounts = rom.w(el + 4), rom.w(el + 8), rom.w(el + 12), rom.w(el + 16)
    ents = []
    for g in range(gcount):
        first, c, ep = rom.h(groups + 8 * g), rom.h(groups + 8 * g + 2), rom.w(groups + 8 * g + 4)
        for k in range(c):
            t, x, y, prm = struct.unpack_from('<4H', rom.data, ep - 0x08000000 + 8 * k)
            ents.append(t)
            if x >> L.GROUP_SHIFT != g: problems.append('entity column'); break
    if len(ents) != count: problems.append('entity count %d != %d' % (len(ents), count))
    hist = collections.Counter(ents)
    # the type count table's length: up to the next object (the links or the desc)
    tc = [rom.h(tcounts + 2 * k) for k in range(max(hist) + 1)]
    if any(tc[k] != hist.get(k, 0) for k in range(len(tc))): problems.append('typeCounts')
    ntypes = (min(x for x in (D[8], desc) if x and x > tcounts) - tcounts) // 2
    types.update(hist)
    tot['rooms'] += 1; tot['layers'] += nlayers; tot['layers_ok'] += layer_ok; tot['chunks'] += chunks_total
    tot['ok'] += not problems
    print(f"L{lvl:02} desc={desc:08x} kind={kind} packed={packed} layers {layer_ok}/{nlayers} "
          f"chunks={chunks_total} entities={count} typeCounts[{ntypes}] "
          + ('OK' if not problems else '; '.join(problems)))
print('%d levels' % i, dict(tot), 'room kinds', dict(kinds))
print('entity types used:', ' '.join('%#x' % t for t in sorted(types)))
