/*
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

#ifndef __GST_ROI_H__
#define __GST_ROI_H__

#include <stdio.h>
#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/video/gstvideofilter.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define DPU_INPUT_WIDTH           640
#define DPU_INPUT_HEIGHT          360
#define DPU_INPUT_STRIDE          1920
#define DPU_INPUT_FORMAT          "BGR"

typedef struct
{
  guint xmin;
  guint ymin;
  guint width;
  guint height;
} GstRoiCoordinates;

typedef struct
{
  GstRoiCoordinates *roi_coordinates;
  guint num;
  guint actual_width, actual_height;
} GstRoiData;

void densebox_init ();
void dpu_run_task (GstRoiData *roi_region_data, unsigned char *buf);
void roi_align_data (GstRoiData *roi_region_data);

#ifdef __cplusplus
}
#endif

#endif /* __GST_ROI_H__ */
