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

#define THIS_FONT_OPTIONS ((struct cairo_mod_font_options *)Pike_fp->current_storage)

struct program *cairo_mod_font_options_program = NULL;

static void init_cairo_mod_font_options(struct object *o)
{
  struct cairo_mod_font_options *fo = THIS_FONT_OPTIONS;
  fo->opt = NULL;
}

static void exit_cairo_mod_font_options(struct object *o)
{
  struct cairo_mod_font_options *fo = THIS_FONT_OPTIONS;
  cairo_font_options_destroy(fo->opt);
}

/*! @module Cairo
 */

/*! @class FontOptions
 */

/*! @decl void create()
 */
static void f_fo_create(INT32 args)
{
  struct cairo_mod_font_options *fo = THIS_FONT_OPTIONS;
  fo->opt = cairo_font_options_create();
}

/*! @decl Cairo.FontOptions copy()
 */
static void f_fo_copy(INT32 args)
{
  struct cairo_mod_font_options *fo = THIS_FONT_OPTIONS;
  struct cairo_mod_font_options *dest_fo;
  struct object *new_o;

  new_o = clone_object(cairo_mod_font_options_program, 0);
  dest_fo = (struct cairo_mod_font_options *)get_storage(new_o, cairo_mod_font_options_program);
  assert(dest_fo);
  dest_fo->opt = cairo_font_options_copy(fo->opt);
  push_object(new_o);
}

/*! @decl int status()
 */
static void f_fo_status(INT32 args)
{
  struct cairo_mod_font_options *fo = THIS_FONT_OPTIONS;
  push_int((INT_TYPE)cairo_font_options_status(fo->opt));
}

/*! @decl void merge(Cairo.FontOptions options)
 */
static void f_fo_merge(INT32 args)
{
  struct cairo_mod_font_options *fo = THIS_FONT_OPTIONS;
  struct cairo_mod_font_options *other;
  struct object* o;

  get_all_args("merge", args, "%o", &o);
  if (!(other = (struct cairo_mod_font_options *)get_storage(o, cairo_mod_font_options_program)))
    Pike_error("Object is not FontOptions");

  cairo_font_options_merge(fo->opt, other->opt);
}

/*! @decl int(0..1) equal(Cairo.FontOptions options)
 */
static void f_fo_equal(INT32 args)
{
  struct cairo_mod_font_options *fo = THIS_FONT_OPTIONS;
  struct cairo_mod_font_options *other;
  struct object* o;

  get_all_args("equal", args, "%o", &o);
  if (!(other = (struct cairo_mod_font_options *)get_storage(o, cairo_mod_font_options_program)))
    Pike_error("Object is not FontOptions");

  push_int((INT_TYPE)cairo_font_options_equal (fo->opt, other->opt));
}

static void f_fo_hash(INT32 args)
{
  struct cairo_mod_font_options *fo = THIS_FONT_OPTIONS;

  /* FIXME: a long is casted to a INT_TYPE here, what about 64-bit
   * systems?  Is there are a 64-bit pike type to store it in? */
  push_int((INT_TYPE)cairo_font_options_hash(fo->opt));
}

static void f_fo_set_antialias(INT32 args)
{
  struct cairo_mod_font_options *fo = THIS_FONT_OPTIONS;
  INT_TYPE s;
  get_all_args("set_antialias", args, "%d", &s);
  pop_n_elems(args);
  cairo_font_options_set_antialias(fo->opt, (cairo_antialias_t)s);
}

static void f_fo_get_antialias(INT32 args)
{
  struct cairo_mod_font_options *fo = THIS_FONT_OPTIONS;
  INT_TYPE s = (INT_TYPE)cairo_font_options_get_antialias(fo->opt);
  push_int(s);
}

static void f_fo_set_subpixel_order(INT32 args)
{
  struct cairo_mod_font_options *fo = THIS_FONT_OPTIONS;
  INT_TYPE s;
  get_all_args("set_subpixel_order", args, "%d", &s);
  pop_n_elems(args);
  cairo_font_options_set_subpixel_order(fo->opt, (cairo_subpixel_order_t)s);
}

static void f_fo_get_subpixel_order(INT32 args)
{
  struct cairo_mod_font_options *fo = THIS_FONT_OPTIONS;
  INT_TYPE s = (INT_TYPE)cairo_font_options_get_subpixel_order(fo->opt);
  push_int(s);
}

static void f_fo_set_hint_style(INT32 args)
{
  struct cairo_mod_font_options *fo = THIS_FONT_OPTIONS;
  INT_TYPE s;
  get_all_args("set_hint_style", args, "%d", &s);
  pop_n_elems(args);
  cairo_font_options_set_hint_style(fo->opt, (cairo_hint_style_t)s);
}

static void f_fo_get_hint_style(INT32 args)
{
  struct cairo_mod_font_options *fo = THIS_FONT_OPTIONS;
  INT_TYPE s = (INT_TYPE)cairo_font_options_get_hint_style(fo->opt);
  push_int(s);
}

static void f_fo_set_hint_metrics(INT32 args)
{
  struct cairo_mod_font_options *fo = THIS_FONT_OPTIONS;
  INT_TYPE s;
  get_all_args("set_hint_metrics", args, "%d", &s);
  pop_n_elems(args);
  cairo_font_options_set_hint_style(fo->opt, (cairo_hint_metrics_t)s);
}

static void f_fo_get_hint_metrics(INT32 args)
{
  struct cairo_mod_font_options *fo = THIS_FONT_OPTIONS;
  INT_TYPE s = (INT_TYPE)cairo_font_options_get_hint_metrics(fo->opt);
  push_int(s);
}

