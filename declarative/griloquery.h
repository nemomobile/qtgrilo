// -*- c++ -*-

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

#ifndef GRILO_QUERY_H
#define GRILO_QUERY_H

#include "grilodatasource.h"

class GriloQuery : public GriloDataSource {
  Q_OBJECT

  Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged);
  Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged);
  Q_PROPERTY(QVariantList supportedKeys READ supportedKeys NOTIFY supportedKeysChanged);
  Q_PROPERTY(QVariantList slowKeys READ slowKeys NOTIFY slowKeysChanged);
  Q_PROPERTY(bool available READ isAvailable NOTIFY availabilityChanged);

public:
  GriloQuery(QObject *parent = 0);
  ~GriloQuery();

  bool refresh();

  QString source() const;
  void setSource(const QString& source);

  QString query() const;
  void setQuery(const QString& query);

  QVariantList supportedKeys() const;
  QVariantList slowKeys() const;

  bool isAvailable() const;

signals:
  void sourceChanged();
  void queryChanged();
  void supportedKeysChanged();
  void slowKeysChanged();
  void availabilityChanged();

private:
  void availableSourcesChanged();

  QString m_source;
  QString m_query;

  QVariantList m_slowKeys;
  QVariantList m_supportedKeys;
  bool m_available;
};

#endif /* GRILO_QUERY_H */
