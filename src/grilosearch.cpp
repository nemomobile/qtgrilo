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

#include "grilosearch.h"
#include <QDebug>

GriloSearch::GriloSearch(QObject *parent) :
  GriloSingleDataSource(parent) {
}

GriloSearch::~GriloSearch() {

}

bool GriloSearch::refresh() {
  cancelRefresh();

  GrlSource *src = getSource();

  if (!src) {
    qWarning() << "Failed to get source" << source();
    return false;
  }

  GList *keys = keysAsList();
  GrlOperationOptions *options = operationOptions(src, Search);
  m_opId = grl_source_search(src, m_text.toUtf8().constData(),
			     keys, options, grilo_source_result_cb, this);

  g_object_unref(options);
  g_list_free(keys);

  return m_opId != 0;
}

QString GriloSearch::text() const {
  return m_text;
}

void GriloSearch::setText(const QString& text) {
  if (m_text != text) {
    m_text = text;
    emit textChanged();
  }
}
