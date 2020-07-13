/*
 * Copyright (C) 2014 Collabora Ltd.
 *     Author: Nicolas Dufresne <nicolas.dufresne@collabora.co.uk>
 * Copyright (C) 2020-2021 Xilinx, Inc.
 *     Author: Saket Kumar Bafna <saketb@xilinx.com>
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
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef __GST_XLNX_ROI_H__
#define __GST_XLNX_ROI_H__

#include <gst/gst.h>
#include <gst/base/gstbasetransform.h>

#include <gstv4l2object.h>
#include <gstv4l2bufferpool.h>
#include "gstface.h"

#ifdef __cplusplus
extern "C" {
#endif

G_BEGIN_DECLS

#define GST_TYPE_XLNX_ROI \
  (gst_xlnx_roi_get_type())
#define GST_XLNX_ROI(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_XLNX_ROI,GstXlnxRoi))
#define GST_XLNX_ROI_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_XLNX_ROI,GstXlnxRoiClass))
#define GST_IS_XLNX_ROI(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_XLNX_ROI))
#define GST_IS_XLNX_ROI_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_XLNX_ROI))
#define GST_XLNX_ROI_GET_CLASS(inst) \
  (G_TYPE_INSTANCE_GET_CLASS ((inst),GST_TYPE_XLNX_ROI,GstXlnxRoiClass))

typedef struct _GstXlnxRoi GstXlnxRoi;
typedef struct _GstXlnxRoiClass GstXlnxRoiClass;
typedef struct _GstXlnxRoiPrivate GstXlnxRoiPrivate;

struct _GstXlnxRoi
{
  GstBaseTransform parent;

  /* < private > */
  GstV4l2Object * v4l2output;
  GstV4l2Object * v4l2capture;

  /* pads */
  GstCaps *probed_srccaps;
  GstCaps *probed_sinkcaps;

  /* Selected caps */
  GstCaps *incaps;
  GstCaps *outcaps;

  gint relative_qp;
  gboolean disable_passthrough;
  gboolean import_buffer_alignment;
};

struct _GstXlnxRoiClass
{
  GstBaseTransformClass parent_class;
  gchar *default_device;
};

GType gst_xlnx_roi_get_type (void);

gboolean gst_xlnx_roi_is_transform (GstCaps * sink_caps, GstCaps * src_caps);
void gst_xlnx_roi_register (GstPlugin * plugin, const gchar *basename,
     const gchar *device_path, GstCaps * sink_caps, GstCaps * src_caps);

G_END_DECLS
#ifdef __cplusplus
}
#endif

#endif /* __GST_XLNX_ROI_H__ */
