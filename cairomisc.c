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

#if defined(HAVE_GTK2)
# if !GTK_CHECK_VERSION(2,7,0)
/* copied from gtk+/gdk/gdkcairo.c and gtk+/gdk/x11/gdkdrawable-x11.c
 * gdk_cairo_create() which is available in 2.7.0 and later.
 */

#include <gdk/gdkx.h>

cairo_t *
gdk_cairo_create (GdkDrawable *drawable)
{
    int width, height;
    cairo_t *cr = NULL;
    cairo_surface_t *surface = NULL;
    GdkVisual *visual = gdk_drawable_get_visual (drawable);

    gdk_drawable_get_size (drawable, &width, &height);
    if (visual) 
	surface = cairo_xlib_surface_create (GDK_DRAWABLE_XDISPLAY (drawable),
					     GDK_DRAWABLE_XID (drawable),
					     GDK_VISUAL_XVISUAL (visual),
					     width, height);
    else if (gdk_drawable_get_depth (drawable) == 1)
	surface = cairo_xlib_surface_create_for_bitmap 
	    (GDK_PIXMAP_XDISPLAY (drawable),
	     GDK_PIXMAP_XID (drawable),
	     GDK_SCREEN_XSCREEN (gdk_drawable_get_screen (drawable)),
	     width, height);
    else {
	g_warning ("Using Cairo rendering requires the drawable argument to\n"
		   "have a specified colormap. All windows have a colormap,\n"
		   "however, pixmaps only have colormap by default if they\n"
		   "were created with a non-NULL window argument. Otherwise\n"
		   "a colormap must be set on them with "
		   "gdk_drawable_set_colormap");
	return NULL;
    }
    if (surface) {
	cr = cairo_create (surface);
	cairo_surface_destroy (surface);
    }
    return cr;
}
# endif // !GTK_CHECK_VERSION(2,7,0)
#endif // defined(HAVE_GTK2)