/*! @endclass
 */

void cairo_mod_init_font()
{
  ADD_INT_CONSTANT("CAIRO_SUBPIXEL_ORDER_DEFAULT", (INT_TYPE)CAIRO_SUBPIXEL_ORDER_DEFAULT, 0);
  ADD_INT_CONSTANT("CAIRO_SUBPIXEL_ORDER_RGB", (INT_TYPE)CAIRO_SUBPIXEL_ORDER_RGB, 0);
  ADD_INT_CONSTANT("CAIRO_SUBPIXEL_ORDER_BGR", (INT_TYPE)CAIRO_SUBPIXEL_ORDER_BGR, 0);
  ADD_INT_CONSTANT("CAIRO_SUBPIXEL_ORDER_VRGB", (INT_TYPE)CAIRO_SUBPIXEL_ORDER_VRGB, 0);
  ADD_INT_CONSTANT("CAIRO_SUBPIXEL_ORDER_VBGR", (INT_TYPE)CAIRO_SUBPIXEL_ORDER_VBGR, 0);

  ADD_INT_CONSTANT("CAIRO_HINT_STYLE_DEFAULT", (INT_TYPE)CAIRO_HINT_STYLE_DEFAULT, 0);
  ADD_INT_CONSTANT("CAIRO_HINT_STYLE_NONE", (INT_TYPE)CAIRO_HINT_STYLE_NONE, 0);
  ADD_INT_CONSTANT("CAIRO_HINT_STYLE_SLIGHT", (INT_TYPE)CAIRO_HINT_STYLE_SLIGHT, 0);
  ADD_INT_CONSTANT("CAIRO_HINT_STYLE_MEDIUM", (INT_TYPE)CAIRO_HINT_STYLE_MEDIUM, 0);
  ADD_INT_CONSTANT("CAIRO_HINT_STYLE_FULL", (INT_TYPE)CAIRO_HINT_STYLE_FULL, 0);

  ADD_INT_CONSTANT("CAIRO_HINT_METRICS_DEFAULT", (INT_TYPE)CAIRO_HINT_METRICS_DEFAULT, 0);
  ADD_INT_CONSTANT("CAIRO_HINT_METRICS_OFF", (INT_TYPE)CAIRO_HINT_METRICS_OFF, 0);
  ADD_INT_CONSTANT("CAIRO_HINT_METRICS_ON", (INT_TYPE)CAIRO_HINT_METRICS_ON, 0);

  ADD_INT_CONSTANT("CAIRO_FONT_SLANT_NORMAL", (INT_TYPE)CAIRO_FONT_SLANT_NORMAL, 0);
  ADD_INT_CONSTANT("CAIRO_FONT_SLANT_ITALIC", (INT_TYPE)CAIRO_FONT_SLANT_ITALIC, 0);
  ADD_INT_CONSTANT("CAIRO_FONT_SLANT_OBLIQUE", (INT_TYPE)CAIRO_FONT_SLANT_OBLIQUE, 0);

  ADD_INT_CONSTANT("CAIRO_FONT_WEIGHT_NORMAL", (INT_TYPE)CAIRO_FONT_WEIGHT_NORMAL, 0);
  ADD_INT_CONSTANT("CAIRO_FONT_WEIGHT_BOLD", (INT_TYPE)CAIRO_FONT_WEIGHT_BOLD, 0);

  start_new_program();
  {
    ADD_STORAGE(struct cairo_mod_font_options);
    ADD_FUNCTION("create", f_fo_create, tFunc(tNone, tVoid), ID_PUBLIC);
    ADD_FUNCTION("copy", f_fo_copy, tFunc(tNone, tObj), ID_PUBLIC);
    ADD_FUNCTION("status", f_fo_status, tFunc(tNone, tInt), ID_PUBLIC);

    ADD_FUNCTION("merge", f_fo_merge, tFunc(tObj,tVoid), ID_PUBLIC);
    ADD_FUNCTION("equal", f_fo_equal, tFunc(tObj,tInt01), ID_PUBLIC);
    ADD_FUNCTION("hash", f_fo_hash, tFunc(tVoid,tInt), ID_PUBLIC);

    ADD_FUNCTION("set_antialias", f_fo_set_antialias, tFunc(tInt, tVoid), ID_PUBLIC);
    ADD_FUNCTION("get_antialias", f_fo_get_antialias, tFunc(tVoid,tInt), ID_PUBLIC); 
    ADD_FUNCTION("set_subpixel_order", f_fo_set_subpixel_order, tFunc(tInt, tVoid), ID_PUBLIC);
    ADD_FUNCTION("get_subpixel_order", f_fo_get_subpixel_order, tFunc(tVoid,tInt), ID_PUBLIC);
    ADD_FUNCTION("set_hint_style", f_fo_set_hint_style, tFunc(tInt, tVoid), ID_PUBLIC);
    ADD_FUNCTION("get_hint_style", f_fo_get_hint_style, tFunc(tVoid,tInt), ID_PUBLIC);
    ADD_FUNCTION("set_hint_metrics", f_fo_set_hint_metrics, tFunc(tInt, tVoid), ID_PUBLIC);
    ADD_FUNCTION("get_hint_metrics", f_fo_get_hint_metrics, tFunc(tVoid,tInt), ID_PUBLIC);

    set_init_callback(init_cairo_mod_font_options);
    set_exit_callback(exit_cairo_mod_font_options);
  }
  add_program_constant("FontOptions", cairo_mod_font_options_program = end_program(), 0);
}

/*! @endmodule */

#endif // HAVE_CAIRO
