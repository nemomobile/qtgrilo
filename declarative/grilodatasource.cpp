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
#include "grilomedia.h"
#include "grilomodel.h"
#include "griloregistry.h"
#include <QDebug>
#include <grilo.h>

static void fill_key_id(gpointer data, gpointer user_data) {
  QVariantList *varList = static_cast<QVariantList *>(user_data);
  varList->append(GriloDataSource::MetadataKeys(GRLPOINTER_TO_KEYID(data)));
}

GriloDataSource::GriloDataSource(QDeclarativeItem *parent) :
  QDeclarativeItem(parent),
  m_opId(0),
  m_registry(0),
  m_count(0),
  m_skip(0) {

  m_metadataKeys << Title;
  m_typeFilter << None;
}

GriloDataSource::~GriloDataSource() {
  m_models.clear();
  cancelRefresh();
}

const QList<GriloMedia *> *GriloDataSource::media() const {
  return &m_media;
}

void GriloDataSource::addModel(GriloModel *model) {
  if (m_models.indexOf(model) == -1) {
    m_models << model;
  }
}

void GriloDataSource::removeModel(GriloModel *model) {
  if (int index = m_models.indexOf(model) != -1) {
    m_models.removeAt(index);
  }
}

void GriloDataSource::prefill(GriloModel *model) {
  if (m_media.isEmpty()) {
    return;
  }

  model->beginInsertRows(QModelIndex(), 0, m_media.size() - 1);
  model->endInsertRows();
}

void GriloDataSource::addMedia(GriloMedia *media) {
  int size = m_media.size();

  foreach (GriloModel *model, m_models) {
    model->beginInsertRows(QModelIndex(), size, size);
  }

  m_media << media;

  foreach (GriloModel *model, m_models) {
    model->endInsertRows();
  }
}

void GriloDataSource::clearMedia() {
  int size = m_media.size();

  foreach (GriloModel *model, m_models) {
    model->beginRemoveRows(QModelIndex(), 0, size - 1);
  }

  qDeleteAll(m_media);
  m_media.clear();

  foreach (GriloModel *model, m_models) {
    model->endRemoveRows();
  }
}

GriloRegistry *GriloDataSource::registry() const {
  return m_registry;
}

void GriloDataSource::setRegistry(GriloRegistry *registry) {
  // Registry change is not allowed for now.

  if (!m_registry && registry != m_registry) {

    m_registry = registry;

    QObject::connect(m_registry, SIGNAL(availableSourcesChanged()),
		     this, SLOT(availableSourcesChanged()));

    emit registryChanged();
  }
}

int GriloDataSource::count() const {
  return m_count;
}

void GriloDataSource::setCount(int count) {
  if (m_count != count) {
    m_count = count;
    emit countChanged();
  }
}

int GriloDataSource::skip() const {
  return m_skip;
}

void GriloDataSource::setSkip(int skip) {
  if (m_skip != skip) {
    m_skip = skip;
    emit skipChanged();
  }
}

QVariantList GriloDataSource::metadataKeys() const {
  return m_metadataKeys;
}

void GriloDataSource::setMetadataKeys(const QVariantList& keys) {
  if (m_metadataKeys != keys) {
    m_metadataKeys = keys;
    emit metadataKeysChanged();
  }
}

QVariantList GriloDataSource::typeFilter() const {
  return m_typeFilter;
}

void GriloDataSource::setTypeFilter(const QVariantList& filter) {
  if (m_typeFilter != filter) {
    m_typeFilter = filter;
    emit typeFilterChanged();
  }
}

GrlOperationOptions *GriloDataSource::operationOptions(GrlSource *src, const OperationType& type) {
  GrlCaps *caps = NULL;

  if (src) {
    caps = grl_source_get_caps(src, (GrlSupportedOps)type);
  }

  GrlOperationOptions *options = grl_operation_options_new(caps);

  grl_operation_options_set_flags(options, GRL_RESOLVE_IDLE_RELAY); // TODO: hardcoded
  grl_operation_options_set_skip(options, m_skip);

  if (m_count != 0) {
    grl_operation_options_set_count(options, m_count);
  }

  int typeFilter = 0;
  foreach (const QVariant& var, m_typeFilter) {
    if (var.canConvert<int>()) {
      typeFilter |= var.toInt();
    }
  }

  grl_operation_options_set_type_filter(options, (GrlTypeFilter)typeFilter);

  return options;
}

GList *GriloDataSource::keysAsList() {
  // TODO: Check why using  grl_metadata_key_list_new() produces a symbol error.
  GList *keys = NULL;

  foreach (const QVariant& var, m_metadataKeys) {
    if (var.canConvert<int>()) {
      keys = g_list_append(keys, GRLKEYID_TO_POINTER(var.toInt()));
    }
  }

  return keys;
}

void GriloDataSource::cancelRefresh() {
  if (m_opId != 0) {
    grl_operation_cancel(m_opId);
    m_opId = 0;
  }

  clearMedia();
}

void GriloDataSource::grilo_source_result_cb(GrlSource *source, guint op_id,
					     GrlMedia *media, guint remaining,
					     gpointer user_data, const GError *error) {

  Q_UNUSED(source);

  // We get an error if the operation has been canceled:
  if (error) {
    if (error->domain != GRL_CORE_ERROR || error->code != GRL_CORE_ERROR_OPERATION_CANCELLED) {
      // TODO: error reporting?
      qCritical() << "Operation failed" << error->message;
    }
  }

  GriloDataSource *that = static_cast<GriloDataSource *>(user_data);

  if (that->m_opId != op_id) {
    qWarning() << "Got results belonging to an unknown browse id";

    if (media) {
      g_object_unref(media);
    }

    return;
  }

  if (media) {
    that->addMedia(new GriloMedia(media));
  }

  if (remaining == 0) {
    that->m_opId = 0;
  }
}

QVariantList GriloDataSource::listToVariantList(const GList *keys) const {
  QVariantList varList;

  g_list_foreach(const_cast<GList *>(keys), fill_key_id, &varList);

  return varList;
}
