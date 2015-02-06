/*!
 *
 * Copyright (C) 2012-2014 Jolla Ltd.
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

#include "grilomodel.h"
#include <QDebug>
#include "grilomedia.h"
#include "grilodatasource.h"

GriloModel::GriloModel(QObject *parent) :
  QAbstractListModel(parent),
  m_source(0) {

  QHash<int, QByteArray> roles;
  roles[MediaRole] = "media";

  setRoleNames(roles);

  QObject::connect(this, SIGNAL(rowsInserted(const QModelIndex&, int, int)),
                   this, SIGNAL(countChanged()));
  QObject::connect(this, SIGNAL(rowsRemoved(const QModelIndex&, int, int)),
                   this, SIGNAL(countChanged()));
}

GriloModel::~GriloModel() {
  setSource(0);
}

int GriloModel::rowCount(const QModelIndex& parent) const {
  if (!parent.isValid()) {
    return m_source ? m_source->media()->count() : 0;
  }

  return 0;
}

QVariant GriloModel::data(const QModelIndex& index, int role) const {
  if (index.row() < 0 || index.row() >= rowCount()) {
    return QVariant();
  }

  switch (role) {
  case MediaRole:
    return QVariant::fromValue(m_source->media()->at(index.row()));
  }

  return QVariant();
}

GriloDataSource *GriloModel::source() const {
  return m_source;
}

void GriloModel::setSource(GriloDataSource *source) {
  if (m_source == source) {
    return;
  }

  beginResetModel();

  if (m_source) {
    m_source->removeModel(this);
  }

  m_source = source;

  if (m_source) {
    m_source->addModel(this);
  }

  endResetModel();

  emit sourceChanged();

  if (m_source) {
    m_source->prefill(this);
  }
}

int GriloModel::count() const {
  return rowCount();
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
QHash<int, QByteArray> GriloModel::roleNames() const {
  return m_roleNames;
}

void GriloModel::setRoleNames(const QHash<int, QByteArray> &roles) {
  m_roleNames = roles;
}
#endif
