/*!
 *
 * Copyright (C) 2014 Jolla Ltd.
 *
 * Contact: Mohammed Hassan <mohammed.hassan@jollamobile.com>
 * Authors: Mohammed Hassan <mohammed.hassan@jollamobile.com>,
 *          Andres Gomez <agomez@igalia.com>
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

#include "declarativegrilomodel.h"

#include <GriloMedia>

#if QT_VERSION_5
# include <QQmlEngine>
# define QDeclarativeEngine QQmlEngine
#else
# include <QDeclarativeEngine>
#endif

DeclarativeGriloModel::DeclarativeGriloModel(QObject *parent) :
  GriloModel(parent) {

}

DeclarativeGriloModel::~DeclarativeGriloModel() {

}

QObject *DeclarativeGriloModel::get(int rowIndex) const {
  QVariant mediaVariant = data(index(rowIndex), GriloModel::MediaRole);

  GriloMedia *media = mediaVariant.value<GriloMedia*>();
  if (media) {
    QDeclarativeEngine::setObjectOwnership(media, QDeclarativeEngine::CppOwnership);
  }

  return media;
}
