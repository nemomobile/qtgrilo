/*!
 *
 * Copyright (C) 2012-2013 Jolla Ltd.
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

#include "grilomodel.h"
#include <QDebug>
#include "grilomedia.h"
#include "grilodatasource.h"

GriloModel::GriloModel(QObject *parent) :
  QAbstractListModel(parent) {

  QHash<int, QByteArray> roles;
  roles[MediaRole] = "media";

  setRoleNames(roles);
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
    return QVariant::fromValue(get(index.row()));
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

QObject *GriloModel::get(int index) const {
  if (index < 0 || index >= rowCount()) {
    return 0;
  }

  return m_source->media()->at(index);
}

int GriloModel::count() const {
  return rowCount();
}
