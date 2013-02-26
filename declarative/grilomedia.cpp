/*!
 *
 * Copyright (C) 2012 Jolla Ltd.
 *
 * Contact: Mohammed Hassan <mohammed.hassan@jollamobile.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "grilomedia.h"
#include <QDebug>

GriloMedia::GriloMedia(GrlMedia *media, QObject *parent) :
  QObject(parent), m_media(media) {

}

GriloMedia::~GriloMedia() {
  g_object_unref(m_media);
  m_media = 0;
}

GrlMedia *GriloMedia::media() {
  return m_media;
}

QString GriloMedia::serialize() {
  QString result;
  gchar *str = grl_media_serialize_extended(m_media, GRL_MEDIA_SERIALIZE_FULL, NULL);

  if (str) {
    result = QString::fromUtf8(str);
    g_free(str);
  }

  return result;
}

QString GriloMedia::id() const {
  return QString::fromUtf8(grl_media_get_id(m_media));
}

QString GriloMedia::title() const {
  return QString::fromUtf8(grl_media_get_title(m_media));
}

QUrl GriloMedia::url() const {
  QUrl url = QUrl::fromEncoded(QByteArray(grl_media_get_url(m_media)));

  return url;
}

int GriloMedia::duration() const {
  return grl_media_get_duration(m_media);
}

bool GriloMedia::isContainer() const {
  return GRL_IS_MEDIA_BOX(m_media) == TRUE;
}

QString GriloMedia::author() const {
  return QString::fromUtf8(grl_media_get_author(m_media));
}

QString GriloMedia::album() const {
  if (GRL_IS_MEDIA_AUDIO(m_media)) {
    return QString::fromUtf8(grl_media_audio_get_album(GRL_MEDIA_AUDIO(m_media)));
  }

  return QString();
}

QString GriloMedia::artist() const
{
  if (GRL_IS_MEDIA_AUDIO(m_media)) {
    return QString::fromUtf8(grl_media_audio_get_artist(GRL_MEDIA_AUDIO(m_media)));
  }

  return QString();
}

int GriloMedia::childCount() const
{
  if (GRL_IS_MEDIA_BOX(m_media)) {
    return grl_media_box_get_childcount(GRL_MEDIA_BOX(m_media));
  }

  return GRL_METADATA_KEY_CHILDCOUNT_UNKNOWN;
}

QString GriloMedia::mimeType() const
{
  return QString::fromUtf8(grl_media_get_mime(m_media));
}
