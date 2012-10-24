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
#include "grilomedia.h"
#include "grilomedia_p.h"
#include <QDebug>

class GriloDataSourcePrivate {
public:
  enum OperationType {
    Browse = GRL_OP_BROWSE,
    Search = GRL_OP_SEARCH,
  };

  GriloDataSourcePrivate(GriloDataSource *q, GriloRegistry *registry) :
    m_registry(registry),
    m_opId(0),
    m_count(0),
    m_skip(0),
    q_ptr(q) {
  }

  void prefill(GriloModel *model) {
    if (m_media.isEmpty()) {
      return;
    }

    model->beginInsertRows(QModelIndex(), 0, m_media.size() - 1);
    model->endInsertRows();
  }

  GList *keysAsList() {
    // TODO: Check why using  grl_metadata_key_list_new() produces a symbol error.
    GList *keys = NULL;

    foreach (const QVariant& var, m_metadataKeys) {
      if (var.canConvert<int>()) {
	keys = g_list_append(keys, GRLKEYID_TO_POINTER(var.toInt()));
      }
    }

    return keys;
  }

  GrlOperationOptions *operationOptions(GrlSource *src, const OperationType& type) {
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

  static void fill_key_id(gpointer data, gpointer user_data) {
    QVariantList *varList = static_cast<QVariantList *>(user_data);
    varList->append(GriloDataSource::MetadataKeys(GRLPOINTER_TO_KEYID(data)));
  }

  QVariantList listToVariantList(const GList *keys) const {
    QVariantList varList;

    g_list_foreach(const_cast<GList *>(keys), fill_key_id, &varList);

    return varList;
  }

  void clearMedia() {
    int size = m_media.size();

    foreach (GriloModel *model, m_models) {
      model->beginRemoveRows(QModelIndex(), 0, size - 1);
    }

    qDeleteAll(m_media);
    m_media.clear();

    emit q_ptr->mediaCleared();

    foreach (GriloModel *model, m_models) {
      model->endRemoveRows();
    }
  }

  void addMedia(GrlMedia *media) {
    GriloMedia *m = new GriloMedia(new GriloMediaPrivate(media));

    int size = m_media.size();

    foreach (GriloModel *model, m_models) {
      model->beginInsertRows(QModelIndex(), size, size);
    }

    m_media << m;

    emit q_ptr->mediaAdded(m);

    foreach (GriloModel *model, m_models) {
      model->endInsertRows();
    }
  }

  static void grilo_source_result_cb(GrlSource *source, guint op_id,
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

    GriloDataSourcePrivate *that = static_cast<GriloDataSourcePrivate *>(user_data);
    if (that->m_opId != op_id) {
      qWarning() << "Got results belonging to an unknown browse id";

      if (media) {
	g_object_unref(media);
      }

      return;
    }

    if (media) {
      that->addMedia(media);
    }

    if (remaining == 0) {
      that->m_opId = 0;
    }
  }

  GriloRegistry *m_registry;
  guint m_opId;
  int m_count;
  int m_skip;

  QVariantList m_metadataKeys;
  QVariantList m_typeFilter;

  QList<GriloMedia *> m_media;
  QList<GriloModel *> m_models;

  GriloDataSource *q_ptr;
};

#endif /* GRILO_DATA_SOURCE_P_H */
