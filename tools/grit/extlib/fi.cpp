//
//! \file fi.cpp
//!  FreeImage-free replacement for grit's extlib/fi.cpp (crash-decomp).
//
// Upstream grit reads and writes images through FreeImage, which is not in
// this repo's build environment (nor in current nixpkgs). This file
// implements the two cldib load/save hooks grit uses directly on top of
// libpng instead, producing the same CLDIB layout upstream's fi2dib() did:
// top-down rows, DWORD-aligned pitch, RGBQUAD (BGRx) palette.
//
// Supported input: PNG only. Paletted and grayscale images keep their bit
// depth (1/4/8; 2bpp is widened to 4bpp, like FreeImage does), truecolor
// becomes 24bpp (alpha is dropped). Output (-fx shared tileset only) is a
// paletted/truecolor PNG.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <png.h>

#include <cldib.h>

#include "fi.h"

void fiInit()
{
	dib_set_load_proc(cldib_load);
	dib_set_save_proc(cldib_save);
}

CLDIB *cldib_load(const char *fpath, void *extra)
{
	FILE *fp= fopen(fpath, "rb");
	if(fp == NULL)
		return NULL;

	png_structp png= png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info= png ? png_create_info_struct(png) : NULL;
	CLDIB *dib= NULL;
	png_bytep *rows= NULL;

	if(info == NULL || setjmp(png_jmpbuf(png)))
	{
		fprintf(stderr, "grit: can't read '%s' (only PNG input is supported)\n", fpath);
		free(rows);
		if(dib)
			dib_free(dib);
		png_destroy_read_struct(&png, &info, NULL);
		fclose(fp);
		return NULL;
	}

	png_init_io(png, fp);
	png_read_info(png, info);

	int w= png_get_image_width(png, info);
	int h= png_get_image_height(png, info);
	int depth= png_get_bit_depth(png, info);
	int ctype= png_get_color_type(png, info);
	int bpp;

	if(depth == 16)
		png_set_strip_16(png);

	if(ctype == PNG_COLOR_TYPE_PALETTE || ctype == PNG_COLOR_TYPE_GRAY)
	{
		if(depth == 2)
		{
			png_set_packing(png);	// 2bpp -> 8bpp indices, repacked to 4 below
			bpp= 4;
		}
		else
			bpp= depth > 8 ? 8 : depth;
	}
	else
	{
		if(ctype == PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_to_rgb(png);
		png_set_strip_alpha(png);
		png_set_bgr(png);
		bpp= 24;
	}
	png_read_update_info(png, info);

	dib= dib_alloc(w, h, bpp, NULL, true);
	if(dib == NULL)
		png_error(png, "out of memory");

	// Palette
	int nclrs= dib_get_nclrs(dib);
	RGBQUAD *pal= dib_get_pal(dib);
	if(ctype == PNG_COLOR_TYPE_PALETTE)
	{
		png_colorp plte;
		int nplte= 0;
		png_get_PLTE(png, info, &plte, &nplte);
		for(int ii=0; ii<nplte && ii<nclrs; ii++)
		{
			pal[ii].rgbRed= plte[ii].red;
			pal[ii].rgbGreen= plte[ii].green;
			pal[ii].rgbBlue= plte[ii].blue;
			pal[ii].rgbReserved= 0;
		}
	}
	else if(ctype == PNG_COLOR_TYPE_GRAY)
	{
		int max= (1<<(depth > 8 ? 8 : depth))-1;
		for(int ii=0; ii<nclrs; ii++)
		{
			BYTE g= (BYTE)(ii*255/max);
			pal[ii].rgbRed= pal[ii].rgbGreen= pal[ii].rgbBlue= g;
			pal[ii].rgbReserved= 0;
		}
	}

	// Pixels: libpng rows are top-down, as is the dib (bTopDown=true).
	int pitch= dib_get_pitch(dib);
	BYTE *img= dib_get_img(dib);
	int rowbytes= png_get_rowbytes(png, info);
	rows= (png_bytep*)malloc(h*sizeof(png_bytep));
	BYTE *tmp= NULL;
	if(depth == 2 && bpp == 4)
		tmp= (BYTE*)malloc((size_t)rowbytes*h);
	for(int iy=0; iy<h; iy++)
		rows[iy]= tmp ? tmp + (size_t)iy*rowbytes : img + (size_t)iy*pitch;
	png_read_image(png, rows);
	png_read_end(png, NULL);

	if(tmp)
	{
		for(int iy=0; iy<h; iy++)
		{
			BYTE *dst= img + (size_t)iy*pitch, *src= tmp + (size_t)iy*rowbytes;
			for(int ix=0; ix<w; ix++)
			{
				if(ix&1)	dst[ix>>1] |= src[ix]&15;
				else		dst[ix>>1]  = (BYTE)(src[ix]<<4);
			}
		}
		free(tmp);
	}

	free(rows);
	png_destroy_read_struct(&png, &info, NULL);
	fclose(fp);
	return dib;
}

bool cldib_save(const CLDIB *cdib, const char *fpath, void *extra)
{
	CLDIB *dib= (CLDIB*)cdib;
	int w= dib_get_width(dib), h= dib_get_height(dib), bpp= dib_get_bpp(dib);
	if(bpp != 1 && bpp != 4 && bpp != 8 && bpp != 24)
	{
		fprintf(stderr, "grit: can't save %d bpp image '%s'\n", bpp, fpath);
		return false;
	}

	FILE *fp= fopen(fpath, "wb");
	if(fp == NULL)
		return false;

	png_structp png= png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info= png ? png_create_info_struct(png) : NULL;
	if(info == NULL || setjmp(png_jmpbuf(png)))
	{
		png_destroy_write_struct(&png, &info);
		fclose(fp);
		return false;
	}
	png_init_io(png, fp);

	if(bpp <= 8)
	{
		png_set_IHDR(png, info, w, h, bpp, PNG_COLOR_TYPE_PALETTE,
			PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
		int nclrs= dib_get_nclrs(dib);
		RGBQUAD *pal= dib_get_pal(dib);
		png_color plte[256];
		for(int ii=0; ii<nclrs; ii++)
		{
			plte[ii].red= pal[ii].rgbRed;
			plte[ii].green= pal[ii].rgbGreen;
			plte[ii].blue= pal[ii].rgbBlue;
		}
		png_set_PLTE(png, info, plte, nclrs);
	}
	else
	{
		png_set_IHDR(png, info, w, h, 8, PNG_COLOR_TYPE_RGB,
			PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
		png_set_bgr(png);
	}
	png_write_info(png, info);

	int pitch= dib_get_pitch(dib);
	BYTE *img= dib_get_img(dib);
	bool topdown= dib_is_topdown(dib);
	for(int iy=0; iy<h; iy++)
		png_write_row(png, img + (size_t)(topdown ? iy : h-1-iy)*pitch);
	png_write_end(png, NULL);

	png_destroy_write_struct(&png, &info);
	fclose(fp);
	return true;
}

// EOF
