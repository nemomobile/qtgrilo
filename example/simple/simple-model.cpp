// -*- c++ -*-

/*!
 *
 * Copyright (C) 2012 Jolla Ltd.
 *
 * Contact: Mohammed Hassan <mohammed.hassan@jollamobile.com>
 * Author: Andres Gomez <agomez@igalia.com>
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

#include "simple-model.h"

#include <GriloRegistry>
#include <GriloBrowse>
#include <GriloMedia>

#include <QVariant>

SimpleModel::SimpleModel(QObject *parent) :
    GriloModel(parent),
    m_grlRegistry(0),
    m_grlBrowse(0)
{
    m_grlRegistry = new GriloRegistry();
    // Not nice that we have to call this ...
    m_grlRegistry->componentComplete();
    m_grlRegistry->loadPluginById("grl-filesystem");

    m_grlBrowse = new GriloBrowse();
    m_grlBrowse->setSource("grl-filesystem");
    m_grlBrowse->setRegistry(m_grlRegistry);

    QVariantList metaDataKeys;
    metaDataKeys.append(QVariant(GriloDataSource::Title));

    QVariantList typeFilter;
    typeFilter.append(QVariant(GriloDataSource::Audio));

    m_grlBrowse->setMetadataKeys(metaDataKeys);
    m_grlBrowse->setTypeFilter(typeFilter);

    QObject::connect(m_grlBrowse, SIGNAL(baseMediaChanged()), m_grlBrowse, SLOT(refresh()));

    setSource(m_grlBrowse);
}

SimpleModel::~SimpleModel()
{
    if (m_grlBrowse) {
        delete m_grlBrowse;
    }

    if (m_grlRegistry) {
        delete m_grlRegistry;
    }
}

QVariant SimpleModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount()) {
        return QVariant();
    }

    switch (role) {
    case GriloModel::MediaRole:
        return QVariant::fromValue(get(index.row()));
    case Qt::DisplayRole:
        return qobject_cast<GriloMedia*>(get(index.row()))->title();
    }

    return QVariant();
}

void SimpleModel::onItemClicked(const QModelIndex& index)
{
    QVariant mediaVariant = data(index, GriloModel::MediaRole);

    GriloMedia *media = mediaVariant.value<GriloMedia*>();
    if (!media) {
        return;
    }

    if (media->isContainer()) {
        m_grlBrowse->setBaseMedia(media->serialize());
        qWarning() << media->serialize();
    }
}
