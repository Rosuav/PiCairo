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

/* Pike includes */
#include <global.h>
#include <svalue.h>
#include <object.h>
#include <array.h>
#include <stralloc.h>
#include <interpret.h>
#include <pike_types.h>
#include <pike_memory.h>
#include <threads.h>
#include <module_support.h>
#include <mapping.h>
#include <multiset.h>
#include <builtin_functions.h>
#include <opcodes.h>
#include <pike_macros.h>
#include <version.h>
#include <program.h>
#include <operators.h>

#include <assert.h>

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

/* GTK includes */
#ifdef HAVE_GTK2
# include <gtk/gtk.h>
# include <gdk/gdkkeysyms.h>
#endif // HAVE_GTK2

/* Cairo includes */
#ifdef HAVE_CAIRO
# include <cairo.h>
#endif // HAVE_CAIRO

/* From Pike modules sources (these are not installed) */

#if defined(HAVE_GTK2)
/*  From pgtk.h */
struct object_wrapper {
  GObject *obj;
  int extra_int;
  void *extra_data;
};
#endif // defined(HAVE_GTK2)
/*  End from pgtk.h */

/* Programs */
extern struct program *cairo_mod_context_program;
extern struct program *gtk2_cairo_mod_context_program;
extern struct program *cairo_mod_surface_program;
extern struct program *cairo_mod_image_surface_program;
extern struct program *cairo_mod_path_program;
extern struct program *cairo_mod_path_iterator_program;
extern struct program *cairo_mod_path_element_program;
extern struct program *cairo_mod_matrix_program;
extern struct program *cairo_mod_font_options_program;
extern struct program *cairo_mod_pattern_program;
extern struct program *cairo_mod_gradient_program;
extern struct program *cairo_mod_linear_gradient_program;
extern struct program *cairo_mod_radial_gradient_program;
extern struct program *cairo_mod_surface_pattern_program;

extern struct program *pgtk_window_program;

#ifdef HAVE_CAIRO

extern void cairo_pike_set_error (cairo_status_t err);

/* Storages */
struct cairo_mod_path
{
  cairo_path_t* path;
};

struct cairo_mod_surface
{
  cairo_surface_t* surface;
  unsigned char* data;
  cairo_format_t format;
};

struct cairo_mod_context
{
  cairo_t* ctx;
};

struct cairo_mod_path_iterator
{
  struct object* path_obj;
  int idx;
};

struct cairo_mod_font_options
{
  cairo_font_options_t* opt;
};

struct cairo_mod_path_element
{
  struct object* path_obj;
  cairo_path_data_t* element;
};

struct cairo_mod_pattern
{
  cairo_pattern_t* pattern;
};

extern void cairo_mod_init_surface(void);
extern void cairo_mod_init_path(void);
extern void cairo_mod_init_matrix(void);
extern void cairo_mod_init_font(void);
extern void cairo_mod_init_pattern(void);
extern void cairo_mod_init_error(void);
#endif // HAVE_CAIRO

/* Misc. defines */
#define tNumber tOr(tFloat,tInt)

#define THIS_OBJ Pike_fp->current_object

#if defined(HAVE_GTK2) && defined(HAVE_CAIRO)
#include <gtk/gtkversion.h>
# if ! GTK_CHECK_VERSION(2,7,0)
cairo_t *gdk_cairo_create (GdkDrawable *drawable);
# endif // !GTK_CHECK_VERSION(2,7,0)
#endif // defined(HAVE_GTK2) && defined(HAVE_CAIRO)
