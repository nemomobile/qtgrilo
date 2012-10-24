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

#ifndef GRILO_DATA_SOURCE_P_H
#define GRILO_DATA_SOURCE_P_H

#include "grilomodel.h"

class GriloDataSourcePrivate {
public:
  GriloDataSourcePrivate(GriloRegistry *registry) :
  m_registry(registry),
  m_opId(0),
  m_count(0),
  m_skip(0) {

  }

  void prefill(GriloModel *model) {
    if (m_media.isEmpty()) {
      return;
    }

    model->beginInsertRows(QModelIndex(), 0, m_media.size() - 1);
    model->endInsertRows();
  }

  GriloRegistry *m_registry;
  guint m_opId;
  int m_count;
  int m_skip;

  QList<GriloMedia *> m_media;
  QList<GriloModel *> m_models;
};

#endif /* GRILO_DATA_SOURCE_P_H */
