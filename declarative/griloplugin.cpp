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

#include "griloplugin.h"
#include "declarativegrilomodel.h"

#include <GriloQt>
#include <GriloDataSource>
#include <GriloBrowse>
#include <GriloMedia>
#include <GriloMultiSearch>
#include <GriloQuery>
#include <GriloRegistry>
#include <GriloSearch>
#include <GriloSingleDataSource>


#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
# include <qqml.h>
#else
# include <qdeclarative.h>
#endif

GriloPlugin::GriloPlugin(QObject *parent) :
  QDeclarativeExtensionPlugin(parent) {

}

GriloPlugin::~GriloPlugin() {

}

void GriloPlugin::registerTypes(const char *uri) {
  qmlRegisterType<GriloRegistry>(uri, 0, 1, "GriloRegistry");
  qmlRegisterType<DeclarativeGriloModel>(uri, 0, 1, "GriloModel");
  qmlRegisterType<GriloBrowse>(uri, 0, 1, "GriloBrowse");
  qmlRegisterType<GriloSearch>(uri, 0, 1, "GriloSearch");
  qmlRegisterType<GriloQuery>(uri, 0, 1, "GriloQuery");
  // TODO: Symbol error when used :(
  //  qmlRegisterType<GriloMultiSearch>(uri, 0, 1, "GriloMultiSearch");
  qmlRegisterInterface<GriloSingleDataSource>("GriloSingleDataSource");
  qmlRegisterUncreatableType<GriloDataSource>(uri, 0, 1, "GriloDataSource", "GriloDataSource is base class providing certain enums for its derived classes");
  qmlRegisterUncreatableType<GriloMedia>(uri, 0, 1, "GriloMedia", "GriloMedia can only be obtained from other classes");
}

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(qmlgriloplugin, GriloPlugin);
#endif
