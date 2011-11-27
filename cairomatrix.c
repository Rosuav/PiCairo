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

#ifdef HAVE_CAIRO

#define THIS_MATRIX ((cairo_matrix_t *)Pike_fp->current_storage)

struct program *cairo_mod_matrix_program = NULL;

/*! @module Cairo
 */

/*! @class Matrix
 */

/*! @decl Cairo.Matrix init(float|int xx, float|int yx, float|int xy, float|int x0, float|int y0)
 */
static void f_matrix_init(INT32 args)
{
  cairo_matrix_t* m = THIS_MATRIX;
  FLOAT_TYPE xx, yx, xy, yy, x0, y0;
  get_all_args("init", args, "%F%F%F%F%F%F", &xx, &yx, &xy, &yy, &x0, &y0);
  pop_n_elems(args);
  cairo_matrix_init(m, xx, yx, xy, yy, x0, y0);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Matrix init_identity()
 */
static void f_matrix_init_identity(INT32 args)
{
  cairo_matrix_t* m = THIS_MATRIX;
  cairo_matrix_init_identity(m);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Matrix init_translation(float|int x, float|int y)
 */
static void f_matrix_init_translate(INT32 args)
{
  cairo_matrix_t* m = THIS_MATRIX;
  FLOAT_TYPE x, y;
  get_all_args("init_translate", args, "%F%F", &x, &y);
  pop_n_elems(args);
  cairo_matrix_init_translate(m, x, y);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Matrix init_scale(float|int sx, float|int sy)
 */
static void f_matrix_init_scale(INT32 args)
{
  cairo_matrix_t* m = THIS_MATRIX;
  FLOAT_TYPE sx, sy;
  get_all_args("init_scale", args, "%F%F", &sx, &sy);
  pop_n_elems(args);
  cairo_matrix_init_scale(m, sx, sy);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Matrix init_rotate(float|int r)
 */
static void f_matrix_init_rotate(INT32 args)
{
  cairo_matrix_t* m = THIS_MATRIX;
  FLOAT_TYPE r;
  get_all_args("init_rotate", args, "%F", &r);
  pop_n_elems(args);
  cairo_matrix_rotate(m, r);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Matrix translate(float|int tx, float|int ty)
 */
static void f_matrix_translate(INT32 args)
{
  cairo_matrix_t* m = THIS_MATRIX;
  FLOAT_TYPE tx, ty;
  get_all_args("translate", args, "%F%F", &tx, &ty);
  pop_n_elems(args);
  cairo_matrix_translate(m, tx, ty);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Matrix scale(float|int sx, float|int sy)
 */
static void f_matrix_scale(INT32 args)
{
  cairo_matrix_t* m = THIS_MATRIX;
  FLOAT_TYPE sx, sy;
  get_all_args("scale", args, "%F%F", &sx, &sy);
  pop_n_elems(args);
  cairo_matrix_scale(m, sx, sy);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Matrix rotate(float|int r)
 */
static void f_matrix_rotate(INT32 args)
{
  cairo_matrix_t* m = THIS_MATRIX;
  FLOAT_TYPE r;
  get_all_args("rotate", args, "%F", &r);
  pop_n_elems(args);
  cairo_matrix_rotate(m, r);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Matrix invert()
 */
static void f_matrix_invert(INT32 args)
{
  cairo_matrix_t* m = THIS_MATRIX;
  cairo_matrix_invert(m);
  ref_push_object(THIS_OBJ);
}

/*! @decl Cairo.Matrix multiply(Cairo.Matrix m)
 */
static void f_matrix_multiply(INT32 args)
{
  cairo_matrix_t *m = THIS_MATRIX;
  cairo_matrix_t *m2;
  struct object *o1, *o2;
  get_all_args("multiply", args, "%o", &o1);
  pop_n_elems(args);

  if (!(m2 = (cairo_matrix_t *)get_storage(o1, cairo_mod_matrix_program)))
    Pike_error("Object is not Matrix");

  cairo_matrix_multiply(m, m, m2);
  ref_push_object(THIS_OBJ);
}

/*! @decl array(float) transform_distance()
 */
static void f_matrix_transform_distance(INT32 args)
{
  cairo_matrix_t* m = THIS_MATRIX;
  double dx, dy;
  cairo_matrix_transform_distance(m, &dx, &dy);
  push_float((FLOAT_TYPE)dx);
  push_float((FLOAT_TYPE)dy);
  f_aggregate(2);
}

/*! @decl array(float) transform_point()
 */
static void f_matrix_transform_point(INT32 args)
{
  cairo_matrix_t* m = THIS_MATRIX;
  double x, y;
  cairo_matrix_transform_point(m, &x, &y);
  push_float((FLOAT_TYPE)x);
  push_float((FLOAT_TYPE)y);
  f_aggregate(2);
}

/*! @decl void _sprintf();
 */
static void f_matrix_sprintf(INT32 args)
{
  cairo_matrix_t* m = THIS_MATRIX;
  INT_TYPE mode;
  struct mapping *opts;

  get_all_args("_sprintf", args, "%i%m", &mode, &opts);
  pop_n_elems (args);
  if (mode == 'O')
    {
      push_constant_text ("%O(%f,%f,%f,%f,%f,%f)");
      ref_push_object(Pike_fp->current_object);
      f_object_program(1);
      push_float(m->xx);
      push_float(m->yx);
      push_float(m->xy);
      push_float(m->yy);
      push_float(m->x0);
      push_float(m->y0);
      f_sprintf(8);
    }
  else
    push_undefined();
}

/*! @endclass */

void cairo_mod_init_matrix()
{
  ptrdiff_t off;

  start_new_program();
  /* Cairo.Matrix */
  {
    off = ADD_STORAGE(cairo_matrix_t);
    ADD_FUNCTION("init", f_matrix_init, tFunc(tNumber tNumber tNumber tNumber tNumber tNumber,tObj), ID_PUBLIC);
    ADD_FUNCTION("init_identity", f_matrix_init_identity, tFunc(tNone,tObj), ID_PUBLIC);
    ADD_FUNCTION("init_translate", f_matrix_init_translate, tFunc(tNumber tNumber,tObj), ID_PUBLIC);
    ADD_FUNCTION("init_scale", f_matrix_init_scale, tFunc(tNumber tNumber,tObj), ID_PUBLIC);
    ADD_FUNCTION("init_rotate", f_matrix_init_rotate, tFunc(tNumber,tObj), ID_PUBLIC);
    ADD_FUNCTION("translate", f_matrix_translate, tFunc(tNumber tNumber,tObj), ID_PUBLIC);
    ADD_FUNCTION("scale", f_matrix_scale, tFunc(tNumber tNumber,tObj), ID_PUBLIC);
    ADD_FUNCTION("rotate", f_matrix_rotate, tFunc(tNumber,tObj), ID_PUBLIC);
    ADD_FUNCTION("invert", f_matrix_invert, tFunc(tNumber tNumber,tObj), ID_PUBLIC);
    ADD_FUNCTION("multiply", f_matrix_multiply, tFunc(tObj,tObj), ID_PUBLIC);
    ADD_FUNCTION("transform_distance", f_matrix_transform_distance, tFunc(tNone,tArray), ID_PUBLIC);
    ADD_FUNCTION("transform_point", f_matrix_transform_point, tFunc(tNone,tArray), ID_PUBLIC);
    ADD_FUNCTION("_sprintf", f_matrix_sprintf, tFunc(tInt tMapping, tStr), ID_STATIC);
  }
  add_program_constant("Matrix", cairo_mod_matrix_program = end_program(), 0);
}

/*! @endmodule */

#endif // HAVE_CAIRO

