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

#include "griloregistry.h"
#include <QDebug>

GriloRegistry::GriloRegistry(QDeclarativeItem *parent) :
  QDeclarativeItem(parent),
  m_registry(0) {

  grl_init(0, 0);
}

GriloRegistry::~GriloRegistry() {
  m_registry = 0;
}

void GriloRegistry::componentComplete() {
  m_registry = grl_registry_get_default();

  g_signal_connect(m_registry, "source-added", G_CALLBACK(grilo_source_added), this);
  g_signal_connect(m_registry, "source-removed", G_CALLBACK(grilo_source_removed), this);

  loadConfigurationFile();

  QDeclarativeItem::componentComplete();
}

QStringList GriloRegistry::availableSources() {
  return m_sources;
}

bool GriloRegistry::loadAll() {
  // TODO: error reporting
  return grl_registry_load_all_plugins(m_registry, NULL) == TRUE;
}

QString GriloRegistry::configurationFile() const {
  return m_configurationFile;
}

void GriloRegistry::setConfigurationFile(const QString& file) {
  if (m_configurationFile != file) {
    m_configurationFile = file;
    emit configurationFileChanged();

    if (isComponentComplete()) {
      loadConfigurationFile();
    }
  }
}

void GriloRegistry::loadConfigurationFile() {
  if (!m_configurationFile.isEmpty() && m_registry) {
    grl_registry_add_config_from_file(m_registry, m_configurationFile.toLocal8Bit().data(), NULL);
  }
}

void GriloRegistry::grilo_source_added(GrlRegistry *registry, GrlSource *src,
				       gpointer user_data) {

  Q_UNUSED(registry);

  GriloRegistry *reg = static_cast<GriloRegistry *>(user_data);

  const char *id = grl_source_get_id(src);

  if (reg->m_sources.indexOf(id) == -1) {
    reg->m_sources << id;
    emit reg->availableSourcesChanged();
  }
}

void GriloRegistry::grilo_source_removed(GrlRegistry *registry, GrlSource *src,
					 gpointer user_data) {

  Q_UNUSED(registry);

  GriloRegistry *reg = static_cast<GriloRegistry *>(user_data);

  const char *id = grl_source_get_id(src);

  if (int index = reg->m_sources.indexOf(id) != -1) {
    reg->m_sources.removeAt(index);

    emit reg->availableSourcesChanged();
  }
}

GrlSource *GriloRegistry::lookupSource(const QString& id) {
  if (m_registry) {
    return grl_registry_lookup_source(m_registry, id.toAscii().data());
  }

  return 0;
}
