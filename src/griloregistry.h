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

#include <GriloQt>

#include <QObject>
#include <grilo.h>

#if QT_VERSION_5
# include <QQmlParserStatus>
# define QDeclarativeParserStatus QQmlParserStatus
#else
# include <QDeclarativeParserStatus>
#endif

#include <QStringList>

class GRILO_QT_EXPORT GriloRegistry : public QObject, public QDeclarativeParserStatus {
  Q_OBJECT

  Q_PROPERTY(QStringList availableSources READ availableSources NOTIFY availableSourcesChanged);
  Q_PROPERTY(QString configurationFile READ configurationFile WRITE setConfigurationFile NOTIFY configurationFileChanged);
  Q_INTERFACES(QDeclarativeParserStatus)
public:
  GriloRegistry(QObject *parent = 0);
  ~GriloRegistry();

  void classBegin();
  virtual void componentComplete();
  QStringList availableSources();

  Q_INVOKABLE bool loadAll();

  Q_INVOKABLE bool loadPluginById(const QString& pluginId);

  GrlSource *lookupSource(const QString& id);

  QString configurationFile() const;
  void setConfigurationFile(const QString& file);

signals:
  void availableSourcesChanged();
  void configurationFileChanged();
  void contentChanged(const QString &source, GrlSourceChangeType change_type,
                      GPtrArray *changed_media);

private:
  static void connect_source(gpointer data, gpointer user_data);
  static void grilo_source_added(GrlRegistry *registry, GrlSource *src, gpointer user_data);
  static void grilo_source_removed(GrlRegistry *registry, GrlSource *src, gpointer user_data);
  static void grilo_content_changed_cb(GrlSource *source, GPtrArray *changed_media,
                     GrlSourceChangeType change_type, gboolean location_unknown,
                     gpointer data);

  void loadConfigurationFile();

  GrlRegistry *m_registry;
  QStringList m_sources;
  QString m_configurationFile;
};

#endif /* GRILO_REGISTRY_H */
