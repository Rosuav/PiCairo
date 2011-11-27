/* -*- Mode: c; indent-tabs-mode: nil; c-basic-offset: 2; c-file-style: "gnu" -*-
 *
 * pike bindings for Cairo (http://www.cairographics.org)
 *
 * Started 2006-02-03
 * Copyright (C) 2006, David Vest <davve@davve.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it either under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * (the "LGPL") or, at your option, under the terms of the Mozilla
 * Public License Version 1.1 (the "MPL"). If you do not alter this
 * notice, a recipient may use your version of this file under either
 * the MPL or the LGPL.
 *
 * You should have received a copy of the LGPL along with this library
 * in the file COPYING-LGPL-2.1; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * You should have received a copy of the MPL along with this library
 * in the file COPYING-MPL-1.1
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
 * OF ANY KIND, either express or implied. See the LGPL or the MPL for
 * the specific language governing rights and limitations.
 */

#include "cairomod.h"
#include "image.h"

/*! @module Cairo
 */

/*! @class Surface
 */

#ifdef HAVE_CAIRO

#define THIS_SURFACE ((struct cairo_mod_surface *)Pike_fp->current_storage)

struct program *cairo_mod_surface_program = NULL;

static void init_cairo_mod_surface(struct object *o)
{
  struct cairo_mod_surface *cs = THIS_SURFACE;
  cs->surface = NULL;
  cs->data = NULL;
  cs->format = 0;
}

static void exit_cairo_mod_surface(struct object *o)
{
  struct cairo_mod_surface *cs = THIS_SURFACE;
  if (cs->surface)
    {
      cairo_surface_destroy (cs->surface);
      free(cs->data);
    }
}

static void f_surface_create(INT32 args)
{
  Pike_error("Do not instantiate this class, use one of the derived variants");
}

/*! @decl void write_to_png(string filename)
 *!
 *! Write an image surface to file in PNG format. This method is
 *! availible if Cairo has been built with PNG support.
 *!
 *! @note
 *! The recommended way to export to image-file is through the
 *! @[ImageSurface.get_image] the method of the ImageSurface
 *! class. This way all pikes image encodings can be used.
 */
#if CAIRO_HAS_PNG_FUNCTIONS
static void f_surface_write_to_png(INT32 args)
{
  struct cairo_mod_surface *cs = THIS_SURFACE;
  struct pike_string *data;
  get_all_args("write_to_png", args, "%S", &data);
  int ret = cairo_surface_write_to_png(cs->surface, (char *)data->str);
  pop_n_elems(args);
}
#endif // CAIRO_HAS_PNG_FUNCTIONS

/*! @endclass */

/*! @class ImageSurface
 */

static struct program *image_program = NULL;

static void assert_image_program()
{
  if( !image_program )
  {
    push_text( "Image.Image" );
    APPLY_MASTER( "resolv", 1 );
    image_program = program_from_svalue( Pike_sp - 1  );
    Pike_sp--;/* Do not free image program.. */
  }
  if( !image_program )
    Pike_error("No Image.Image?!\n");
}

static void rgb_groups_to_image_surface(rgb_group * src, int width, int height, unsigned char* dest)
{
  int pixels = width * height;
  int i;
  unsigned int* dest_int = (unsigned int*)dest;
  unsigned char alpha, r, g, b;
  rgb_group* group;

  for (i=0;i<pixels;++i)
    {
      group = &src[i];
      r = group->r;
      g = group->g;
      b = group->b;
      dest_int[i] = (0xff << 24) | (r << 16) | (g << 8) | b;
    }
}

/*! @decl void create(Image.Image|int image_or_format, int| void width, int|void height)
 */
