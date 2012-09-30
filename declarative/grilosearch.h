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

#ifndef GRILO_SEARCH_H
#define GRILO_SEARCH_H

#include "grilodatasource.h"

class GriloSearch : public GriloDataSource {
  Q_OBJECT

  Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged);
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged);
  Q_PROPERTY(QVariantList supportedKeys READ supportedKeys NOTIFY supportedKeysChanged);
  Q_PROPERTY(QVariantList slowKeys READ slowKeys NOTIFY slowKeysChanged);
  Q_PROPERTY(bool available READ isAvailable NOTIFY availabilityChanged);

public:
  GriloSearch(QDeclarativeItem *parent = 0);
  ~GriloSearch();

  bool refresh();

  QString source() const;
  void setSource(const QString& source);

  QString text() const;
  void setText(const QString& text);

  QVariantList supportedKeys() const;
  QVariantList slowKeys() const;

  bool isAvailable() const;

signals:
  void sourceChanged();
  void textChanged();
  void supportedKeysChanged();
  void slowKeysChanged();
  void availabilityChanged();

private:
  void availableSourcesChanged();

  QString m_source;
  QString m_text;

  QVariantList m_slowKeys;
  QVariantList m_supportedKeys;
  bool m_available;
};

#endif /* GRILO_SEARCH_H */
