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

/*! @module Cairo
 */

/*! @class Path
 */

#ifdef HAVE_CAIRO

#ifdef THIS
#undef THIS
#endif
#define THIS ((struct cairo_mod_path *)Pike_fp->current_storage)

struct program *cairo_mod_path_program = NULL;
struct program *cairo_mod_path_iterator_program = NULL;
struct program *cairo_mod_path_element_program = NULL;

static void init_cairo_mod_path( struct object *o )
{
  MEMSET(THIS, 0, sizeof(struct cairo_mod_path));
}

static void exit_cairo_mod_path( struct object *o )
{
  if (THIS->path)
    cairo_path_destroy (THIS->path);
}

static void f_path_create(INT32 args)
{
}

/*! @decl PathIterator _get_iterator()
 *!
 *! Get an iterator for a given path. The iterator is implicitly used in the
 *! @tt{foreach(;;)@} statement.
 *!
 *! @example
 *!
 *! Cairo.Context ctx = ...;
 *! Cairo.Path p = ctx->copy_path();
 *! foreach(p; int i; Cairo.PathElement e)
 *!   {
 *!     ... examine e here ...
 *!   }
 *!
 */
static void f_path_get_iterator(INT32 args)
{
  struct object* iterator_object;
  struct cairo_mod_path_iterator* iterator;

  iterator_object = clone_object(cairo_mod_path_iterator_program, 0);
  iterator = (struct cairo_mod_path_iterator*)get_storage(iterator_object,
                                                          cairo_mod_path_iterator_program);
  assert(iterator);
  iterator->path_obj = Pike_fp->current_object;
  add_ref(Pike_fp->current_object);
  push_object(iterator_object);
}

/*! @endclass */

/*! @class PathIterator
 */

#ifdef THIS
#undef THIS
#endif
#define THIS ((struct cairo_mod_path_iterator *)Pike_fp->current_storage)

static void init_cairo_mod_path_iterator(struct object *o)
{
  MEMSET(THIS, 0, sizeof(struct cairo_mod_path_iterator));
}

static void exit_cairo_mod_path_iterator(struct object *o)
{
  if (THIS->path_obj)
    sub_ref(THIS->path_obj);
}

/*! @decl int(0..1) `!()
 *! Checks if the iterator is pointing to a valid path element of the path.
 */
static void f_path_iterator_not_operator(INT32 args)
{
  struct cairo_mod_path* pw;
  cairo_path_t* path;
  int j,i;

  pw = (struct cairo_mod_path*)get_storage(THIS->path_obj, cairo_mod_path_program);
  path = pw->path;

  for (i=0,j=0; j<=THIS->idx && i<path->num_data; i+= path->data[i].header.length, j++);
  push_int(!(THIS->idx >= 0 && THIS->idx <= j));
}

/*! @decl void `+=(int step)
 *! Move @tt{step@} steps forward in the path.
 */
static void f_path_iterator_add_self(INT32 args)
{
  int i;
  get_all_args("`+=", args, "%d", &i);
  THIS->idx += i;
  pop_n_elems(args);
  ref_push_object(Pike_fp->current_object);
}

/*! @decl int index()
 *! Return the current index of the path element that the iterator is pointing at.
 */
static void f_path_iterator_index(INT32 args)
{
  push_int(THIS->idx);
}

/*! @decl int value()
 *! Return the current path element that the iterator is pointing at.
 */
static void f_path_iterator_value(INT32 args)
{
  struct cairo_mod_path* pw;
  struct object* o;
  struct cairo_mod_path_element* pew;
  cairo_path_t* path;
  cairo_path_data_t* data = NULL;
  int j,i;

  pw = (struct cairo_mod_path*)get_storage(THIS->path_obj, cairo_mod_path_program);
  path = pw->path;

  for (i=0, j=0; i<path->num_data; i+= path->data[i].header.length, j++)
    {
      if (j == THIS->idx)
        {
          data = &path->data[i];
          break;
        }
    }

  if (!data)
    {
      
    }
    
  o = clone_object(cairo_mod_path_element_program, 0);
  pew = (struct cairo_mod_path_element*)get_storage(o, cairo_mod_path_element_program);
  pew->path_obj = THIS->path_obj;
  add_ref(THIS->path_obj);
  pew->element = data;
  ref_push_object(o);
}

static void f_path_iterator_next(INT32 args)
{
  struct cairo_mod_path* pw;
  cairo_path_t* path;
  int j,i;

  pw = (struct cairo_mod_path*)get_storage(THIS->path_obj, cairo_mod_path_program);
  path = pw->path;
  THIS->idx++;
  for (i=0,j=0; j<=THIS->idx && i<path->num_data; i+= path->data[i].header.length, j++);
  push_int(THIS->idx >= 0 && THIS->idx < j);
}

static void f_path_iterator_first(INT32 args)
{
  struct cairo_mod_path* pw;
  cairo_path_t* path;
  int j,i;

  pw = (struct cairo_mod_path*)get_storage(THIS->path_obj, cairo_mod_path_program);
  path = pw->path;

  THIS->idx = 0;
  push_int(path->num_data > 0);
}

/*! @endclass */

/*! @class PathElement
 */

#ifdef THIS
#undef THIS
#endif
#define THIS ((struct cairo_mod_path_element *)Pike_fp->current_storage)

static void init_cairo_mod_path_element(struct object *o)
{
  MEMSET(THIS, 0, sizeof(struct cairo_mod_path_element));
}

static void exit_cairo_mod_path_element(struct object* o)
{
  if (THIS->path_obj)
    sub_ref(THIS->path_obj);
}

/*! @decl int get_type()
 */
static void f_path_element_get_type(INT32 args)
{
  push_int(THIS->element->header.type);
}

