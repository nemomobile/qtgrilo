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

#include "grilomedia.h"
#include <QDebug>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
# include <QQmlInfo>
#else
# include <QDeclarativeInfo>
#endif


GriloMedia::GriloMedia(GrlMedia *media, QObject *parent) :
  QObject(parent), m_media(media) {

}

GriloMedia::~GriloMedia() {
  g_object_unref(m_media);
  m_media = 0;
}

GrlMedia *GriloMedia::media() {
  return m_media;
}

void GriloMedia::setMedia(GrlMedia *media) {
  if (m_media != media) {
    g_object_unref(m_media);
    m_media = media;
  }
}

QVariant GriloMedia::get(const QString& keyId) const {
  // There is a GriloRegistry Qt object, but it is not smart to add a
  // dependency to it here since GrlRegistry is, actually, a
  // singleton.
  GrlKeyID actualKey = grl_registry_lookup_metadata_key(grl_registry_get_default(),
                                                        keyId.toUtf8().constData());

  if (GRL_METADATA_KEY_INVALID == actualKey) {
    qmlInfo(this) << "Grilo key doesn't exist in the register.";
    return QVariant();
  }

  const GValue *gValue = grl_data_get(GRL_DATA(m_media), actualKey);

  return convertValue(gValue);
}

QString GriloMedia::serialize() {
  QString result;
  gchar *str = grl_media_serialize_extended(m_media, GRL_MEDIA_SERIALIZE_FULL, NULL);

  if (str) {
    result = QString::fromUtf8(str);
    g_free(str);
  }

  return result;
}

QString GriloMedia::id() const {
  return QString::fromUtf8(grl_media_get_id(m_media));
}

QString GriloMedia::title() const {
  return QString::fromUtf8(grl_media_get_title(m_media));
}

QUrl GriloMedia::url() const {
  QUrl url = QUrl::fromEncoded(QByteArray(grl_media_get_url(m_media)));

  return url;
}

int GriloMedia::duration() const {
  return grl_media_get_duration(m_media);
}

bool GriloMedia::isContainer() const {
  return GRL_IS_MEDIA_BOX(m_media) == TRUE;
}

QString GriloMedia::author() const {
  return QString::fromUtf8(grl_media_get_author(m_media));
}

QString GriloMedia::album() const {
  if (GRL_IS_MEDIA_AUDIO(m_media)) {
    return QString::fromUtf8(grl_media_audio_get_album(GRL_MEDIA_AUDIO(m_media)));
  }

  return QString();
}

QString GriloMedia::artist() const
{
  if (GRL_IS_MEDIA_AUDIO(m_media)) {
    return QString::fromUtf8(grl_media_audio_get_artist(GRL_MEDIA_AUDIO(m_media)));
  }

  return QString();
}

QString GriloMedia::genre() const
{
  if (GRL_IS_MEDIA_AUDIO(m_media)) {
    return QString::fromUtf8(grl_media_audio_get_genre(GRL_MEDIA_AUDIO(m_media)));
  }

  return QString();
}

QUrl GriloMedia::thumbnail() const
{
  if (GRL_IS_MEDIA(m_media)) {
    return QUrl(grl_media_get_thumbnail(GRL_MEDIA(m_media)));
  }

  return QUrl();
}

int GriloMedia::year() const
{
  if (GRL_IS_MEDIA(m_media)) {
    return g_date_time_get_year(grl_media_get_creation_date(GRL_MEDIA(m_media)));
  }

  return 0;
}

int GriloMedia::trackNumber() const
{
  if (GRL_IS_MEDIA_AUDIO(m_media)) {
    return grl_media_audio_get_track_number(GRL_MEDIA_AUDIO(m_media));
  }

  return 0;
}

int GriloMedia::childCount() const
{
  if (GRL_IS_MEDIA_BOX(m_media)) {
    return grl_media_box_get_childcount(GRL_MEDIA_BOX(m_media));
  }

  return GRL_METADATA_KEY_CHILDCOUNT_UNKNOWN;
}

QString GriloMedia::mimeType() const
{
  return QString::fromUtf8(grl_media_get_mime(m_media));
}

QVariant GriloMedia::convertValue(const GValue *value) const
{
  if (!value) {
    return QVariant();
  }

  switch (G_VALUE_TYPE(value)) {
  case G_TYPE_BOOLEAN:
    return QVariant::fromValue(static_cast<bool>(g_value_get_boolean(value)));
  case G_TYPE_BOXED: {
    GByteArray *array;

    array = static_cast<GByteArray *>(g_value_get_boxed(value));
    const char *arrayData = reinterpret_cast<char *>(array->data);
    return QVariant::fromValue(QByteArray::fromRawData(arrayData, array->len));
  }
  case G_TYPE_DOUBLE:
    return QVariant::fromValue(g_value_get_double(value));
  case G_TYPE_ENUM:
    return QVariant::fromValue(g_value_get_enum(value));
  case G_TYPE_FLAGS:
    return QVariant::fromValue(g_value_get_flags(value));
  case G_TYPE_FLOAT:
    return QVariant::fromValue(g_value_get_float(value));
  case G_TYPE_INT:
    return QVariant::fromValue(g_value_get_int(value));
  case G_TYPE_INT64:
    return QVariant::fromValue(g_value_get_int64(value));
  case G_TYPE_LONG:
    return QVariant::fromValue(g_value_get_long(value));
  case G_TYPE_CHAR:
    return QVariant::fromValue(QChar(g_value_get_schar(value)));
  case G_TYPE_STRING:
    return QVariant::fromValue(QString::fromUtf8(g_value_get_string(value)));
  case G_TYPE_UCHAR:
    return QVariant::fromValue(QChar(g_value_get_uchar(value)));
  case G_TYPE_UINT:
    return QVariant::fromValue(g_value_get_uint(value));
  case G_TYPE_UINT64:
    return QVariant::fromValue(g_value_get_uint64(value));
  case G_TYPE_ULONG:
    return QVariant::fromValue(g_value_get_ulong(value));
  }

  // We cannot use G_TYPE_GTYPE as a checking value so we check,
  // finally, here.
  if (G_UNLIKELY(G_VALUE_HOLDS_GTYPE(value))) {
    return QVariant::fromValue(g_value_get_gtype(value));
  }

  qmlInfo(this) << "GValue not converted to QVariant.";

  return QVariant();
}

