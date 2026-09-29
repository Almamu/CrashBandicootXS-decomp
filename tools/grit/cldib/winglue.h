//
//! \file winglue.h
//!  Required windows stuff for portability's sake
//!  Mostly ripped from FreeImage.h
//! \date 20060725 - 20070403
//! \author cearn
//=== NOTES ===
// * The switch-define is either _MSC_VER or _WIN32, I'm not sure which. 

#ifndef __WINGLUE_H__
#define __WINGLUE_H__


#ifdef _MSC_VER

// --------------------------------------------------------------------
// WINDOWS/Visual C++ platform
// --------------------------------------------------------------------

#include <windows.h>

#ifndef PATH_MAX
#define PATH_MAX _MAX_PATH
#endif

// <sys/param.h> functionality
#ifndef _SYS_PARAM_H
#define _SYS_PARAM_H

#include <limits.h>

/* These are useful for cross-compiling */ 
#define BIG_ENDIAN      4321
#define LITTLE_ENDIAN   1234
#define BYTE_ORDER      LITTLE_ENDIAN

#define MAXPATHLEN      PATH_MAX

#endif	// _SYS_PARAM_H

#define strcasecmp stricmp		// 


#else	// _MSC_VER


// --------------------------------------------------------------------
// NON-WINDOWS platform
// --------------------------------------------------------------------


// This defines BYTE_ORDER etc. If it doesn't exist, just C&P from 
// above
#include <sys/param.h>	

#include <stdlib.h>
#include <string.h>

#include <stdint.h>

// crash-decomp: the Windows-style base types below used to come from
// <FreeImage.h>; defined here (same layout as FreeImage's non-Windows
// definitions) so the vendored grit builds without FreeImage.
#ifndef FREEIMAGE_H
typedef int32_t BOOL;
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;

typedef struct tagRGBQUAD {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
} RGBQUAD;

typedef struct tagRGBTRIPLE {
	BYTE rgbtBlue;
	BYTE rgbtGreen;
	BYTE rgbtRed;
} RGBTRIPLE;

typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;
	LONG  biWidth;
	LONG  biHeight;
	WORD  biPlanes;
	WORD  biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG  biXPelsPerMeter;
	LONG  biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagBITMAPINFO {
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD          bmiColors[1];
} BITMAPINFO, *PBITMAPINFO;
#endif

// --- BASE TYPES -----------------------------------------------------

typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long UINT;
// rest is defined in FreeImage.h

typedef DWORD COLORREF;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

typedef struct _RECT
{
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECT;

// --- BITMAP STUFF ---------------------------------------------------
 

// --- INLINES --------------------------------------------------------

static inline COLORREF RGB( BYTE red, BYTE green, BYTE blue )
{
	return ( (blue << 16) | ( green << 8 ) | red );
}

static inline BYTE GetRValue( COLORREF color )
{
	return (BYTE)(color & 0xFF);
}

static inline BYTE GetGValue( COLORREF color )
{
	return (BYTE)(( color >> 8 )& 0xFF);
}

static inline BYTE GetBValue( COLORREF color )
{
	return (BYTE)(( color >> 16 ) & 0xFF);
}


#endif	// _MSC_VER


#endif	//__WINGLUE_H__

// EOF
