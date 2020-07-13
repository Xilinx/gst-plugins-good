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

#include <chrono>
#include <vector>
#include "gstface.h"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <vitis/ai/facedetect.hpp>
#include <vitis/ai/nnpp/facedetect.hpp>

using namespace std;
using namespace cv;
using namespace std::chrono;

std::unique_ptr < vitis::ai::FaceDetect > det;

void
densebox_init ()
{
  det = vitis::ai::FaceDetect::create ("densebox_640_360");
}

void
roi_align_data (GstRoiData * roi_region_data)
{
  guint quotient, i;
  for (i = 0; i < roi_region_data->num; i++) {
    quotient = roi_region_data->roi_coordinates[i].xmin / 32;
    roi_region_data->roi_coordinates[i].xmin = 32 * quotient;

    quotient = roi_region_data->roi_coordinates[i].ymin / 32;
    roi_region_data->roi_coordinates[i].ymin = 32 * quotient;
    if (roi_region_data->roi_coordinates[i].width % 32) {
      quotient = roi_region_data->roi_coordinates[i].width / 32;
      roi_region_data->roi_coordinates[i].width =
          std::min ((guint) 32 * (quotient + 1), roi_region_data->actual_width);
    }
    if (roi_region_data->roi_coordinates[i].height % 32) {
      quotient = roi_region_data->roi_coordinates[i].height / 32;
      roi_region_data->roi_coordinates[i].height =
          std::min ((guint) 32 * (quotient + 1),
          roi_region_data->actual_height);
    }
  }
}

void
dpu_run_task (GstRoiData * roi_region_data, unsigned char *buf)
{
  guint i = 0;
  float
      scaleW = ((float) roi_region_data->actual_width) / DPU_INPUT_WIDTH;
  float
      scaleH = ((float) roi_region_data->actual_height) / DPU_INPUT_HEIGHT;

  Mat image (DPU_INPUT_HEIGHT, DPU_INPUT_WIDTH, CV_8UC3, buf, DPU_INPUT_STRIDE);
  auto result = det->run (image);
  roi_region_data->num = result.rects.size();
  if (roi_region_data->num > 0)
    roi_region_data->roi_coordinates =
        (GstRoiCoordinates *) g_malloc0 (roi_region_data->num *
        sizeof (GstRoiCoordinates));

  for (auto &box : result.rects) {
    float x = box.x * (image.cols);
    float y = box.y * (image.rows);
    int xmin = x;
    int ymin = y;
    int xmax = (box.width) * (image.cols);
    int ymax = (box.height) * (image.rows);

    xmin = std::min (std::max (xmin, 0), image.cols);
    xmax = std::min (std::max (xmax, 0), image.cols);
    ymin = std::min (std::max (ymin, 0), image.rows);
    ymax = std::min (std::max (ymax, 0), image.rows);

    roi_region_data->roi_coordinates[i].xmin =
        std::min ((xmin * scaleW), (float) roi_region_data->actual_width);
    roi_region_data->roi_coordinates[i].width =
        std::min ((xmax * scaleW), (float) roi_region_data->actual_width);
    roi_region_data->roi_coordinates[i].ymin =
        std::min ((ymin * scaleH), (float) roi_region_data->actual_height);
    roi_region_data->roi_coordinates[i].height =
        std::min ((ymax * scaleH), (float) roi_region_data->actual_height);

    i++;
  }
}