/*! @decl mapping(string:float) get_point(int point)
 */
static void f_path_element_get_point(INT32 args)
{
  int i;
  get_all_args("get_point", args, "%d", &i);
  i++; // index includes header
  if (i > 0 && i < THIS->element->header.length)
    {
      push_text( "x" );
      push_float(THIS->element[i].point.x);
      push_text( "y" );
      push_float(THIS->element[i].point.y);
      f_aggregate_mapping(4);
    }
  else
    push_undefined();
}

/*! @decl string _sprintf()
 */
static void f_path_element_sprintf(INT32 args)
{
  INT_TYPE mode;
  struct mapping *opts;
  cairo_path_data_t* data = THIS->element;

  get_all_args("_sprintf", args, "%i%m", &mode, &opts);
  pop_n_elems (args);
  if (mode == 'O')
    {
      switch (data->header.type)
        {
        case CAIRO_PATH_MOVE_TO:
          push_constant_text ("%O(CAIRO_PATH_MOVE_TO(%f,%f))");
          ref_push_object(Pike_fp->current_object);
          f_object_program (1);
          push_float(data[1].point.x); push_float(data[1].point.y);
          f_sprintf(4);
          break;
        case CAIRO_PATH_LINE_TO:
          push_constant_text ("%O(CAIRO_PATH_LINE_TO(%f,%f))");
          ref_push_object(Pike_fp->current_object);
          f_object_program (1);
          push_float(data[1].point.x); push_float(data[1].point.y);
          f_sprintf(4);
          break;
        case CAIRO_PATH_CURVE_TO:
          push_constant_text ("%O(CAIRO_PATH_CURVE_TO(%f,%f,%f,%f,%f))");
          ref_push_object(Pike_fp->current_object);
          f_object_program (1);
          push_float(data[1].point.x); push_float(data[1].point.y);
          push_float(data[2].point.x); push_float(data[2].point.y);
          push_float(data[3].point.x); push_float(data[3].point.y);
          f_sprintf(8);
          break;
        case CAIRO_PATH_CLOSE_PATH:
          push_constant_text ("%O(CAIRO_PATH_CLOSE_PATH()");
          ref_push_object(Pike_fp->current_object);
          f_object_program (1);
          f_sprintf(2);
          break;
        default:
          push_undefined();
          return;
        }
    }
  else
    push_undefined();
}

/*! @endclass */

void cairo_mod_init_path(void)
{
  /*! @decl constant CAIRO_PATH_MOVE_TO
   *! @decl constant CAIRO_PATH_LINE_TO
   *! @decl constant CAIRO_PATH_CURVE_TO
   *! @decl constant CAIRO_PATH_CLOSE_PATH
   *!
   *! The various types of path elements in an Cairo path
   */
  ADD_INT_CONSTANT("CAIRO_PATH_MOVE_TO", CAIRO_PATH_MOVE_TO, 0);
  ADD_INT_CONSTANT("CAIRO_PATH_LINE_TO", CAIRO_PATH_LINE_TO, 0);
  ADD_INT_CONSTANT("CAIRO_PATH_CURVE_TO", CAIRO_PATH_CURVE_TO, 0);
  ADD_INT_CONSTANT("CAIRO_PATH_CLOSE_PATH", CAIRO_PATH_CLOSE_PATH, 0);

  /* Cairo.Path */
  start_new_program();
  {
    ADD_STORAGE(struct cairo_mod_path);
    ADD_FUNCTION("_get_iterator", f_path_get_iterator, tFunc(tVoid,tObj), ID_PUBLIC);
    set_init_callback(init_cairo_mod_path);
    set_exit_callback(exit_cairo_mod_path);
  }
  add_program_constant("Path",
                       cairo_mod_path_program = end_program(),
                       0);

  /* Cairo.PathElement */
  start_new_program();
  {
    ADD_STORAGE(struct cairo_mod_path_element);
    ADD_FUNCTION("get_type", f_path_element_get_type, tFunc(tNone,tInt), ID_PUBLIC);
    ADD_FUNCTION("get_point", f_path_element_get_point, tFunc(tInt, tMapping), ID_PUBLIC);
    ADD_FUNCTION("_sprintf", f_path_element_sprintf, tFunc(tInt tMapping, tStr), ID_STATIC);
    set_init_callback(init_cairo_mod_path_element);
    set_exit_callback(exit_cairo_mod_path_element);
  }
  add_program_constant("PathElement",
                       cairo_mod_path_element_program = end_program(),
                       0);

  /* Cairo.PathIterator */
  start_new_program();
  {
    ADD_STORAGE(struct cairo_mod_path_iterator);
    ADD_FUNCTION("`!", f_path_iterator_not_operator, tFunc(tVoid,tInt), ID_PUBLIC);
    ADD_FUNCTION("`+=", f_path_iterator_add_self, tFunc(tInt,tObj), ID_PUBLIC);
    ADD_FUNCTION("index", f_path_iterator_index, tFunc(tNone,tObj), ID_PUBLIC);
    ADD_FUNCTION("value", f_path_iterator_value, tFunc(tNone,tObj), ID_PUBLIC);
    ADD_FUNCTION("first", f_path_iterator_first, tFunc(tNone,tInt), ID_PUBLIC);
    ADD_FUNCTION("next", f_path_iterator_next, tFunc(tNone,tObj), ID_PUBLIC);
    set_init_callback(init_cairo_mod_path_iterator);
    set_exit_callback(exit_cairo_mod_path_iterator);
  }
  add_program_constant("PathIterator",
                       cairo_mod_path_iterator_program = end_program(),
                       0);
}

/*! @endmodule */

#endif // HAVE_CAIRO
