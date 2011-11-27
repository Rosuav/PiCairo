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

#ifdef HAVE_CAIRO

/*! @module Cairo
 */

/*! @class Pattern
 */

#define THIS_PATTERN ((struct cairo_mod_pattern *)Pike_fp->current_storage)

struct program *cairo_mod_pattern_program = NULL;
struct program *cairo_mod_gradient_program = NULL;
struct program *cairo_mod_linear_gradient_program = NULL;
struct program *cairo_mod_radial_gradient_program = NULL;
struct program *cairo_mod_surface_pattern_program = NULL;

static void init_cairo_mod_pattern( struct object *o )
{
  struct cairo_mod_pattern *cp = THIS_PATTERN;
  cp->pattern = NULL;
}

static void exit_cairo_mod_pattern( struct object *o )
{
  struct cairo_mod_pattern *cp = THIS_PATTERN;
  if (cp->pattern)
    cairo_pattern_destroy (cp->pattern);
}

/*! @decl void set_matrix(Cairo.Matrix matrix)
 */
static void f_set_matrix(INT32 args)
{
  struct cairo_mod_pattern *cp = THIS_PATTERN;
  struct object* o;
  cairo_matrix_t *matrix;

  get_all_args("set_matrix", args, "%o", &o);
  pop_n_elems(args);

  if (!(matrix = (cairo_matrix_t *)get_storage(o, cairo_mod_matrix_program)))
    Pike_error("Object is not Matrix");

  cairo_pattern_set_matrix(cp->pattern, matrix);
}

/*! @decl Cairo.Matrix get_matrix()
 */
static void f_get_matrix(INT32 args)
{
  struct cairo_mod_pattern *cp = THIS_PATTERN;
  struct object* o;
  cairo_matrix_t *matrix;

  o = clone_object(cairo_mod_matrix_program, 0);
  matrix = (cairo_matrix_t *)get_storage(o, cairo_mod_matrix_program);
  assert(matrix);
  cairo_pattern_get_matrix(cp->pattern, matrix);
  push_object(o);
}

/*! @endclass */

/*! @class Gradient
 */

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

/*! @decl Cairo.Pattern add_color_stop(float offset, Image.Color color)
 *! @decl Cairo.Pattern add_color_stop(float offset, Image.Color color, float alpha)
 */
