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
#include "griloregistry_p.h"
#include <QDebug>

GriloRegistry::GriloRegistry(QObject *parent) :
  QObject(parent),
  d_ptr(new GriloRegistryPrivate(this)) {

  grl_init(0, 0);
}

GriloRegistry::~GriloRegistry() {
  delete d_ptr; d_ptr = 0;
}

bool GriloRegistry::loadConfigurationFile(const QString& configurationFile) {
  if (!d_ptr->m_registry) {
    return false;
  }

  // TODO: proper error reporting
  return grl_registry_add_config_from_file(d_ptr->m_registry,
				    configurationFile.toLocal8Bit().data(), NULL) == TRUE;
}

bool GriloRegistry::loadAllPlugins() {
  // TODO: error reporting
  return grl_registry_load_all_plugins(d_ptr->m_registry, NULL) == TRUE;
}

QStringList GriloRegistry::availableSources() {
  return d_ptr->m_sources;
}
