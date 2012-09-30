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

#ifndef GRILO_MODEL_H
#define GRILO_MODEL_H

#include <QAbstractListModel>

class GriloMedia;
class GriloDataSource;

class GriloModel : public QAbstractListModel {
  Q_OBJECT

  Q_PROPERTY(GriloDataSource* source READ source WRITE setSource NOTIFY sourceChanged);

  friend class GriloDataSource;

public:
  enum {
    TitleRole = Qt::UserRole + 1,
    IdRole,
    UrlRole,
    ContainerRole,
  };

  GriloModel(QObject *parent = 0);
  ~GriloModel();

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  GriloDataSource *source() const;
  void setSource(GriloDataSource *source);

signals:
  void sourceChanged();

private:
  GriloDataSource *m_source;
};

#endif /* GRILO_MODEL_H */
