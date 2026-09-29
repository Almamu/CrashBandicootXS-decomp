# grit (vendored)

GBA Raster Image Transmogrifier by Jasper Vijn (cearn), as maintained by
devkitPro. It's used here to convert PNGs into GBA pixel layouts (bitmap, tiles,
tilemaps). See docs/graphics.md, "PNG -> C const array (grit)", for how the
build uses it and why its own LZ77 compressor isn't used.

- Upstream: https://github.com/devkitPro/grit
- Version: tag `v0.10.0`, commit `5209ac206360dacf2a2e64d5a6a60ea3a38f512e`
  (upstream's `configure.ac` still says 0.9.2; the Makefile passes 0.10.0).
- Built by the top-level Makefile (`make -C tools/grit`) the same way as
  `tools/gbagfx`. It needs a C++ compiler and libpng, both already in the
  devshell and in CI. No FreeImage and no autotools.

## License

Upstream ships `COPYING` (GNU GPL v2) and `licence-mit.txt` (MIT, (c) 2007
J Vijn), both copied here unchanged. Its source files carry no per-file notice.
It also ships `license-fi.txt`, the FreeImage Public License, which covers
FreeImage and not grit. This copy doesn't use FreeImage, so that file isn't
included. Either license allows redistributing the source with modifications
as long as the license texts are kept and the changes are marked, which this
file does. grit is a build-time host tool, so no grit code ends up in the ROM.

## What was taken

Only the files the command-line tool compiles: `cldib/` (minus the unused
BMP/PCX/TGA/PNG file classes and test code), `libgrit/` (minus the obsolete
`grit_{lz,huff,rle}.cpp` duplicates and docs), `srcgrit/`, and `extlib/fi.*`
(rewritten, see below). Also `grit_version.h`, the license texts,
and `grit-readme.txt` (upstream's option reference).

## Local changes (marked `crash-decomp` in the source)

1. **`extlib/fi.cpp`, `extlib/fi.h`**: rewritten. Upstream loads and saves
   images through FreeImage, which isn't in the devshell. It also isn't in the
   devshell's pinned nixpkgs at all. The two
   `cldib_load`/`cldib_save` hooks grit calls are now implemented directly on
   libpng. They produce the same CLDIB layout upstream's `fi2dib()` did:
   top-down rows, DWORD pitch, BGRx palette. Only PNG is supported.
   - Paletted and grayscale PNGs keep their bit depth (1/4/8; 2bpp is widened
     to 4bpp, like FreeImage does).
   - Truecolor PNGs become 24bpp.
2. **`cldib/winglue.h`**: the Windows-style base types (`BYTE`, `DWORD`,
   `RGBQUAD`, `BITMAPINFOHEADER`, ...) were pulled in from `<FreeImage.h>`.
   They're now defined inline with FreeImage's layout.
3. **`srcgrit/grit_main.cpp`**: removed the `<FreeImage.h>` include, the
   `FreeImage_Initialise`/`DeInitialise` calls, and the FreeImage-based
   "can this format hold 8bpp" check for `-fx` (the external tileset is
   always written as PNG, which can).
4. **`Makefile`**: a plain make build that replaces `configure.ac`/`Makefile.am`.

Nothing in the conversion or compression code (`libgrit/`, `cldib/`) was
changed.
