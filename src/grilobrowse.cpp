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

#include "grilobrowse.h"
#include <QDebug>
#include "grilomedia.h"

GriloBrowse::GriloBrowse(QObject *parent) :
  GriloSingleDataSource(parent),
  m_media(0) {

}

GriloBrowse::~GriloBrowse() {
  setBaseMedia(QString());
}

bool GriloBrowse::refresh() {
  cancelRefresh();

  GrlSource *src = getSource();

  if (!src) {
    qWarning() << "Failed to get source" << source();
    return false;
  }

  GList *keys = keysAsList();
  GrlOperationOptions *options = operationOptions(src, Browse);

  m_opId = grl_source_browse(src, rootMedia(),
			     keys, options, grilo_source_result_cb, this);

  g_object_unref(options);
  g_list_free(keys);

  return m_opId != 0;
}

QString GriloBrowse::baseMedia() const {
  return m_baseMedia;
}

void GriloBrowse::setBaseMedia(const QString& media){
  if (m_baseMedia == media) {
    return;
  }

  if (m_media) {
    delete m_media;
    m_media = 0;
  }

  m_baseMedia = media;

  emit baseMediaChanged();
}

GrlMedia *GriloBrowse::rootMedia() {
  if (m_media) {
    return m_media->media();
  }
  else if (m_baseMedia.isEmpty()) {
    return NULL;
  }

  GrlMedia *m = grl_media_unserialize(m_baseMedia.toUtf8().constData());
  if (m) {
    m_media = new GriloMedia(m);
    return m_media->media();
  }
  else {
    qDebug() << "Failed to create GrlMedia from" << m_baseMedia;
  }

  return NULL;
}
