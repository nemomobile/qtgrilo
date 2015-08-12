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

#ifndef GRILO_SINGLE_DATA_SOURCE_H
#define GRILO_SINGLE_DATA_SOURCE_H

#include <GriloQt>
#include <GriloDataSource>

#include <grilo.h>

class GriloMedia;

class GRILO_QT_EXPORT GriloSingleDataSource : public GriloDataSource {
  Q_OBJECT

  Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged);
  Q_PROPERTY(QVariantList supportedKeys READ supportedKeys NOTIFY supportedKeysChanged);
  Q_PROPERTY(QVariantList slowKeys READ slowKeys NOTIFY slowKeysChanged);
  Q_PROPERTY(bool available READ isAvailable NOTIFY availabilityChanged);

public:
  GriloSingleDataSource(QObject *parent = 0);
  virtual ~GriloSingleDataSource();

  QString source() const;
  void setSource(const QString& source);

  QVariantList supportedKeys() const;

  QVariantList slowKeys() const;

  bool isAvailable() const;

  Q_INVOKABLE GriloMedia* getMedia(const QUrl &url);
  Q_INVOKABLE bool requestMedia(const QUrl &url);

signals:
  void sourceChanged();
  void supportedKeysChanged();
  void slowKeysChanged();
  void availabilityChanged();
  void mediaReceived(GriloMedia *media);

protected:
  GrlSource* getSource() const;

  static void grilo_source_resolve_cb(GrlSource *source, guint operation_id,
                                      GrlMedia *media, gpointer user_data,
                                      const GError *error);

private:
  void availableSourcesChanged();

  QString m_source;

  QVariantList m_slowKeys;
  QVariantList m_supportedKeys;
  bool m_available;
};

#endif /* GRILO_SINGLE_DATA_SOURCE_H */
