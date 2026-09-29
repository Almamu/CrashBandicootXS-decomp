//
//! \file fi.h
//!  FreeImage-free replacement for grit's extlib/fi.h (crash-decomp).
//
// Upstream grit loads/saves images through FreeImage. FreeImage is not
// available in this repo's build environment, so this shim routes the
// cldib load/save hooks through cldib's own libpng-based CPngFile
// (cldib/cldib_png.cpp) instead. Only PNG input/output is supported.

#ifndef __FI_EX_H__
#define __FI_EX_H__

#include <cldib.h>

void fiInit();

CLDIB *cldib_load(const char *fpath, void *extra);
bool cldib_save(const CLDIB *dib, const char *fpath, void *extra);

#endif // __FI_EX_H__
