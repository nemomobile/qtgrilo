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

#ifndef GRILO_REGISTRY_H
#define GRILO_REGISTRY_H

#include <QObject>
#include <QStringList>

class GriloRegistryPrivate;

class GriloRegistry : public QObject {
  Q_OBJECT

  Q_PROPERTY(QStringList availableSources READ availableSources NOTIFY availableSourcesChanged);

  friend class GriloRegistryPrivate;

public:
  GriloRegistry(QObject *parent = 0);
  ~GriloRegistry();

  QStringList availableSources();

  bool loadConfigurationFile(const QString& configurationFile);
  bool loadAllPlugins();

signals:
  void availableSourcesChanged();
  void configurationFileChanged();

private:
  GriloRegistryPrivate *d_ptr;
};

#endif /* GRILO_REGISTRY_H */
