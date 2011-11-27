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
 *
 * The Original Code is the cairo binding module for Pike.
 *
 * The Initial Developer of the Original Code is David Vest
 * <davve@davve.net>.
 *
 * Contributor(s):
 *	None
 */

#include "cairomod.h"
#include "image.h"

#define CHECK_CAIRO_CONTEXT_ERROR(CC)           \
  do {                                          \
    cairo_status_t status;                      \
    status = cairo_status((CC));                \
    if (status != CAIRO_STATUS_SUCCESS)         \
      {                                         \
        cairo_pike_set_error(status);           \
        pike_throw();                           \
      }                                         \
  } while(0);                                   \

struct program *cairo_mod_context_program = NULL;
struct program *gtk2_cairo_mod_context_program = NULL;
struct program *cairo_mod_image_surface_program = NULL;
struct program *pgtk_window_program = NULL;

/*! @module Cairo
 *!
 *! This module provides binding to the Cairo 2D grapics library. See
 *! @url{http://www.cairographics.org/@} for more information about
 *! Cairo.
 *!
 */

/*! @class Context
 *!
 *! @[Cairo.Context] is the main object used when drawing with
 *! cairo. To draw with cairo, you create a @[Cairo.Context], set the
 *! target surface, and drawing options for the @[Cairo.Context],
 *! create shapes with functions like @[move_to] and @[line_to], and
 *! then draw shapes with @[stroke] or @[fill].
 *!
 *! @[Cairo.Context]'s can be pushed to a stack via @[save]. They may
 *! then safely be changed, without loosing the current state. Use
 *! @[restore] to restore to the saved state.
 */

#ifdef HAVE_CAIRO

#define THIS_CONTEXT ((struct cairo_mod_context *)Pike_fp->current_storage)

static void init_cairo_mod_context( struct object *o )
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  cc->ctx = NULL;
}

static void exit_cairo_mod_context( struct object *o )
{
  if (THIS_CONTEXT->ctx)
    {
      cairo_destroy(THIS_CONTEXT->ctx);
    }
}

/*! @decl void create(Cairo.Surface surface)
 *!
 *! Create a @[Cairo.Context] to a @[Cairo.Surface]. See below for a
 *! trivial example that draws a triangle to a image surface and saves
 *! the result to a png-file with alpha-channel.
 *!
 *! @example
 *!  // Draw a triangle to a png-file
 *!  Cairo.Surface surface = Cairo.ImageSurface(Cairo.CAIRO_FORMAT_ARGB32, 100, 100);
 *!  Cairo.Context cr = Cairo.Context(surface);
 *!  cr->move_to(0,0);
 *!  cr->line_to(100,100);
 *!  cr->line_to(0,100);
 *!  cr->close_path();
 *!  cr->set_source(0.4, 0.5, 0.6, 1.0);
 *!  cr->fill_preserve();
 *!  cr->set_source(1.0, 1.0, 1.0, 1.0);
 *!  cr->set_line_width(1.0);
 *!  cr->stroke();
 *!  mapping(string:Image.Image) image = surface->get_image();
 *!  Image.Image im = image["image"];
 *!  Image.Image alpha = image["alpha"];
 *!  Stdio.write_file("output.png",
 *!                   Image.PNG.encode(im, ([ "alpha" : alpha ])));
 */
static void f_context_create(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  struct object *o=NULL;
  struct cairo_mod_surface *sw=NULL;
  get_all_args("create", args, "%o", &o);
  pop_n_elems(args);

  if (!(sw = (struct cairo_mod_surface*)get_storage(o, cairo_mod_surface_program)))
    Pike_error("Object is not Surface\n");

  cc->ctx = cairo_create(sw->surface);
}

/*! @decl Cairo.Context move_to(int|float x, int|float y)
 *!
 *! If the current subpath is not empty, begin a new subpath. After
 *! this call the current point will be (@[x], @[y]).
 *!
 *! @param x
 *!  The X coordinate of the new position
 *! @param y
 *!  The Y coordinate of the new position
 */