static void f_image_surface_create(INT32 args)
{
  struct cairo_mod_surface *cs = THIS_SURFACE;
  struct object *o;
  INT_TYPE format, w, h;
  int data_size;
  struct image *im;
  rgb_group *r;

  if (args == 1 && Pike_sp[-1].type == T_OBJECT)
    {
      get_all_args("create", args, "%o", &o);
      pop_n_elems(args);

      assert_image_program();

      im = (struct image*)get_storage(o, image_program);
      r = im->img;
      data_size = im->xsize * im->ysize * 4;
     
      cs->data = malloc(data_size);
      cs->format = CAIRO_FORMAT_ARGB32;
      rgb_groups_to_image_surface(r, im->xsize, im->ysize, cs->data);
      cs->surface = cairo_image_surface_create_for_data(cs->data,
                                                        cs->format,
                                                        im->xsize,
                                                        im->ysize,
                                                        im->xsize*4);
    }
  else if (args == 3)
    {
      get_all_args("create", args, "%d%d%d", &format, &w, &h);
      pop_n_elems(args);
      
      switch(format)
        {
        case CAIRO_FORMAT_ARGB32:
        case CAIRO_FORMAT_RGB24:
          data_size = w * h * 4;
          break;
        case CAIRO_FORMAT_A8:
          data_size = w * h;
          break;
        case CAIRO_FORMAT_A1:
          data_size = ((w * h) / 8) + 1;
          break;
        default:
          Pike_error("Unknown format");
          break;
        }

      cs->data = malloc(data_size);
      memset(cs->data, 0, data_size);
      cs->format = format;
      cs->surface = cairo_image_surface_create_for_data(cs->data,
                                                        format, w, h,
                                                        w*4);
    }
  else
    Pike_error("Wrong arguments to create()");
}

/*! @decl int get_width()
 */
static void f_image_surface_get_width(INT32 args)
{
  struct cairo_mod_surface *cs = THIS_SURFACE;
  INT_TYPE width = cairo_image_surface_get_width(cs->surface);
  push_int(width);
}

/*! @decl int get_height()
 */
static void f_image_surface_get_height(INT32 args)
{
  struct cairo_mod_surface *cs = THIS_SURFACE;
  INT_TYPE height = cairo_image_surface_get_height(cs->surface);
  push_int(height);
}

static void rgb_image_surface_to_rgb_groups(unsigned char* src, int width, int height, rgb_group * dest, rgb_group* alpha)
{
  int pixels = width * height;
  int i;
  unsigned int* src_int = (unsigned int*)src;
  unsigned int pixel;
  unsigned char a, r, g, b;
  rgb_group* group;

  for (i=0;i<pixels;++i)
    {
      pixel = src_int[i];
      a = ((pixel & 0xff000000) >> 24) & 0xff;
      r = ((pixel & 0x00ff0000) >> 16) & 0xff;
      g = ((pixel & 0x0000ff00) >> 8) & 0xff;
      b = (pixel & 0x000000ff);

      group = &dest[i];
      group->r = r;
      group->g = g;
      group->b = b;

      group = &alpha[i];
      group->r = a;
      group->g = a;
      group->b = a;
    }
}

/*! @decl mapping get_image()
 *!
 *! @note
 *! The surface is automatically @[flush]ed, so there are no
 *! need to call @[flush] before this function.
 *!
 *! @returns
 *!   @mapping
 *!     @member Image.Image "image"
 *!       The RGB representation of the image
 *!     @member Image.Image "alpha"
 *!       The alpha channel of the image
 *!   @endmapping
 *!
 *! @example
 *!   mapping(string:Image.Image) image = surface->get_image();
 *!   Image.Image im = image["image"];
 *!   Image.Image alpha = image["alpha"];
 *!   Stdio.write_file("output.png",
 *!                    Image.PNG.encode(im, ([ "alpha" : alpha ])));
 */
static void f_image_surface_get_image(INT32 args)
{
  struct cairo_mod_surface *cs = THIS_SURFACE;
  struct object *image, *alpha;
  rgb_group *r, *a;
  int width = cairo_image_surface_get_width(cs->surface);
  int height = cairo_image_surface_get_height(cs->surface);
  int x, y;

  assert_image_program();

  push_int(width);
  push_int(height);
  image = clone_object(image_program, 2);
  r = ((struct image *)image->storage)->img;
  push_int(width);
  push_int(height);
  alpha = clone_object(image_program, 2);
  a = ((struct image *)alpha->storage)->img;

  cairo_surface_flush(cs->surface);

  switch(cs->format)
    {
    case CAIRO_FORMAT_ARGB32:
    case CAIRO_FORMAT_RGB24:
      rgb_image_surface_to_rgb_groups(cs->data, width, height, r, a);
      break;
    default:
      Pike_error("Wrong format of surface. Only color surfaces (RGBA32 and RGB24) are supported");
    }

  push_text( "image" ); push_object(image);
  push_text( "alpha" ); push_object(alpha);
  f_aggregate_mapping(4);
}

