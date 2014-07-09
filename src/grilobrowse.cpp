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
#include "griloregistry.h"
#include "grilomedia.h"

GriloBrowse::GriloBrowse(QObject *parent) :
  GriloDataSource(parent),
  m_media(0),
  m_available(false) {

}

GriloBrowse::~GriloBrowse() {
  setBaseMedia(QString());
}

bool GriloBrowse::refresh() {
  cancelRefresh();

  if (!m_registry) {
    qWarning() << "GriloRegistry not set";
    return false;
  }

  if (m_source.isEmpty()) {
    qWarning() << "source id not set";
    return false;
  }

  GrlSource *src = m_registry->lookupSource(m_source);

  if (!src) {
    qWarning() << "Failed to get source" << m_source;
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

QString GriloBrowse::source() const {
  return m_source;
}

void GriloBrowse::setSource(const QString& source) {
  if (m_source != source) {
    m_source = source;
    emit sourceChanged();
    emit slowKeysChanged();
    emit supportedKeysChanged();
  }
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

QVariantList GriloBrowse::supportedKeys() const {
  if (m_source.isEmpty() || !m_registry) {
    return QVariantList();
  }

  GrlSource *src = m_registry->lookupSource(m_source);
  if (src) {
    return listToVariantList(grl_source_supported_keys(src));
  }

  return QVariantList();
}

QVariantList GriloBrowse::slowKeys() const {
  if (m_source.isEmpty() || !m_registry) {
    return QVariantList();
  }

  GrlSource *src = m_registry->lookupSource(m_source);
  if (src) {
    return listToVariantList(grl_source_slow_keys(src));
  }

  return QVariantList();
}

bool GriloBrowse::isAvailable() const {
  return m_registry && !m_source.isEmpty() &&
    m_registry->availableSources().indexOf(m_source) != -1;
}

void GriloBrowse::availableSourcesChanged() {
  bool available = isAvailable();

  if (m_available != available) {
    m_available = available;

    emit availabilityChanged();
  }

  if (!m_available && m_opId) {
    // A source has disappeared while an operation is already running.
    // Most grilo will crash soon but we will just reset the opId
    m_opId = 0;
  }
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
