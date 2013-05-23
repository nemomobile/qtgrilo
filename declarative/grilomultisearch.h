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

#ifndef GRILO_MULTI_SEARCH_H
#define GRILO_MULTI_SEARCH_H

#include "grilodatasource.h"
#include <QStringList>

class GriloMultiSearch : public GriloDataSource {
  Q_OBJECT

  Q_PROPERTY(QStringList sources READ sources WRITE setSources NOTIFY sourcesChanged);
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged);

public:
  GriloMultiSearch(QObject *parent = 0);
  ~GriloMultiSearch();

  bool refresh();

  QStringList sources() const;
  void setSources(const QStringList& sources);

  QString text() const;
  void setText(const QString& text);

signals:
  void sourcesChanged();
  void textChanged();

private:
  QStringList m_sources;
  QString m_text;
};

#endif /* GRILO_MULTI_SEARCH_H */
