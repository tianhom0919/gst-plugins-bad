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
#ifndef __GST_MPDCLIENT_H__
#define __GST_MPDCLIENT_H__

#include "gstmpdparser.h"

G_BEGIN_DECLS

#define GST_MPD_CLIENT_LOCK(c) g_mutex_lock (&c->lock);
#define GST_MPD_CLIENT_UNLOCK(c) g_mutex_unlock (&c->lock);

typedef struct _GstMpdClient              GstMpdClient;

struct _GstMpdClient
{
  GstMPDNode *mpd_node;                       /* active MPD manifest file */

  GList *periods;                             /* list of GstStreamPeriod */
  guint period_idx;                           /* index of current Period */

  GList *active_streams;                      /* list of GstActiveStream */

  guint update_failed_count;
  gchar *mpd_uri;                             /* manifest file URI */
  gchar *mpd_base_uri;                        /* base URI for resolving relative URIs.
                                               * this will be different for redirects */

  /* profiles */
  gboolean profile_isoff_ondemand;

  GstUriDownloader * downloader;
};

/* Basic initialization/deinitialization functions */
GstMpdClient *gst_mpd_client_new (void);
void gst_mpd_client_active_streams_free (GstMpdClient * client);
void gst_mpd_client_free (GstMpdClient * client);

/* main mpd parsing methods from xml data */
gboolean gst_mpd_client_parse (GstMpdClient * client, const gchar * data, gint size);

void gst_mpd_client_set_uri_downloader (GstMpdClient * client, GstUriDownloader * download);
void  gst_mpd_client_check_profiles (GstMpdClient * client);
void gst_mpd_client_fetch_on_load_external_resources (GstMpdClient * client);

/* Streaming management */
gboolean gst_mpd_client_setup_media_presentation (GstMpdClient *client, GstClockTime time, gint period_index, const gchar *period_id);
gboolean gst_mpd_client_setup_streaming (GstMpdClient * client, GstAdaptationSetNode * adapt_set);
gboolean gst_mpd_client_setup_representation (GstMpdClient *client, GstActiveStream *stream, GstRepresentationNode *representation);

GstClockTime gst_mpd_client_get_next_fragment_duration (GstMpdClient * client, GstActiveStream * stream);
GstClockTime gst_mpd_client_get_media_presentation_duration (GstMpdClient *client);
GstClockTime gst_mpd_client_get_maximum_segment_duration (GstMpdClient * client);
gboolean gst_mpd_client_get_last_fragment_timestamp_end (GstMpdClient * client, guint stream_idx, GstClockTime * ts);
gboolean gst_mpd_client_get_next_fragment_timestamp (GstMpdClient * client, guint stream_idx, GstClockTime * ts);
gboolean gst_mpd_client_get_next_fragment (GstMpdClient *client, guint indexStream, GstMediaFragmentInfo * fragment);
gboolean gst_mpd_client_get_next_header (GstMpdClient *client, gchar **uri, guint stream_idx, gint64 * range_start, gint64 * range_end);
gboolean gst_mpd_client_get_next_header_index (GstMpdClient *client, gchar **uri, guint stream_idx, gint64 * range_start, gint64 * range_end);
gboolean gst_mpd_client_is_live (GstMpdClient * client);
gboolean gst_mpd_client_stream_seek (GstMpdClient * client, GstActiveStream * stream, gboolean forward, GstSeekFlags flags, GstClockTime ts, GstClockTime * final_ts);
gboolean gst_mpd_client_seek_to_time (GstMpdClient * client, GDateTime * time);
GstClockTime gst_mpd_client_get_stream_presentation_offset (GstMpdClient *client, guint stream_idx);
gchar** gst_mpd_client_get_utc_timing_sources (GstMpdClient *client, guint methods, GstMPDUTCTimingType *selected_method);
GstClockTime gst_mpd_client_get_period_start_time (GstMpdClient *client);

/* Period selection */
guint gst_mpd_client_get_period_index_at_time (GstMpdClient * client, GstDateTime * time);
gboolean gst_mpd_client_set_period_index (GstMpdClient *client, guint period_idx);
gboolean gst_mpd_client_set_period_id (GstMpdClient *client, const gchar * period_id);
guint gst_mpd_client_get_period_index (GstMpdClient *client);
const gchar *gst_mpd_client_get_period_id (GstMpdClient *client);
gboolean gst_mpd_client_has_next_period (GstMpdClient *client);
gboolean gst_mpd_client_has_previous_period (GstMpdClient * client);

/* Representation selection */
gint gst_mpd_client_get_rep_idx_with_max_bandwidth (GList *Representations, gint64 max_bandwidth, gint max_video_width, gint max_video_height, gint max_video_framerate_n, gint max_video_framerate_d);
gint gst_mpd_client_get_rep_idx_with_min_bandwidth (GList * Representations);

GstDateTime *
gst_mpd_client_get_availability_start_time (GstMpdClient * client);

/* URL management */
const gchar *gst_mpd_client_get_baseURL (GstMpdClient *client, guint indexStream);
gchar *gst_mpd_client_parse_baseURL (GstMpdClient * client, GstActiveStream * stream, gchar ** query);

/* Active stream */
guint gst_mpd_client_get_nb_active_stream (GstMpdClient *client);
GstActiveStream *gst_mpd_client_get_active_stream_by_index (GstMpdClient *client, guint stream_idx);
gboolean gst_mpd_client_active_stream_contains_subtitles (GstActiveStream * stream);

/* AdaptationSet */
guint gst_mpd_client_get_nb_adaptationSet (GstMpdClient *client);
GList * gst_mpd_client_get_adaptation_sets (GstMpdClient * client);

/* Segment */
gboolean gst_mpd_client_has_next_segment (GstMpdClient * client, GstActiveStream * stream, gboolean forward);
GstFlowReturn gst_mpd_client_advance_segment (GstMpdClient * client, GstActiveStream * stream, gboolean forward);
void gst_mpd_client_seek_to_first_segment (GstMpdClient * client);
GstDateTime *gst_mpd_client_get_next_segment_availability_start_time (GstMpdClient * client, GstActiveStream * stream);

/* Get audio/video stream parameters (caps, width, height, rate, number of channels) */
GstCaps * gst_mpd_client_get_stream_caps (GstActiveStream * stream);
gboolean gst_mpd_client_get_bitstream_switching_flag (GstActiveStream * stream);
guint gst_mpd_client_get_video_stream_width (GstActiveStream * stream);
guint gst_mpd_client_get_video_stream_height (GstActiveStream * stream);
gboolean gst_mpd_client_get_video_stream_framerate (GstActiveStream * stream, gint * fps_num, gint * fps_den);
guint gst_mpd_client_get_audio_stream_rate (GstActiveStream * stream);
guint gst_mpd_client_get_audio_stream_num_channels (GstActiveStream * stream);

/* Support multi language */
guint gst_mpd_client_get_list_and_nb_of_audio_language (GstMpdClient *client, GList **lang);

gint64 gst_mpd_client_calculate_time_difference (const GstDateTime * t1, const GstDateTime * t2);
GstDateTime *gst_mpd_client_add_time_difference (GstDateTime * t1, gint64 usecs);
gint64 gst_mpd_client_parse_default_presentation_delay(GstMpdClient * client, const gchar * default_presentation_delay);

/* profiles */
gboolean gst_mpd_client_has_isoff_ondemand_profile (GstMpdClient *client);


G_END_DECLS

#endif /* __GST_MPDCLIENT_H__ */