static void f_move_to(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE x, y;
  get_all_args("move_to", args, "%F%F", &x, &y);
  pop_n_elems(args);
  cairo_move_to(cc->ctx, x, y);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context rel_move_to(int|float x, int|float y)
 *!
 *! If the current subpath is not empty, begin a new subpath. After
 *! this call the current point will be offset by (@[x], @[y]).
 *!
 *! @param x
 *!  The X offset
 *! @param y
 *!  The Y offset
 */
static void f_rel_move_to(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE dx, dy;
  get_all_args("rel_move_to", args, "%F%F", &dx, &dy);
  pop_n_elems(args);
  cairo_rel_move_to(cc->ctx, dx, dy);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context arc(int|float xc, int|float yc, int|float radius, int|float angle1, int|float angle2)
 *!
 *! Adds a circular arc of the given @[radius] to the current path.  The
 *! arc is centered at (@[xc], @[yc]), begins at @[angle1] and proceeds in
 *! the direction of increasing angles to end at @[angle2]. If @[angle2] is
 *! less than @[angle1] it will be progressively increased by 2*PI
 *! until it is greater than @[angle1].
 *!
 *! If there is a current point, an initial line segment will be added
 *! to the path to connect the current point to the beginning of the
 *! arc.
 *!
 *! Angles are measured in radians. An angle of 0 is in the direction
 *! of the positive X axis (in user-space). An angle of PI radians
 *! (90 degrees) is in the direction of the positive Y axis (in
 *! user-space). Angles increase in the direction from the positive X
 *! axis toward the positive Y axis. So with the default transformation
 *! matrix, angles increase in a clockwise direction.
 *!
 *! (To convert from degrees to radians, use @expr{degrees * (Math.pi /
 *! 180)@}.)
 *!
 *! This function gives the arc in the direction of increasing angles;
 *! see @[arc_negative] to get the arc in the direction of
 *! decreasing angles.
 *!
 *! The arc is circular in user-space. To achieve an elliptical arc,
 *! you can scale the current transformation matrix by different
 *! amounts in the X and Y directions. For example, to draw an ellipse
 *! in the box given by @tt{x@}, @tt{y@}, @tt{width@}, @tt{height@}:
 *!
 *! @example
 *! cr = Cairo.Context();
 *! cr->save();
 *! cr->translate(x + width / 2.0, y + height / 2.0);
 *! cr->scale(1.0 / (height / 2.0), 1.0 / (width / 2.0));
 *! cr->arc(0., 0.0, 1.0, 0.0, 2 * Math.pi);
 *! cr->restore();
 */
static void f_arc(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE x, y, a, b, c;
  get_all_args("arc", args, "%F%F%F%F%F", &x, &y, &a, &b, &c);
  pop_n_elems(args);
  cairo_arc(cc->ctx, x, y, a, b, c);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context arc_negative(int|float xc, int|float yc, int|float radius, int|float angle1, int|float angle2)
 *!
 *! Adds a circular arc of the given @[radius] to the current path. The
 *! arc is centered at (@[xc], @[yc]), begins at @[angle1] and proceeds in
 *! the direction of decreasing angles to end at @[angle2]. If @[angle2] is
 *! greater than @[angle1] it will be progressively decreased by [2*PI]
 *! until it is greater than @[angle1].
 *!
 *! See @[arc] for more details. This function differs only in the
 *! direction of the arc between the two angles.
 */
static void f_arc_negative(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE xc, yc, radius, angle1, angle2;
  get_all_args("arc", args, "%F%F%F%F%F", &xc, &yc, &radius, &angle1, &angle2);
  pop_n_elems(args);
  cairo_arc_negative(cc->ctx, xc, yc, radius, angle1, angle2);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context rel_line_to(int|float dx, int|float dy)
 */
static void f_rel_line_to(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE dx, dy;
  get_all_args("rel_line_to", args, "%F%F", &dx, &dy);
  pop_n_elems(args);
  cairo_rel_line_to(cc->ctx, dx, dy);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context line_to(int|float x, int|float y)
 */
static void f_line_to(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE x, y;
  get_all_args("line_to", args, "%F%F", &x, &y);
  pop_n_elems(args);
  cairo_line_to(cc->ctx, x, y);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context cairo_curve_to(float x1, float y1, @
 *!                           float x2, float y2, @
 *!                           float x3, float y3)
 *!
 *! @param x1
 *! the X coordinate of the first control point
 *! @param y1
 *! the Y coordinate of the first control point
 *! @param x2
 *! the X coordinate of the second control point
 *! @param y2
 *! the Y coordinate of the second control point
 *! @param x3
 *! the X coordinate of the end of the curve
 *! @param y3
 *! the Y coordinate of the end of the curve
 *!
 *! Adds a cubic Bezier spline to the path from the current point to
 *! position (@[x3], @[y3]) in user-space coordinates, using (@[x1], @[y1]) and
 *! (@[x2], @[y2]) as the control points. After this call the current point
 *! will be (@[x3], @[y3]).
 */
static void f_curve_to(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE x1, y1, x2, y2, x3, y3;
  get_all_args("curve_to", args, "%F%F%F%F%F%F",
               &x1, &y1, &x2, &y2, &x3, &y3);
  pop_n_elems(args);
  cairo_curve_to (cc->ctx, x1, y1,
                  x2, y2, x3, y3);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context cairo_rel_curve_to(float dx1, float dy1, @
 *!                               float dx2, float dy2, @
 *!                               float dx3, float dy3)
 */
static void f_rel_curve_to(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE dx1, dy1, dx2, dy2, dx3, dy3;
  get_all_args("rel_curve_to", args, "%F%F%F%F%F%F",
               &dx1, &dy1, &dx2, &dy2, &dx3, &dy3);
  pop_n_elems(args);
  cairo_rel_curve_to (cc->ctx, dx1, dy1,
                      dx2, dy2, dx3, dy3);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context close_path()
 */
static void f_close_path(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  cairo_close_path(cc->ctx);
  ref_push_object(THIS_OBJ);
}

#ifdef DYNAMIC_MODULE
struct program *image_color_program;
#else
program *image_color_program;
#endif

static void assert_image_color_program()
{
#ifdef DYNAMIC_MODULE
  if(!image_color_program)
  {
    image_color_program = PIKE_MODULE_IMPORT(Image, image_color_program);
    if(!image_color_program)
      {
        yyerror("Could not load Image module.");
        return;
      }
  }
#else
  Pike_error("No dynamic modules. No Image.Color submodule.\n");
#endif
}

/*! @decl Cairo.Context set_source(Image.Color color)
 *! @decl Cairo.Context set_source(Image.Color color, float a)
 *! @decl Cairo.Context set_source(Cairo.Pattern pattern)
 *! @decl Cairo.Context set_source(Cairo.Surface surface, float|int x, float|int y)
 *! @decl Cairo.Context set_source(float r, float g, float b)
 *! @decl Cairo.Context set_source(float r, float g, float b, float a)
 */
static void f_set_source(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  struct object* o;
  struct cairo_mod_pattern *cp;
  struct cairo_mod_surface* s;
  FLOAT_TYPE r, g, b, a, x, y;
  struct color_struct *col;

  if (args == 4)
    {
      get_all_args("set_source", args, "%F%F%F%F", &r, &g, &b, &a);
      pop_n_elems(args);
      cairo_set_source_rgba(cc->ctx, r, g, b, a);
    }
  else if (args == 3)
    {
      if (Pike_sp[-3].type == PIKE_T_OBJECT)
        {
          get_all_args("set_source", args, "%o%F%F", &o, &x, &y);
          pop_n_elems(args);
          if (!(s = (struct cairo_mod_surface *)get_storage(o, cairo_mod_surface_program)))
            SIMPLE_ARG_TYPE_ERROR("set_source", 0, "Image.Surface");
          cairo_set_source_surface(cc->ctx, s->surface, x, y);
        }
      else
        {
          get_all_args("set_source", args, "%F%F%F", &r, &g, &b);
          pop_n_elems(args);
          cairo_set_source_rgb(cc->ctx, r, g, b);
        }
    }
  else if (args == 2)
    {
      get_all_args("set_source", args, "%o%F", &o, &a);
      pop_n_elems(args);

      if (o->prog->id == PROG_IMAGE_COLOR_COLOR_ID)
        {
          assert_image_color_program();
          col = (struct color_struct*)get_storage(o, image_color_program);
          cairo_set_source_rgba(cc->ctx,
                                col->rgbl.r / (FLOAT_TYPE)COLORLMAX,
                                col->rgbl.g / (FLOAT_TYPE)COLORLMAX,
                                col->rgbl.b / (FLOAT_TYPE)COLORLMAX,
                                a);
        }
      else
        SIMPLE_ARG_TYPE_ERROR("set_source", 0, "Image.Color");
    }
  else if (args == 1)
    {
      get_all_args("set_source", args, "%o", &o);
      pop_n_elems(args);

      if (o->prog->id == PROG_IMAGE_COLOR_COLOR_ID)
        {
          assert_image_color_program();
          col = (struct color_struct*)get_storage(o, image_color_program);
          cairo_set_source_rgb(cc->ctx,
                               col->rgbl.r / (FLOAT_TYPE)COLORLMAX,
                               col->rgbl.g / (FLOAT_TYPE)COLORLMAX,
                               col->rgbl.b / (FLOAT_TYPE)COLORLMAX);
        }
      else
        {
          // we got pattern hopefully
          if (!(cp = (struct cairo_mod_pattern *)get_storage(o, cairo_mod_pattern_program)))
            SIMPLE_ARG_TYPE_ERROR ("set_source", 0, "Image.Color|Cairo.Pattern");
          cairo_set_source(cc->ctx, cp->pattern);
        }
    }

  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context fill_preserve()
 */
static void f_fill_preserve(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  cairo_fill_preserve(cc->ctx);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context fill()
 */
static void f_fill(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  cairo_fill(cc->ctx);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl void clip()
 */
static void f_clip(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  cairo_clip(cc->ctx);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
}

/*! @decl Cairo.Context clip_preserve()
 */
static void f_clip_preserve(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  cairo_clip_preserve(cc->ctx);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context reset_clip()
 */
static void f_reset_clip(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  cairo_reset_clip(cc->ctx);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context save()
 */
static void f_save(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  cairo_save(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl void restore()
 *!
 *! @note
 *! This function does not return its own object. This is because the
 *! state is cleared from the cairo context when restore is called, and
 *! nesting of this command makes little sense.
 */
static void f_restore(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  cairo_restore(cc->ctx);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
}

/*! @decl Cairo.Context translate(float|int tx, float|int ty)
 */
static void f_translate(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE tx, ty;
  get_all_args("translate", args, "%F%F", &tx, &ty);
  cairo_translate(cc->ctx, tx, ty);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context scale(float|int sx, float|int sy)
 */
static void f_scale(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE sx, sy;
  get_all_args("scale", args, "%F%F", &sx, &sy);
  cairo_scale(cc->ctx, sx, sy);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context rotate(float|int r)
 */
static void f_rotate(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE r;

  get_all_args("rotate", args, "%F", &r);
  cairo_rotate(cc->ctx, r);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context paint()
 */
static void f_paint(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  cairo_paint(cc->ctx);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context paint_with_alpha()
 */
static void f_paint_with_alpha(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE alpha;
  get_all_args("paint_with_alpha", args, "%f", &alpha);
  cairo_paint_with_alpha(cc->ctx, alpha);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context new_path()
 */
static void f_new_path(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  cairo_new_path(cc->ctx);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context rectangle()
 */
static void f_rectangle(INT32 args)
{
  FLOAT_TYPE x, y, width, height;
  get_all_args("rectangle", args, "%F%F%F%F", &x, &y, &width, &height);
  pop_n_elems(args);
  struct cairo_mod_context *cc = THIS_CONTEXT;
  cairo_rectangle(cc->ctx, x, y, width, height);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context set_line_width(float width)
 */
static void f_set_line_width(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE width;
  get_all_args("set_line_width", args, "%F", &width);
  pop_n_elems(args);
  cairo_set_line_width(cc->ctx, width);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl void stroke()
 *!
 *! @note
 *! This function does not return its own object. This is because the
 *! path is cleared from the cairo context when stroke is called, and
 *! nesting of this command makes little sense.
 */
static void f_stroke(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  cairo_stroke(cc->ctx);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
}

/*! @decl Cairo.Path copy_path()
 */
static void f_copy_path(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  struct cairo_mod_path *ps;
  struct object *path_obj;

  path_obj = clone_object(cairo_mod_path_program, 0);
  ps = (struct cairo_mod_path*)get_storage(path_obj, cairo_mod_path_program);
  assert(ps);
  ps->path = cairo_copy_path(cc->ctx);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  push_object(path_obj);
}

/*! @decl Cairo.Path copy_path_flat()
 */
static void f_copy_path_flat(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  struct cairo_mod_path *ps;
  struct object *path_obj;

  path_obj = clone_object(cairo_mod_path_program, 0);
  ps = (struct cairo_mod_path*)get_storage(path_obj, cairo_mod_path_program);
  assert(ps);
  ps->path = cairo_copy_path_flat(cc->ctx);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  push_object(path_obj);
}

/*! @decl Cairo.Context append_path(Cairo.Path)
 */
static void f_append_path(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  struct cairo_mod_path *ps;
  struct object *path_obj;

  get_all_args("append_path", args, "%o", &path_obj);
  if (!(ps = (struct cairo_mod_path*)get_storage(path_obj, cairo_mod_path_program)))
    Pike_error("Object is not a Cairo.Path");

  cairo_append_path(cc->ctx, ps->path);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  push_object(path_obj);
}

/*! @decl Cairo.Context transform(Cairo.Matrix matrix)
 */
static void f_transform(INT32 args)
{
  struct object *o;
  struct cairo_mod_context *cc = THIS_CONTEXT;
  cairo_matrix_t *m;
  get_all_args("transform", args, "%o", &o);
  pop_n_elems(args);
  if (!(m = (cairo_matrix_t *)get_storage(o, cairo_mod_matrix_program)))
    Pike_error("Object is not Matrix");

  cairo_transform(cc->ctx, m);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context identity_matrix()
 *! Reset the transformation matrix on this context to the identity matrix.
 */
static void f_identity_matrix(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  cairo_identity_matrix(cc->ctx);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context select_font_face(string font_family, int slant, int weight)
 *! Select font.
 *!
 *! Selects a family and style of font from a simplified description as
 *! a family name, slant and weight. This function is meant to be used
 *! only for applications with simple font needs: Cairo doesn't provide
 *! for operations such as listing all available fonts on the system,
 *! and it is expected that most applications will need to use a more
 *! comprehensive font handling and text layout library in addition to
 *! cairo.
 */
static void f_select_font_face(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  int slant;
  int weight;
  struct pike_string *data;
  get_all_args("select_font_face", args, "%t%d%d", &data, &slant, &weight);
  pop_n_elems(args);
  // FIXME: convert data to utf-8?
  cairo_select_font_face(cc->ctx, (const char *)data->str, slant, weight);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context set_font_size(float|int size)
 *! Sets the current font matrix to a scale by a factor of @[size], replacing
 *! any font matrix previously set with cairo_set_font_size() or
 *! cairo_set_font_matrix(). This results in a font size of size user space
 *! units. (More precisely, this matrix will result in the font's
 *! em-square being a @[size] by @[size] square in user space.)
 */
static void f_set_font_size(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE size;
  get_all_args("set_font_size", args, "%F", &size);
  pop_n_elems(args);
  cairo_set_font_size(cc->ctx, size);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context set_font_matrix(Cairo.Matrix)
 */
static void f_set_font_matrix(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  struct object *o=NULL;
  cairo_matrix_t *m=NULL;
  get_all_args("set_font_matrix", args, "%o", &o);
  if (!(m = (cairo_matrix_t *)get_storage(o, cairo_mod_matrix_program)))
    Pike_error("Object is not Matrix");

  cairo_set_font_matrix (cc->ctx, m);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Matrix get_font_matrix()
 */
static void f_get_font_matrix(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  struct object *matrix_object = NULL;
  cairo_matrix_t* matrix;

  matrix_object = clone_object(cairo_mod_matrix_program, 0);
  matrix = (cairo_matrix_t*)get_storage(matrix_object,
                                        cairo_mod_matrix_program);
  assert(matrix);
  cairo_get_font_matrix(cc->ctx, matrix);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  push_object(matrix_object);
}

/*! @decl Cairo.Context set_font_options(Cairo.FontOptions options)
 */
static void f_set_font_options(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  struct object *o=NULL;
  struct cairo_mod_font_options* options;
  get_all_args("set_font_options", args, "%o", &o);
  if (!(options = (struct cairo_mod_font_options*)get_storage(o, cairo_mod_font_options_program)))
    Pike_error("Object is not FontOptions");
  cairo_set_font_options(cc->ctx, options->opt);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.FontOptions get_font_options()
 */
static void f_get_font_options(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  struct object *font_options_object = NULL;
  struct cairo_mod_font_options* options;

  font_options_object = clone_object(cairo_mod_font_options_program, 0);
  options = (struct cairo_mod_font_options*)get_storage(font_options_object,
                                                        cairo_mod_font_options_program);
  assert(options);
  cairo_get_font_options(cc->ctx, options->opt);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  push_object(font_options_object);
}

/*! @decl Cairo.Context show_text(string text)
 *! A drawing operator that generates the shape from a string of UTF-8
 *! characters, rendered according to the current font_face, font_size
 *! (font_matrix), and font_options.
 *!
 *! This function first computes a set of glyphs for the string of
 *! text. The first glyph is placed so that its origin is at the
 *! current point. The origin of each subsequent glyph is offset from
 *! that of the previous glyph by the advance values of the previous
 *! glyph.
 *!
 *! After this call the current point is moved to the origin of where
 *! the next glyph would be placed in this same progression. That is,
 *! the current point will be at the origin of the final glyph offset
 *! by its advance values. This allows for easy display of a single
 *! logical string with multiple calls to cairo_show_text().
 *!
 *! NOTE: The cairo_show_text() function call is part of what the
 *! cairo designers call the "toy" text API. It is convenient for
 *! short demos and simple programs, but it is not expected to be
 *! adequate for the most serious of text-using applications. See
 *! cairo_show_glyphs() for the "real" text display API in cairo.
 */
static void f_show_text(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  struct pike_string *data;

  if (args > 1)
      f_sprintf(args);

  f_string_to_utf8(1);
  get_all_args("show_text", 1, "%t", &data);
  cairo_show_text (cc->ctx, (char*)data->str);
  pop_n_elems(1);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Context set_operator(int operator)
 *!
 *! Sets the compositing operator to be used for all drawing
 *! operations. See #cairo_operator_t for details on the semantics of
 *! each available compositing operator.
 *!
 *! @param operator
 *! A compositing operator, specified as a Cairo.CAIRO_OPERATOR_*
 */
static void f_set_operator(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  INT_TYPE operator;
  get_all_args("set_operator", args, "%d", &operator);
  cairo_set_operator(cc->ctx, operator);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl int get_operator()
 *!
 *! Gets the current compositing operator for a cairo context.
 *!
 *! @returns
 *! A compositing operator, specified as a Cairo.CAIRO_OPERATOR_*
 */
static void f_get_operator(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  INT_TYPE operator;
  operator = cairo_get_operator(cc->ctx);
  push_int(operator);
}

/*! @decl Cairo.Context set_tolerance(float tolerance)
 */
static void f_set_tolerance(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE tolerance;
  get_all_args("set_tolerance", args, "%f", &tolerance);
  cairo_set_tolerance(cc->ctx, tolerance);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl int get_miter_limit()
 *!
 *! Gets the miter limit for a cairo context.
 *!
 */
static void f_get_miter_limit(INT32 args)
{
  push_float(cairo_get_miter_limit(THIS_CONTEXT->ctx));
}

/*! @decl Cairo.Context set_miter_limit(float miter_limit)
 */
static void f_set_miter_limit(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE miter_limit;
  get_all_args("set_tolerance", args, "%f", &miter_limit);
  cairo_set_tolerance(cc->ctx, miter_limit);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl float get_tolerance()
 *!
 *! Gets the current tolerance for a cairo context.
 *!
 *! @returns
 *! The tolerance number
 */
static void f_get_tolerance(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE tolerance;
  tolerance = cairo_get_tolerance(cc->ctx);
  push_float(tolerance);
}

/*! @decl Cairo.Context set_anti_alias(int setting)
 */
static void f_set_anti_alias(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  INT_TYPE setting;
  get_all_args("set_anti_alias", args, "%d", &setting);
  cairo_set_antialias(cc->ctx, setting);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl int get_anti_alias()
 *!
 *! Gets the anti-alias setting of the cairo context.
 *!
 *! @returns
 *! The anti-alias setting
 */
static void f_get_anti_alias(INT32 args)
{
  push_int(cairo_get_antialias(THIS_CONTEXT->ctx));
}

/*! @decl Cairo.Context set_line_cap(int setting)
 */
static void f_set_line_cap(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  INT_TYPE setting;
  get_all_args("set_line_cap", args, "%d", &setting);
  cairo_set_line_cap(cc->ctx, setting);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl int get_line_cap()
 *!
 *! Gets the line-cap setting of the cairo context.
 *!
 *! @returns
 *! The line-cap setting
 */
static void f_get_line_cap(INT32 args)
{
  push_int(cairo_get_line_cap(THIS_CONTEXT->ctx));
}

/*! @decl Cairo.Context set_line_join(int setting)
 */
static void f_set_line_join(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  INT_TYPE setting;
  get_all_args("set_line_join", args, "%d", &setting);
  cairo_set_line_join(cc->ctx, setting);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl int get_line_join()
 *!
 *! Gets the line-join setting of the cairo context.
 *!
 *! @returns
 *! The line-join setting
 */
static void f_get_line_join(INT32 args)
{
  push_int(cairo_get_line_join(THIS_CONTEXT->ctx));
}

/*! @decl Cairo.Context set_fill_rule(int rule)
 */
static void f_set_fill_rule(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  INT_TYPE rule;
  get_all_args("set_fill_rule", args, "%d", &rule);
  cairo_set_fill_rule(cc->ctx, rule);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl int get_fill_rule()
 *!
 *! Gets the fill-rule setting of the cairo context.
 *!
 *! @returns
 *! The fill-rule setting
 */
static void f_get_fill_rule(INT32 args)
{
  push_int(cairo_get_fill_rule(THIS_CONTEXT->ctx));
}

/*! @decl Cairo.Context set_dash(array(float) dashes, float offset)
 */
static void f_set_dash(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  struct array *dashes_array;
  double *dashes;
  FLOAT_TYPE offset;
  INT32 i, size;

  get_all_args("set_dash", args, "%a%f", &dashes_array, &offset);
  size = dashes_array->size;
  if (size > 0)
    {
      dashes = malloc(dashes_array->size*sizeof(double)); // OOM
      for (i=0; i<dashes_array->size; ++i)
        {
          if (dashes_array->item[i].type == PIKE_T_FLOAT)
            dashes[i] = dashes_array->item[i].u.float_number;
          else
            dashes[i] = 0;
        }

      cairo_set_dash (cc->ctx, dashes, i, offset);
      free(dashes);
      CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
    }

  ref_push_object(THIS_OBJ);
}

/*! @decl array(float) user_to_device(float x, float y)
 */
static void f_user_to_device(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;

  /* Cairo takes pointers to doubles, so if FLOAT_TYPE is 'float' we
     need to make doubles that we can send to cairo. If FLOAT_TYPE is
     'double' this it just compotron-burning.
   */

  FLOAT_TYPE x, y;
  double cx, cy;
  get_all_args("user_to_device", args, "%f%f", &x, &y);
  cx = x; cy = y;
  cairo_user_to_device(cc->ctx, &cx, &cy);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  push_float(cx);
  push_float(cy);
  f_aggregate(2);
}

/*! @decl array(float) user_to_device_distance(float x, float y)
 */
static void f_user_to_device_distance(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE x, y;
  double cx, cy;
  get_all_args("user_to_device_distance", args, "%f%f", &x, &y);
  cx = x; cy = y;
  cairo_user_to_device_distance(cc->ctx, &cx, &cy);
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  push_float(cx);
  push_float(cy);
  f_aggregate(2);
}

/*! @decl Cairo.Context mask(Cairo.Pattern|Cairo.Surface object, float|void x, float|void y)
 *!
 *! A drawing operator that paints the current source using the alpha
 *! channel of @[object] as a mask. (Opaque areas of @[object] are
 *! painted with the source, transparent areas are not painted.)
 */
static void f_mask(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  struct object *o;
  struct cairo_mod_pattern* cp;
  struct cairo_mod_surface* cs;
  FLOAT_TYPE x, y;

  if (args == 1)
    {
      get_all_args("mask", args, "%o", &o);
      if (!(cp = (struct cairo_mod_pattern *)get_storage(o, cairo_mod_pattern_program)))
        Pike_error("Expected Object as pattern as sole argument");
      cairo_mask(cc->ctx, cp->pattern);
    }
  else if (args == 3)
    {
      get_all_args("mask", args, "%o", &o, &x, &y);
      if (!(cs = (struct cairo_mod_surface *)get_storage(o, cairo_mod_surface_program)))
        Pike_error("Object wasn't a surface");
      cairo_mask_surface(cc->ctx, cs->surface, x, y);
    }
  else
    SIMPLE_WRONG_NUM_ARGS_ERROR("mask", 3);
    
  CHECK_CAIRO_CONTEXT_ERROR(cc->ctx);
  ref_push_object(THIS_OBJ);
}

/*! @decl int(0..1) in_stroke(float|int x, float|int y)
 */
static void f_in_stroke(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE x, y;
  get_all_args("in_stroke", args, "%F%F", &x, &y);
  pop_n_elems(args);
  push_int(cairo_in_stroke(cc->ctx, x, y));
}

/*! @decl int(0..1) in_fill(float|int x, float|int y)
 */
static void f_in_fill(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  FLOAT_TYPE x, y;
  get_all_args("in_fill", args, "%F%F", &x, &y);
  pop_n_elems(args);
  push_int(cairo_in_fill(cc->ctx, x, y));
}

/*! @decl mapping(string:float) stroke_extents()
 *! 
 *! Return the extents of the current stroke
 *!
 *! @returns
 *!   @mapping
 *!     @member float x1
 *!     @member float y1
 *!     @member float x2
 *!     @member float y2
 *!   @endmapping
 */
static void f_stroke_extents(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  double x1, y1, x2, y2;
  cairo_stroke_extents (cc->ctx,
                        &x1, &y1,
                        &x2, &y2);
  push_text("x1"); push_float(x1);
  push_text("y1"); push_float(y1);
  push_text("x2"); push_float(x2);
  push_text("y2"); push_float(y2);
  f_aggregate_mapping(8);
}

/*! @decl mapping(string:float) fill_extents()
 *!
 *! Return the extents of the current fill
 *!
 *! @returns
 *!   @mapping
 *!     @member float x1
 *!     @member float y1
 *!     @member float x2
 *!     @member float y2
 *!   @endmapping
 */
static void f_fill_extents(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  double x1, y1, x2, y2;
  cairo_fill_extents (cc->ctx,
                      &x1, &y1,
                      &x2, &y2);
  push_text("x1"); push_float(x1);
  push_text("y1"); push_float(y1);
  push_text("x2"); push_float(x2);
  push_text("y2"); push_float(y2);
  f_aggregate_mapping(8);
}

/*! @decl mapping text_extents(string s)
 *!
 *! @returns
 *!   @mapping
 *!     @member float x_bearing
 *!     @member float y_bearing
 *!     @member float width
 *!     @member float height
 *!     @member float x_advance
 *!     @member float y_advance
 *!   @endmapping
 */
static void f_text_extents(INT32 args)
{
  struct cairo_mod_context *cc = THIS_CONTEXT;
  cairo_text_extents_t extents;
  struct pike_string *data;

  if (args == 1)
    {
      f_string_to_utf8(1);
      get_all_args("text_extents", 1, "%t", &data);
      cairo_text_extents(cc->ctx, (const char *)data->str, &extents);
      pop_n_elems(1);

      push_text("x_bearing"); push_float(extents.x_bearing);
      push_text("y_bearing"); push_float(extents.y_bearing);
      push_text("width"); push_float(extents.width);
      push_text("height"); push_float(extents.height);
      push_text("x_advance"); push_float(extents.x_advance);
      push_text("y_advance"); push_float(extents.y_advance);
      f_aggregate_mapping(12);
    }
  else
    SIMPLE_WRONG_NUM_ARGS_ERROR("text_extents", 1);
}

/*! @endclass */

#if defined(HAVE_GTK2)

/*! @class GTK2Context
 *!
 */

static struct program *gobject_program = NULL;

static void assert_gobject_program()
{
  if(!gobject_program)
  {
    push_text( "GTK2.GObject" );
    APPLY_MASTER( "resolv", 1 );
    gobject_program = program_from_svalue( Pike_sp - 1  );
    Pike_sp--;/* Do not free gobject program.. */

    if( !gobject_program )
      Pike_error("No GTK2 module?\n");
  }
}

/*! @decl void create()
 */
static void f_gtk2_create(INT32 args)
{
  struct object_wrapper *ow;
  struct object *o=NULL;
  struct cairo_mod_context *cc = THIS_CONTEXT;
  get_all_args( "create", args, "%o", &o );

  assert_gobject_program();

  /* Type-check here is point-less until I can get hold of a program
     that has object_wrapper as storage */
  if (!(ow = (struct object_wrapper*)get_storage(o, gobject_program)))
    Pike_error("Object is not derived from GTK2.GObject");

  if (!G_TYPE_CHECK_INSTANCE_TYPE ((ow->obj), GTK_TYPE_WIDGET))
    Pike_error("Object is not a GTK2.Widget");

  cc->ctx = gdk_cairo_create(GTK_WIDGET(ow->obj)->window);
  pop_n_elems(args);
}

/*! @endclass */

#endif // defined(HAVE_GTK2)

#endif // HAVE_CAIRO

#define tNumber tOr(tFloat,tInt)

PIKE_MODULE_INIT
{
  ptrdiff_t off;
  struct svalue prog;
  prog.type = PIKE_T_PROGRAM;
  prog.subtype = 0;

#ifdef HAVE_CAIRO

  /*! @decl constant CAIRO_HAS_PNG_FUNCTIONS
   *! @decl constant CAIRO_HAS_FT_FONT
   *!
   *! Feature-flag defines that can be used to check what Cairo features
   *! that are available. These are set to 0 (zero) or 1 (one).
   */
  ADD_INT_CONSTANT("CAIRO_HAS_PNG_FUNCTIONS",
#if CAIRO_HAS_PNG_FUNCTIONS
                   1,
#else
                   0,
#endif // CAIRO_HAS_PNG_FUNCTIONS
                   0);
  ADD_INT_CONSTANT("CAIRO_HAS_FT_FONT",
#if CAIRO_HAS_FT_FONT
                   1,
#else
                   0,
#endif // CAIRO_HAS_FT_FONT
                   0);

  /*! @decl constant CAIRO_ANTIALIAS_DEFAULT
   *! @decl constant CAIRO_ANTIALIAS_NONE
   *! @decl constant CAIRO_ANTIALIAS_GRAY
   *! @decl constant CAIRO_ANTIALIAS_SUBPIXEL
   *!
   *! Various anti-alias settings that can be used when drawing shapes
   */
  ADD_INT_CONSTANT("CAIRO_ANTIALIAS_DEFAULT", (INT_TYPE)CAIRO_ANTIALIAS_DEFAULT, 0);
  ADD_INT_CONSTANT("CAIRO_ANTIALIAS_NONE", (INT_TYPE)CAIRO_ANTIALIAS_NONE, 0);
  ADD_INT_CONSTANT("CAIRO_ANTIALIAS_GRAY", (INT_TYPE)CAIRO_ANTIALIAS_GRAY, 0);
  ADD_INT_CONSTANT("CAIRO_ANTIALIAS_SUBPIXEL", (INT_TYPE)CAIRO_ANTIALIAS_SUBPIXEL, 0);

  /*! @decl constant CAIRO_OPERATOR_CLEAR
   *! @decl constant CAIRO_OPERATOR_SOURCE
   *! @decl constant CAIRO_OPERATOR_OVER
   *! @decl constant CAIRO_OPERATOR_IN
   *! @decl constant CAIRO_OPERATOR_OUT
   *! @decl constant CAIRO_OPERATOR_ATOP
   *! @decl constant CAIRO_OPERATOR_DEST
   *! @decl constant CAIRO_OPERATOR_DEST_OVER
   *! @decl constant CAIRO_OPERATOR_DEST_IN
   *! @decl constant CAIRO_OPERATOR_DEST_OUT
   *! @decl constant CAIRO_OPERATOR_DEST_ATOP
   *! @decl constant CAIRO_OPERATOR_XOR
   *! @decl constant CAIRO_OPERATOR_ADD
   *! @decl constant CAIRO_OPERATOR_SATURATE
   *!
   *! Various operators that can be used when drawing shapes
   */
  ADD_INT_CONSTANT("CAIRO_OPERATOR_CLEAR", (INT_TYPE)CAIRO_OPERATOR_CLEAR, 0);
  ADD_INT_CONSTANT("CAIRO_OPERATOR_SOURCE", (INT_TYPE)CAIRO_OPERATOR_SOURCE, 0);
  ADD_INT_CONSTANT("CAIRO_OPERATOR_OVER", (INT_TYPE)CAIRO_OPERATOR_OVER, 0);
  ADD_INT_CONSTANT("CAIRO_OPERATOR_IN", (INT_TYPE)CAIRO_OPERATOR_IN, 0);
  ADD_INT_CONSTANT("CAIRO_OPERATOR_OUT", (INT_TYPE)CAIRO_OPERATOR_OUT, 0);
  ADD_INT_CONSTANT("CAIRO_OPERATOR_ATOP", (INT_TYPE)CAIRO_OPERATOR_ATOP, 0);
  ADD_INT_CONSTANT("CAIRO_OPERATOR_DEST", (INT_TYPE)CAIRO_OPERATOR_DEST, 0);
  ADD_INT_CONSTANT("CAIRO_OPERATOR_DEST_OVER", (INT_TYPE)CAIRO_OPERATOR_DEST_OVER, 0);
  ADD_INT_CONSTANT("CAIRO_OPERATOR_DEST_IN", (INT_TYPE)CAIRO_OPERATOR_DEST_IN, 0);
  ADD_INT_CONSTANT("CAIRO_OPERATOR_DEST_OUT", (INT_TYPE)CAIRO_OPERATOR_DEST_OUT, 0);
  ADD_INT_CONSTANT("CAIRO_OPERATOR_DEST_ATOP", (INT_TYPE)CAIRO_OPERATOR_DEST_ATOP, 0);
  ADD_INT_CONSTANT("CAIRO_OPERATOR_XOR", (INT_TYPE)CAIRO_OPERATOR_XOR, 0);
  ADD_INT_CONSTANT("CAIRO_OPERATOR_ADD", (INT_TYPE)CAIRO_OPERATOR_ADD, 0);
  ADD_INT_CONSTANT("CAIRO_OPERATOR_SATURATE", (INT_TYPE)CAIRO_OPERATOR_SATURATE, 0);

  /*! @decl constant CAIRO_LINE_CAP_BUTT
   *! @decl constant CAIRO_LINE_CAP_ROUND
   *! @decl constant CAIRO_LINE_CAP_SQUARE
   */
  ADD_INT_CONSTANT("CAIRO_LINE_CAP_BUTT", (INT_TYPE)CAIRO_LINE_CAP_BUTT, 0);
  ADD_INT_CONSTANT("CAIRO_LINE_CAP_ROUND", (INT_TYPE)CAIRO_LINE_CAP_ROUND, 0);
  ADD_INT_CONSTANT("CAIRO_LINE_CAP_SQUARE", (INT_TYPE)CAIRO_LINE_CAP_SQUARE, 0);

  /*! @decl constant CAIRO_LINE_JOIN_MITER
   *! @decl constant CAIRO_LINE_JOIN_ROUND
   *! @decl constant CAIRO_LINE_JOIN_BEVEL
   */
  ADD_INT_CONSTANT("CAIRO_LINE_JOIN_MITER", (INT_TYPE)CAIRO_LINE_JOIN_MITER, 0);
  ADD_INT_CONSTANT("CAIRO_LINE_JOIN_ROUND", (INT_TYPE)CAIRO_LINE_JOIN_ROUND, 0);
  ADD_INT_CONSTANT("CAIRO_LINE_JOIN_BEVEL", (INT_TYPE)CAIRO_LINE_JOIN_BEVEL, 0);

  /*! @decl constant CAIRO_FILL_RULE_WINDING
   *! @decl constant CAIRO_FILL_RULE_EVEN_ODD
   */
  ADD_INT_CONSTANT("CAIRO_FILL_RULE_WINDING", (INT_TYPE)CAIRO_FILL_RULE_WINDING, 0);
  ADD_INT_CONSTANT("CAIRO_FILL_RULE_EVEN_ODD", (INT_TYPE)CAIRO_FILL_RULE_EVEN_ODD, 0);

  /* Cairo.Context */
  start_new_program();
  {
    ADD_STORAGE(struct cairo_mod_context);
    ADD_FUNCTION("create", f_context_create, tFunc(tObj,tVoid), ID_STATIC);
    ADD_FUNCTION("move_to", f_move_to, tFunc(tNumber tNumber,tObj), ID_PUBLIC);
    ADD_FUNCTION("rel_move_to", f_rel_move_to, tFunc(tNumber tNumber,tObj), ID_PUBLIC);
    ADD_FUNCTION("arc", f_arc, tFunc(tNumber tNumber tNumber tNumber tNumber,tObj), ID_PUBLIC);
    ADD_FUNCTION("arc_negative", f_arc_negative, tFunc(tNumber tNumber tNumber tNumber tNumber,tObj), ID_PUBLIC);
    ADD_FUNCTION("line_to", f_line_to, tFunc(tNumber tNumber,tObj), ID_PUBLIC);
    ADD_FUNCTION("rel_line_to", f_rel_line_to, tFunc(tNumber tNumber,tObj), ID_PUBLIC);
    ADD_FUNCTION("curve_to", f_curve_to, tFunc(tNumber tNumber tNumber tNumber tNumber tNumber,tObj), ID_PUBLIC);
    ADD_FUNCTION("rel_curve_to", f_rel_curve_to, tFunc(tNumber tNumber tNumber tNumber tNumber tNumber,tObj), ID_PUBLIC);
    ADD_FUNCTION("fill_preserve", f_fill_preserve, tFunc(tNone,tObj), ID_PUBLIC);
    ADD_FUNCTION("fill", f_fill, tFunc(tNone,tObj), ID_PUBLIC);
    ADD_FUNCTION("clip", f_clip, tFunc(tNone,tVoid), ID_PUBLIC);
    ADD_FUNCTION("clip_preserve", f_reset_clip, tFunc(tNone,tObj), ID_PUBLIC);
    ADD_FUNCTION("reset_clip", f_reset_clip, tFunc(tNone,tVoid), ID_PUBLIC);
    ADD_FUNCTION("save", f_save, tFunc(tNone,tObj), ID_PUBLIC);
    ADD_FUNCTION("restore", f_restore, tFunc(tNone,tVoid), ID_PUBLIC);
    ADD_FUNCTION("new_path", f_new_path, tFunc(tNone,tObj), ID_PUBLIC);
    ADD_FUNCTION("rectangle", f_rectangle, tFunc(tNumber tNumber tNumber tNumber, tObj), ID_PUBLIC);
    ADD_FUNCTION("stroke", f_stroke, tFunc(tNone,tVoid), ID_PUBLIC);
    ADD_FUNCTION("close_path", f_close_path, tFunc(tNone,tObj), ID_PUBLIC);
    ADD_FUNCTION("copy_path", f_copy_path, tFunc(tNone,tObj), ID_PUBLIC);
    ADD_FUNCTION("copy_path_flat", f_copy_path_flat, tFunc(tNone,tObj), ID_PUBLIC);
    ADD_FUNCTION("append_path", f_append_path, tFunc(tObj,tObj), ID_PUBLIC);
    ADD_FUNCTION("transform", f_transform, tFunc(tObj, tObj), ID_PUBLIC);
    ADD_FUNCTION("identity_matrix", f_identity_matrix, tFunc(tNone,tObj), ID_PUBLIC);
    ADD_FUNCTION("set_line_width", f_set_line_width, tFunc(tNumber, tObj), ID_PUBLIC);
    ADD_FUNCTION("set_source", f_set_source, tFunc(tOr(tObj,tNumber) tOr(tVoid,tNumber) tOr(tVoid,tNumber) tOr(tVoid,tNumber), tObj), ID_PUBLIC);
    ADD_FUNCTION("translate", f_translate, tFunc(tNumber tNumber, tObj), ID_PUBLIC);
    ADD_FUNCTION("scale", f_scale, tFunc(tNumber tNumber, tObj), ID_PUBLIC);
    ADD_FUNCTION("rotate", f_rotate, tFunc(tNumber, tObj), ID_PUBLIC);
    ADD_FUNCTION("paint", f_paint, tFunc(tNone, tObj), ID_PUBLIC);
    ADD_FUNCTION("paint_with_alpha", f_paint_with_alpha, tFunc(tFloat, tObj), ID_PUBLIC);
    ADD_FUNCTION("set_tolerance", f_set_tolerance, tFunc(tFloat, tObj), ID_PUBLIC);
    ADD_FUNCTION("get_tolerance", f_get_tolerance, tFunc(tNone, tFloat), ID_PUBLIC);
    ADD_FUNCTION("set_anti_alias", f_set_anti_alias, tFunc(tInt, tObj), ID_PUBLIC);
    ADD_FUNCTION("get_anti_alias", f_get_anti_alias, tFunc(tNone, tInt), ID_PUBLIC);
    ADD_FUNCTION("set_line_cap", f_set_line_cap, tFunc(tInt, tObj), ID_PUBLIC);
    ADD_FUNCTION("get_line_cap", f_get_line_cap, tFunc(tNone, tInt), ID_PUBLIC);
    ADD_FUNCTION("set_line_join", f_set_line_join, tFunc(tInt, tObj), ID_PUBLIC);
    ADD_FUNCTION("get_line_join", f_get_line_join, tFunc(tNone, tInt), ID_PUBLIC);
    ADD_FUNCTION("set_fill_rule", f_set_fill_rule, tFunc(tInt, tObj), ID_PUBLIC);
    ADD_FUNCTION("get_fill_rule", f_get_fill_rule, tFunc(tNone, tInt), ID_PUBLIC);
    ADD_FUNCTION("set_operator", f_set_operator, tFunc(tInt, tObj), ID_PUBLIC);
    ADD_FUNCTION("get_operator", f_get_operator, tFunc(tVoid, tInt), ID_PUBLIC);
    ADD_FUNCTION("set_miter_limit", f_set_miter_limit, tFunc(tFloat, tObj), ID_PUBLIC);
    ADD_FUNCTION("get_miter_limit", f_get_miter_limit, tFunc(tVoid, tFloat), ID_PUBLIC);
    ADD_FUNCTION("set_dash", f_set_dash, tFunc(tArray tFloat, tObj), ID_PUBLIC);
    ADD_FUNCTION("user_to_device", f_user_to_device, tFunc(tFloat tFloat,tArray), ID_PUBLIC);
    ADD_FUNCTION("user_to_device_distance", f_user_to_device_distance, tFunc(tFloat tFloat,tArray), ID_PUBLIC);
    ADD_FUNCTION("mask", f_mask, tFunc(tObj tOr(tFloat,tVoid) tOr(tFloat,tVoid),tObj), ID_PUBLIC);
    ADD_FUNCTION("in_stroke", f_in_stroke, tFunc(tNumber tNumber, tInt01), ID_PUBLIC);
    ADD_FUNCTION("in_fill", f_in_fill, tFunc(tNumber tNumber, tInt01), ID_PUBLIC);
    ADD_FUNCTION("stroke_extents", f_stroke_extents, tFunc(tNone, tMapping), ID_PUBLIC);
    ADD_FUNCTION("fill_extents", f_fill_extents, tFunc(tNone, tMapping), ID_PUBLIC);
    /* font stuff */
    ADD_FUNCTION("select_font_face", f_select_font_face, tFunc(tString tInt tInt,tObj), ID_PUBLIC);
    ADD_FUNCTION("set_font_size", f_set_font_size, tFunc(tNumber, tObj), ID_PUBLIC);
    ADD_FUNCTION("set_font_matrix", f_set_font_matrix, tFunc(tObj, tObj), ID_PUBLIC);
    ADD_FUNCTION("get_font_matrix", f_get_font_matrix, tFunc(tVoid, tObj), ID_PUBLIC);
    ADD_FUNCTION("set_font_options", f_set_font_options, tFunc(tObj, tObj), ID_PUBLIC);
    ADD_FUNCTION("get_font_options", f_get_font_options, tFunc(tVoid, tObj), ID_PUBLIC);
    ADD_FUNCTION("show_text", f_show_text, tFuncV(tString, tMixed, tObj), ID_PUBLIC);
    ADD_FUNCTION("text_extents", f_text_extents, tFunc(tString,tMapping), ID_PUBLIC);

    set_init_callback(init_cairo_mod_context);
    set_exit_callback(exit_cairo_mod_context);
  }
  add_program_constant("Context",
                       cairo_mod_context_program = end_program(),
                       0);

#ifdef HAVE_GTK2
  /* Cairo.GTK2Context */
  start_new_program();
  {
    prog.u.program = cairo_mod_context_program;
    do_inherit(&prog, 0, NULL);
    ADD_FUNCTION("create", f_gtk2_create, tFunc(tObj,tVoid), ID_STATIC);
  }
  add_program_constant("GTK2Context",
                       gtk2_cairo_mod_context_program = end_program(),
                       0);
#endif // HAVE_GTK2

  /* Init sub-modules */
  cairo_mod_init_surface();
  cairo_mod_init_path();
  cairo_mod_init_matrix();
  cairo_mod_init_font();
  cairo_mod_init_pattern();
  cairo_mod_init_error();
#else // !HAVE_CAIRO
  HIDE_MODULE();
#endif // !HAVE_CAIRO
}

PIKE_MODULE_EXIT
{
}

/*! @endmodule */
