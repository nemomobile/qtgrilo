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

#ifndef GRILO_REGISTRY_P_H
#define GRILO_REGISTRY_P_H

#include "griloregistry.h"
#include "griloregistry_p.h"
#include <grilo.h>

class GriloRegistryPrivate {
public:
  GriloRegistryPrivate(GriloRegistry *q) :
    q_ptr(q),
    m_registry(grl_registry_get_default()) {

    if (m_registry) {
      g_signal_connect(m_registry, "source-added",
		       G_CALLBACK(grilo_source_added), this);
      g_signal_connect(m_registry, "source-removed",
		       G_CALLBACK(grilo_source_removed), this);
    }
  }

  static void grilo_source_added(GrlRegistry *registry, GrlSource *src, gpointer user_data) {
    Q_UNUSED(registry);

    GriloRegistryPrivate *d = static_cast<GriloRegistryPrivate *>(user_data);

    const char *id = grl_source_get_id(src);

    if (d->m_sources.indexOf(id) == -1) {
      d->m_sources << id;
      emit d->q_ptr->availableSourcesChanged();
    }
  }

  static void grilo_source_removed(GrlRegistry *registry, GrlSource *src, gpointer user_data) {
    Q_UNUSED(registry);

    GriloRegistryPrivate *d = static_cast<GriloRegistryPrivate *>(user_data);

    const char *id = grl_source_get_id(src);

    if (int index = d->m_sources.indexOf(id) != -1) {
      d->m_sources.removeAt(index);

      emit d->q_ptr->availableSourcesChanged();
    }
  }

  GrlSource *lookupSource(const QString& id) {
    if (m_registry) {
      return grl_registry_lookup_source(m_registry, id.toAscii().data());
    }

    return 0;

  }

  QStringList m_sources;

  GriloRegistry *q_ptr;
  GrlRegistry *m_registry;
};

#endif /* GRILO_REGISTRY_P_H */
