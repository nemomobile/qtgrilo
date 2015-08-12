// -*- c++ -*-

/*!
 *
 * Copyright (C) 2014 Jolla Ltd.
 *
 * Contact: Mohammed Hassan <mohammed.hassan@jollamobile.com>
 * Authors: Mohammed Hassan <mohammed.hassan@jollamobile.com>,
 *          Andres Gomez <agomez@igalia.com>
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

#include "grilosingledatasource.h"
#include "griloregistry.h"
#include "grilomedia.h"

#include <QDebug>

// #if QT_VERSION_5
// # include <QQmlEngine>
// # define QDeclarativeEngine QQmlEngine
// #else
// # include <QDeclarativeEngine>
// #endif

GriloSingleDataSource::GriloSingleDataSource(QObject *parent) :
  GriloDataSource(parent),
  m_available(false) {
}

GriloSingleDataSource::~GriloSingleDataSource() {

}

GrlSource* GriloSingleDataSource::getSource() const {
  if (!m_registry) {
    qWarning() << "GriloRegistry is not set";
    return 0;
  }

  if (m_source.isEmpty()) {
    qWarning() << "Source's id is not set";
    return 0;
  }

  return m_registry->lookupSource(m_source);
}

QString GriloSingleDataSource::source() const {
  return m_source;
}

void GriloSingleDataSource::setSource(const QString& source) {
  if (m_source != source) {
    m_source = source;
    emit sourceChanged();
    emit slowKeysChanged();
    emit supportedKeysChanged();
  }
}

QVariantList GriloSingleDataSource::supportedKeys() const {
  if (m_source.isEmpty() || !m_registry) {
    return QVariantList();
  }

  GrlSource *src = m_registry->lookupSource(m_source);
  if (src) {
    return listToVariantList(grl_source_supported_keys(src));
  }

  return QVariantList();
}

QVariantList GriloSingleDataSource::slowKeys() const {
  if (m_source.isEmpty() || !m_registry) {
    return QVariantList();
  }

  GrlSource *src = m_registry->lookupSource(m_source);
  if (src) {
    return listToVariantList(grl_source_slow_keys(src));
  }

  return QVariantList();
}

bool GriloSingleDataSource::isAvailable() const {
  return m_registry && !m_source.isEmpty() &&
    m_registry->availableSources().indexOf(m_source) != -1;
}

GriloMedia* GriloSingleDataSource::getMedia(const QUrl &url) {
  GrlSource *src = getSource();

  if (!src) {
    qWarning() << "Failed to get source" << source();
    return 0;
  }

  QByteArray encodedUrl = url.toEncoded();
  if (!grl_source_test_media_from_uri(src, encodedUrl.constData())) {
    qWarning() << "Failed to get media with url: " << encodedUrl;
    return 0;
  }

  GList *keys = keysAsList();
  GrlOperationOptions *options = operationOptions(src, MediaFromUri);
  GError *error = NULL;
  GrlMedia *media = grl_source_get_media_from_uri_sync (src,
                                                        encodedUrl.constData(),
                                                        keys,
                                                        options,
                                                        &error);

  g_object_unref(options);
  g_list_free(keys);

  if (error) {
    if (error->domain != GRL_CORE_ERROR) {
      qCritical() << "Operation failed" << error->message;
    }
    g_error_free(error);
  }

  if (media) {
    addMedia(media);
  }

  return new GriloMedia(media);
}

bool GriloSingleDataSource::requestMedia(const QUrl &url) {
  GrlSource *src = getSource();

  if (!src) {
    qWarning() << "Failed to get source" << source();
    return false;
  }

  QByteArray encodedUrl = url.toEncoded();
  if (!grl_source_test_media_from_uri(src, encodedUrl.constData())) {
    qWarning() << "Failed to get media with url: " << encodedUrl;
    return false;
  }

  GList *keys = keysAsList();
  GrlOperationOptions *options = operationOptions(src, MediaFromUri);

  m_opId = grl_source_get_media_from_uri (src,
                                          encodedUrl.constData(),
                                          keys,
                                          options,
                                          grilo_source_resolve_cb,
                                          this);

  g_object_unref(options);
  g_list_free(keys);

  return m_opId != 0;
}

void GriloSingleDataSource::grilo_source_resolve_cb(GrlSource *source, guint op_id,
                                                    GrlMedia *media, gpointer user_data,
                                                    const GError *error) {
  Q_UNUSED(source);

  // We get an error if the operation has been canceled:
  if (error) {
    if (error->domain != GRL_CORE_ERROR || error->code != GRL_CORE_ERROR_OPERATION_CANCELLED) {
      // TODO: error reporting?
      qCritical() << "Operation failed" << error->message;
    } else {
      // Cancelled operation notification. Nothing else to be done.
      return;
    }
  }

  GriloSingleDataSource *that = static_cast<GriloSingleDataSource *>(user_data);

  if (that->m_opId != op_id) {
    qWarning() << "Got Op Id result" << op_id
               << "but Op Id" << that->m_opId << "was expected.";

    if (media) {
      g_object_unref(media);
    }

    return;
  }

  if (media) {
    that->addMedia(media);
    GriloMedia *wrappedMedia = that->m_hash.value(QString::fromUtf8(grl_media_get_id(media)), 0);

    // FIXME!!!!!

    // QDeclarativeEngine::setObjectOwnership(wrappedMedia, QDeclarativeEngine::CppOwnership);
    emit that->mediaReceived(wrappedMedia);
  }
}

void GriloSingleDataSource::availableSourcesChanged() {
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