/*! @endclass */

void cairo_mod_init_surface()
{
  struct svalue prog;
  prog.type = PIKE_T_PROGRAM;
  prog.subtype = 0;

  /*! @decl constant CAIRO_FORMAT_ARGB32 
   *! Each pixel is a 32-bit quantity, with alpha in the upper 8
   *! bits, then red, then green, then blue.  The 32-bit quantities
   *! are stored native-endian. Pre-multiplied alpha is used. (That
   *! is, 50% transparent red is 0x80800000, not 0x80ff0000.)
   *!
   *! @decl constant CAIRO_FORMAT_RGB24 
   *! Each pixel is a 32-bit quantity, with the upper 8 bits
   *! unused. Red, Green, and Blue are stored in the remaining 24
   *! bits in that order.
   *!
   *! @decl constant CAIRO_FORMAT_A8
   *! Each pixel is a 8-bit quantity holding an alpha value.
   *!
   *! @decl constant CAIRO_FORMAT_A1
   *! Each pixel is a 1-bit quantity holding an alpha value. Pixels
   *! are packed together into 32-bit quantities. The ordering of
   *! the bits matches the endianess of the platform. On a
   *! big-endian machine, the first pixel is in the uppermost bit,
   *! on a little-endian machine the first pixel is in the
   *! least-significant bit.
   */
  ADD_INT_CONSTANT("CAIRO_FORMAT_ARGB32", CAIRO_FORMAT_ARGB32, 0);
  ADD_INT_CONSTANT("CAIRO_FORMAT_RGB24", CAIRO_FORMAT_RGB24, 0);
  ADD_INT_CONSTANT("CAIRO_FORMAT_A8", CAIRO_FORMAT_A8, 0);
  ADD_INT_CONSTANT("CAIRO_FORMAT_A1", CAIRO_FORMAT_A1, 0);

  /*! @decl constant CAIRO_CONTENT_COLOR
   *! @decl constant CAIRO_CONTENT_ALPHA
   *! @decl constant CAIRO_CONTENT_COLOR_ALPHA
   *!
   *! The type of contents in Cairo's image surfaces
   */
  ADD_INT_CONSTANT("CAIRO_CONTENT_COLOR", CAIRO_CONTENT_COLOR, 0);
  ADD_INT_CONSTANT("CAIRO_CONTENT_ALPHA", CAIRO_CONTENT_ALPHA, 0);
  ADD_INT_CONSTANT("CAIRO_CONTENT_COLOR_ALPHA", CAIRO_CONTENT_COLOR_ALPHA, 0);

  start_new_program();
  {
    ADD_STORAGE(struct cairo_mod_surface);
    ADD_FUNCTION("create", f_surface_create, tFunc(tNone,tVoid), ID_STATIC);
#ifdef CAIRO_HAS_PNG_FUNCTIONS
    ADD_FUNCTION("write_to_png", f_surface_write_to_png, tFunc(tString,tInt), ID_PUBLIC);
#endif // CAIRO_HAS_PNG_FUNCTIONS

    set_init_callback(init_cairo_mod_surface);
    set_exit_callback(exit_cairo_mod_surface);
  }
  add_program_constant("Surface",
                       cairo_mod_surface_program = end_program(),
                       0);

  /* Cairo.ImageSurface */
  start_new_program();
  {
    prog.u.program = cairo_mod_surface_program;
    do_inherit(&prog, 0, NULL);
    ADD_FUNCTION("get_width", f_image_surface_get_width, tFunc(tNone,tInt), ID_PUBLIC);
    ADD_FUNCTION("get_height", f_image_surface_get_height, tFunc(tNone,tInt), ID_PUBLIC);
    ADD_FUNCTION("create", f_image_surface_create, tFunc(tOr(tInt,tObj) tOr(tInt,tVoid) tOr(tInt,tVoid),tVoid), ID_STATIC);
    ADD_FUNCTION("get_image", f_image_surface_get_image, tFunc(tNone,tMapping), ID_PUBLIC);
  }
  add_program_constant("ImageSurface",
                       cairo_mod_image_surface_program = end_program(),
                       0);
}

/*! @endmodule */

#endif // HAVE_CAIRO
