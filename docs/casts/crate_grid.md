# CrateList::nodes and ResetGrid's view (#855)

`CrateList::nodes` is a `CrateGridNode *`, and 5 of its 7 reads cast it to
`CrateGridNodeInit *`: ResetGrid zeroes each node through that view.

## Why the view stays

`CrateGridNodeInit` has `CrateGridNode`'s layout, but `data`, `next` and
`link` are `void *`. In the ROM, the loop body reloads `nodes` twice:

```
ldr   r0, [r4, #12]   ; nodes
adds  r0, r0, r6
str   r0, [r1]        ; (*fl)[i].node = &nodes[i]
ldr   r0, [r4, #12]   ; nodes again: that store was a CrateGridNode *
adds  r0, r6, r0
str   r7, [r0]        ; data = 0
str   r7, [r0, #4]    ; next = 0
str   r7, [r0, #12]   ; link = 0
strb  r7, [r0, #16]   ; mark = 0
ldr   r0, [r4, #12]   ; nodes again: a byte store may alias anything
adds  r0, r6, r0
...
str   r1, [r0, #8]    ; wrap = &links[i]
```

gcc reloads `nodes` after a store that may alias it. A store of a
`CrateGridNode *` may alias the field `nodes`, which has that type; the
stores of `void *`, which have a different alias set, don't. Through
`nodes[i].next = 0` and `nodes[i].link = 0`, gcc reloads `nodes` after
each of them and crate_list_reset.o and crate_list_update.o change. The
ROM's code was built from stores that weren't `CrateGridNode *`.

A local `CrateGridNode *node = &nodes[i]` doesn't need the reloads at
all, so the ROM's third load (after the `strb`) goes, and the objects
change too.

## The view, by name

The 5 casts are one now, in a CrateList accessor before ResetGrid, with
a `codegen:` comment:

```c++
CrateGridNodeInit *InitView()
{
    return (CrateGridNodeInit *)nodes;
}
```

ResetGrid writes `InitView()[i].data = 0`, ... Each call reads `nodes`
again, as the casts did, and crate_list_reset.o and crate_list_update.o
(the two objects that inline ResetGrid) are the same.

InitView is defined before ResetGrid in the class. Defined after it, it
gives different code in both objects.

`--casts`: the 5 casts on `CrateList::nodes` are 1.
