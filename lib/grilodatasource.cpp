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

#include "grilodatasource.h"
#include "grilodatasource_p.h"

#include "grilomodel.h"
#include "griloregistry.h"

GriloDataSource::GriloDataSource(GriloDataSourcePrivate *d, QObject *parent) :
  QObject(parent),
  d_ptr(d) {

  d_ptr->m_metadataKeys << Title;
  d_ptr->m_typeFilter << None;
}

GriloDataSource::~GriloDataSource() {
  d_ptr->m_models.clear();
  cancel();
  delete d_ptr; d_ptr = 0;
}

const QList<GriloMedia *> *GriloDataSource::media() const {
  return &d_ptr->m_media;
}

void GriloDataSource::addModel(GriloModel *model) {
  if (d_ptr->m_models.indexOf(model) == -1) {
    d_ptr->m_models << model;
  }
}

void GriloDataSource::removeModel(GriloModel *model) {
  if (int index = d_ptr->m_models.indexOf(model) != -1) {
    d_ptr->m_models.removeAt(index);
  }
}

GriloRegistry *GriloDataSource::registry() const {
  return d_ptr->m_registry;
}

int GriloDataSource::count() const {
  return d_ptr->m_count;
}

void GriloDataSource::setCount(int count) {
  if (d_ptr->m_count != count) {
    d_ptr->m_count = count;
    emit countChanged();
  }
}

int GriloDataSource::skip() const {
  return d_ptr->m_skip;
}

void GriloDataSource::setSkip(int skip) {
  if (d_ptr->m_skip != skip) {
    d_ptr->m_skip = skip;
    emit skipChanged();
  }
}

QVariantList GriloDataSource::metadataKeys() const {
  return d_ptr->m_metadataKeys;
}

void GriloDataSource::setMetadataKeys(const QVariantList& keys) {
  if (d_ptr->m_metadataKeys != keys) {
    d_ptr->m_metadataKeys = keys;
    emit metadataKeysChanged();
  }
}

QVariantList GriloDataSource::typeFilter() const {
  return d_ptr->m_typeFilter;
}

void GriloDataSource::setTypeFilter(const QVariantList& filter) {
  if (d_ptr->m_typeFilter != filter) {
    d_ptr->m_typeFilter = filter;
    emit typeFilterChanged();
  }
}

void GriloDataSource::cancel() {
  if (d_ptr->m_opId != 0) {
    grl_operation_cancel(d_ptr->m_opId);
    d_ptr->m_opId = 0;
  }

  d_ptr->clearMedia();
}
