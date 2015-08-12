// -*- c++ -*-

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

#include "grilosingledatasource.h"
#include "griloregistry.h"

#include <QDebug>

GriloSingleDataSource::GriloSingleDataSource(QObject *parent) :
  GriloDataSource(parent),
  m_available(false) {
}

GriloSingleDataSource::~GriloSingleDataSource() {

}

GrlSource* GriloSingleDataSource::getSource() const {
  if (!m_registry) {
    qWarning() << "GriloRegistry is not set";
    return 0;
  }

  if (m_source.isEmpty()) {
    qWarning() << "Source's id is not set";
    return 0;
  }

  return m_registry->lookupSource(m_source);
}

QString GriloSingleDataSource::source() const {
  return m_source;
}

void GriloSingleDataSource::setSource(const QString& source) {
  if (m_source != source) {
    m_source = source;
    emit sourceChanged();
    emit slowKeysChanged();
    emit supportedKeysChanged();
  }
}

QVariantList GriloSingleDataSource::supportedKeys() const {
  if (m_source.isEmpty() || !m_registry) {
    return QVariantList();
  }

  GrlSource *src = m_registry->lookupSource(m_source);
  if (src) {
    return listToVariantList(grl_source_supported_keys(src));
  }

  return QVariantList();
}

QVariantList GriloSingleDataSource::slowKeys() const {
  if (m_source.isEmpty() || !m_registry) {
    return QVariantList();
  }

  GrlSource *src = m_registry->lookupSource(m_source);
  if (src) {
    return listToVariantList(grl_source_slow_keys(src));
  }

  return QVariantList();
}

bool GriloSingleDataSource::isAvailable() const {
  return m_registry && !m_source.isEmpty() &&
    m_registry->availableSources().indexOf(m_source) != -1;
}

void GriloSingleDataSource::availableSourcesChanged() {
  bool available = isAvailable();

  if (m_available != available) {
    m_available = available;

    emit availabilityChanged();
  }

  if (!m_available && m_opId) {
    // A source has disappeared while an operation is already running.
    // Most grilo will crash soon but we will just reset the opId
    m_opId = 0;
  }
}