static void f_add_color_stop(INT32 args)
{
  struct cairo_mod_pattern *cp = THIS_PATTERN;
  struct object* o;
  struct color_struct *col;
  FLOAT_TYPE off, a;
  
  if (args == 2)
    {
      get_all_args("add_color_stop", args, "%f%o", &off, &o);
      a = 1.0;
    }
  else if (args == 3)
    {
      get_all_args("add_color_stop", args, "%f%o%f", &off, &o, &a);
    }
  else
    SIMPLE_WRONG_NUM_ARGS_ERROR("add_color_stop", 2);

  if (o->prog->id == PROG_IMAGE_COLOR_COLOR_ID)
    {
      assert_image_color_program();
      col = (struct color_struct*)get_storage(o, image_color_program);
      cairo_pattern_add_color_stop_rgba(cp->pattern,
                                        off,
                                        col->rgbl.r / (FLOAT_TYPE)COLORLMAX,
                                        col->rgbl.g / (FLOAT_TYPE)COLORLMAX,
                                        col->rgbl.b / (FLOAT_TYPE)COLORLMAX,
                                        a);
    }
  else
    SIMPLE_ARG_TYPE_ERROR("add_color_stop", 1, "Image.Color");
  pop_n_elems(args);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Pattern add_color_stop_rgb(float offset, float r, float g, float b)
 */
static void f_add_color_stop_rgb(INT32 args)
{
  struct cairo_mod_pattern *cp = THIS_PATTERN;
  FLOAT_TYPE off, r, g, b;
  get_all_args("add_color_stop_rgb", args, "%f%f%f%f", &off, &r, &g, &b);
  pop_n_elems(args);
  cairo_pattern_add_color_stop_rgb(cp->pattern, off, r, g, b);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Pattern add_color_stop_rgba(float offset, float r, float g, float b, float a)
 */
static void f_add_color_stop_rgba(INT32 args)
{
  struct cairo_mod_pattern *cp = THIS_PATTERN;
  FLOAT_TYPE off, r, g, b, a;
  get_all_args("add_color_stop_rgba", args, "%f%f%f%f%f", &off, &r, &g, &b, &a);
  pop_n_elems(args);
  cairo_pattern_add_color_stop_rgb(cp->pattern, off, r, g, b);
  ref_push_object(THIS_OBJ);
}

/*! @endclass */

/*! @class LinearGradient
 */

static void f_create_linear_gradient(INT32 args)
{
  struct cairo_mod_pattern *cp = THIS_PATTERN;
  FLOAT_TYPE x0, y0, x1, y1;
  get_all_args("create", args, "%f%f%f%f", &x0, &y0, &x1, &y1);
  pop_n_elems(args);
  cp->pattern = cairo_pattern_create_linear(x0, y0, x1, y1);
}

/*! @endclass */

/*! @class RadialGradient
 */

static void f_create_radial_gradient(INT32 args)
{
  struct cairo_mod_pattern *cp = THIS_PATTERN;
  FLOAT_TYPE cx0, cy0, radius0, cx1, cy1, radius1;
  get_all_args("create", args, "%f%f%f%f%f%f", &cx0, &cy0, &radius0, &cx1, &cy1, &radius1);
  pop_n_elems(args);
  cp->pattern = cairo_pattern_create_radial(cx0, cy0, radius0, cx1, cy1, radius1);
}

/*! @endclass */

/*! @class SurfacePattern
 */

static void f_create_surface_pattern(INT32 args)
{
  struct cairo_mod_pattern *cp = THIS_PATTERN;
  struct object *o;
  struct cairo_mod_surface * cs;

  get_all_args("create", args, "%o", &o);
  pop_n_elems(args);

  cs = (struct cairo_mod_surface *)get_storage(o, cairo_mod_surface_program);
  cp->pattern = cairo_pattern_create_for_surface(cs->surface);
}

/*! @decl void set_extend(int extend)
 */
static void f_set_extend(INT32 args)
{
  struct cairo_mod_pattern *cp = THIS_PATTERN;
  INT_TYPE extend_type;
  get_all_args("set_extend", args, "%d", &extend_type);
  pop_n_elems(args);
  cairo_pattern_set_extend(cp->pattern, extend_type);
}

/*! @decl int get_extend()
 */
static void f_get_extend(INT32 args)
{
  struct cairo_mod_pattern *cp = THIS_PATTERN;
  push_int(cairo_pattern_get_extend(cp->pattern));
}

/*! @endclass */

void cairo_mod_init_pattern()
{
  struct svalue prog;
  prog.type = PIKE_T_PROGRAM;
  prog.subtype = 0;

  ADD_INT_CONSTANT("CAIRO_EXTEND_NONE", (INT_TYPE)CAIRO_EXTEND_NONE, 0);
  ADD_INT_CONSTANT("CAIRO_EXTEND_REPEAT", (INT_TYPE)CAIRO_EXTEND_REPEAT, 0);
  ADD_INT_CONSTANT("CAIRO_EXTEND_REFLECT", (INT_TYPE)CAIRO_EXTEND_REFLECT, 0);

#if CAIRO_VERSION > CAIRO_VERSION_ENCODE(1,0,2)
  ADD_INT_CONSTANT("CAIRO_EXTEND_PAD", (INT_TYPE)CAIRO_EXTEND_PAD, 0);
#endif

  start_new_program();
  {
    ADD_STORAGE(struct cairo_mod_pattern);
    ADD_FUNCTION("set_matrix", f_set_matrix, tFunc(tObj,tVoid), 0);
    ADD_FUNCTION("get_matrix", f_get_matrix, tFunc(tNone,tObj), 0);
    set_init_callback(init_cairo_mod_pattern);
    set_exit_callback(exit_cairo_mod_pattern);
  }
  add_program_constant("Pattern",
                       cairo_mod_pattern_program = end_program(),
                       0);

  start_new_program();
  {
    prog.u.program = cairo_mod_pattern_program;
    do_inherit(&prog, 0, NULL);
    ADD_FUNCTION("add_color_stop", f_add_color_stop, tFunc(tFloat tObj tOr(tFloat,tVoid), tObj), 0);
    ADD_FUNCTION("add_color_stop_rgb", f_add_color_stop_rgb, tFunc(tFloat tFloat tFloat tFloat, tObj), 0);
    ADD_FUNCTION("add_color_stop_rgba", f_add_color_stop_rgba, tFunc(tFloat tFloat tFloat tFloat tFloat, tObj), 0);
  }
  add_program_constant("Gradient",
                       cairo_mod_gradient_program = end_program(),
                       0);

  start_new_program();
  {
    prog.u.program = cairo_mod_gradient_program;
    do_inherit(&prog, 0, NULL);
    ADD_FUNCTION("create", f_create_linear_gradient, tFunc(tFloat tFloat tFloat tFloat, tVoid), 0);
  }
  add_program_constant("LinearGradient",
                       cairo_mod_linear_gradient_program = end_program(),
                       0);

  start_new_program();
  {
    prog.u.program = cairo_mod_gradient_program;
    do_inherit(&prog, 0, NULL);
    ADD_FUNCTION("create", f_create_radial_gradient, tFunc(tFloat tFloat tFloat tFloat tFloat tFloat, tVoid), 0);
  }
  add_program_constant("RadialGradient",
                       cairo_mod_radial_gradient_program = end_program(),
                       0);

  start_new_program();
  {
    prog.u.program = cairo_mod_pattern_program;
    do_inherit(&prog, 0, NULL);

    ADD_FUNCTION("set_extend", f_set_extend, tFunc(tInt, tVoid), ID_PUBLIC);
    ADD_FUNCTION("get_extend", f_get_extend, tFunc(tNone, tInt), ID_PUBLIC);
    //cairo_pattern_set_filter
    //cairo_pattern_get_filter

    ADD_FUNCTION("create", f_create_surface_pattern, tFunc(tObj, tVoid), 0);
  }
  add_program_constant("SurfacePattern",
                       cairo_mod_surface_pattern_program = end_program(),
                       0);
}

/*! @endmodule */

#endif // HAVE_CAIRO
