/* GStreamer
 *
 * Copyright (C) 2019 Collabora Ltd.
 *   Author: Stéphane Cerveau <scerveau@collabora.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library (COPYING); if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
#ifndef __GST_MPDHELPER_H__
#define __GST_MPDHELPER_H__

#include "gstxmlhelper.h"

G_BEGIN_DECLS

typedef enum
{
  GST_MPD_FILE_TYPE_STATIC,
  GST_MPD_FILE_TYPE_DYNAMIC
} GstMPDFileType;

typedef enum
{
  GST_SAP_TYPE_0 = 0,
  GST_SAP_TYPE_1,
  GST_SAP_TYPE_2,
  GST_SAP_TYPE_3,
  GST_SAP_TYPE_4,
  GST_SAP_TYPE_5,
  GST_SAP_TYPE_6
} GstMPDSAPType;


gboolean gst_mpd_helper_get_mpd_type (xmlNode * a_node, const gchar * property_name, GstMPDFileType * property_value);
gboolean gst_mpd_helper_get_SAP_type (xmlNode * a_node, const gchar * property_name, GstMPDSAPType * property_value);
G_END_DECLS
#endif /* __GST_MPDHELPER_H__ */
