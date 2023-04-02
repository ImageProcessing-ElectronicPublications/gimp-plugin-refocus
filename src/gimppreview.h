/* GIMP Plugin Preview Widget
 * Copyright (C) 1998-1999 Shawn T. Amundson
 * Modifications (C) 1999-2003 Ernst Lippe
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Version $Id: gimppreview.h,v 1.1.1.1 2003/01/30 21:30:18 ernstl Exp $
 */

#ifndef GIMP_PREVIEW_H
#define GIMP_PREVIEW_H

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

G_BEGIN_DECLS

#define PREVIEW_FIXED_SIZE           0
#define PREVIEW_DEFAULT_SIZE         -1
#define GIMP_TYPE_PREVIEW            (gimp_preview_get_type ())
#define GIMP_PREVIEW(obj)            (GTK_CHECK_CAST ((obj), GIMP_TYPE_PREVIEW, GimpPreview))
#define GIMP_PREVIEW_CLASS(klass)    (GTK_CHECK_CLASS_CAST ((klass), GIMP_TYPE_PREVIEW, GimpPreviewClass))
#define GIMP_IS_PREVIEW(obj)         (GTK_CHECK_TYPE ((obj), GIMP_TYPE_PREVIEW))
#define GIMP_IS_PREVIEW_CLASS(klass) (GTK_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_PREVIEW))
typedef struct _GimpPreview GimpPreview;
typedef struct _GimpPreviewClass GimpPreviewClass;
typedef struct _GimpPreviewEvent GimpPreviewEvent;

struct _GimpPreviewClass
{
    GtkContainerClass parent_class;

    void (*update_preview) (GimpPreview * preview, GimpPreviewEvent * event);
    void (*preview_changed) (GimpPreview * preview);
};


struct _GimpPreview
{
    GtkContainer parent;

    /* TODO: most of these should be hidden and changed into properties */
    GimpDrawable *drawable;       /* The drawable that is shown */
    gint drawable_has_alpha;      /* Does the drawable have alpha */
    gdouble scale;                /* Scale of Preview   */
    gint width;                   /* Width of Preview   */
    gint height;                  /* Height of Preview  */
    gint max_width;
    gint max_height;


    /* Left/Top of image area viewed in the preview in preview coords */
    gint preview_x;
    gint preview_y;


    guchar *buffer;               /* Contains scaled image data as RGBA */

    gpointer private_data;
};

/**
 * GimpPreviewEvent:
 * @event_id: Id of this event. This is needed for gimp_preview_draw_row,
 * gimp_preview_draw_unscaled_row and gimp_preview_progress_set_fraction.
 * @scale: Current scale of the preview.
 *
 **/
struct _GimpPreviewEvent
{
    gint event_id;                /* Id of this event */
    gdouble scale;                /* Scale of preview */

    /* Left/Top of requested unscaled data in image coordinates */
    gint image_x;
    gint image_y;

    /* Width/Height of requested unscaled data in image coordinates */
    gint image_width;
    gint image_height;

    /* Left/Top of preview in preview coordinates */
    gint preview_x;
    gint preview_y;

    /* Width/Height of the preview */
    gint preview_width;
    gint preview_height;

    /* Contains scaled image data as RGBA (4 bytes per pixel).
       The size of the array is 4 * preview_height * preview_width bytes */
    guchar *scaled_data;
};

GtkType gimp_preview_get_type (void);
GtkWidget *gimp_preview_new (GimpDrawable * drawable);
GtkWidget *gimp_preview_new_with_args (GimpDrawable * drawable,
                                       gint preview_size,
                                       gdouble scale_amount,
                                       gint allow_scale);
void gimp_preview_update (GimpPreview * preview);

gboolean gimp_preview_draw_row (GimpPreview * preview, const gint event_id,
                                GimpImageType type, const gint row,
                                const guchar * const data);

gboolean gimp_preview_draw_unscaled_row (GimpPreview * preview,
        const gint event_id,
        GimpImageType type, const gint row,
        const guchar * const data);

void gimp_preview_force_redraw (GimpPreview * preview);

gboolean gimp_preview_progress_set_fraction (GimpPreview * preview,
        const gint event_id,
        double fraction);

G_END_DECLS
#endif /* __GIMP_PREVIEW_H__ */
