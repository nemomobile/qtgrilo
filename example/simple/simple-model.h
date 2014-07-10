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

#ifndef SIMPLE_MODEL_H
#define SIMPLE_MODEL_H

#include <GriloModel>

#include <QModelIndex>

class GriloRegistry;
class GriloBrowse;

class SimpleModel : public GriloModel
{
    Q_OBJECT

public:
    SimpleModel(QObject *parent = 0);
    ~SimpleModel();

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

public slots:
    void onItemClicked(const QModelIndex& index);

private:
    GriloRegistry *m_grlRegistry;
    GriloBrowse *m_grlBrowse;
};

#endif /* SIMPLE_MODEL_H */
