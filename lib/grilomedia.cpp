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
#include "grilomedia_p.h"
#include <QDebug>

GriloMedia::GriloMedia(GriloMediaPrivate *d, QObject *parent) :
  QObject(parent), d_ptr(d) {

}

GriloMedia::~GriloMedia() {
  delete d_ptr; d_ptr = 0;
}

QString GriloMedia::serialize() {
  QString result;
  gchar *str = grl_media_serialize_extended(d_ptr->m_media, GRL_MEDIA_SERIALIZE_FULL, NULL);

  if (str) {
    result = QString::fromUtf8(str);
    g_free(str);
  }

  return result;
}

QString GriloMedia::id() const {
  return grl_media_get_id(d_ptr->m_media);
}

QString GriloMedia::title() const {
  return grl_media_get_title(d_ptr->m_media);
}

QUrl GriloMedia::url() const {
  QUrl url = QUrl::fromEncoded(QByteArray(grl_media_get_url(d_ptr->m_media)));

  return url;
}

int GriloMedia::duration() const {
  return grl_media_get_duration(d_ptr->m_media);
}

bool GriloMedia::isContainer() const {
  return GRL_IS_MEDIA_BOX(d_ptr->m_media) == TRUE;
}

QString GriloMedia::author() const {
  return grl_media_get_author(d_ptr->m_media);
}
