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

#ifndef GRILO_MEDIA_H
#define GRILO_MEDIA_H

#include <GriloQt>

#include <QObject>
#include <grilo.h>
#include <QUrl>
#include <QVariant>
#include <QDateTime>

class GRILO_QT_EXPORT GriloMedia : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString id READ id CONSTANT);
  Q_PROPERTY(QString title READ title CONSTANT);
  Q_PROPERTY(QUrl url READ url CONSTANT);
  Q_PROPERTY(bool container READ isContainer CONSTANT);
  Q_PROPERTY(int duration READ duration CONSTANT);
  Q_PROPERTY(QString author READ author CONSTANT);
  Q_PROPERTY(QString album READ album CONSTANT);
  Q_PROPERTY(QString artist READ artist CONSTANT);
  Q_PROPERTY(QString genre READ genre CONSTANT);
  Q_PROPERTY(QUrl thumbnail READ thumbnail CONSTANT);
  Q_PROPERTY(int year READ year CONSTANT);
  Q_PROPERTY(int trackNumber READ trackNumber CONSTANT);
  Q_PROPERTY(int childCount READ childCount CONSTANT);
  Q_PROPERTY(QString mimeType READ mimeType CONSTANT);
  Q_PROPERTY(QDateTime modificationDate READ modificationDate CONSTANT);
  Q_PROPERTY(int height READ height CONSTANT);
  Q_PROPERTY(int orientation READ orientation CONSTANT);
  Q_PROPERTY(int width READ width CONSTANT);
public:
  GriloMedia(GrlMedia *media, QObject *parent = 0);
  ~GriloMedia();

  QString id() const;
  QString title() const;
  QUrl url() const;
  bool isContainer() const;
  int duration() const;
  QString author() const;
  QString album() const;
  QString artist() const;
  QString genre() const;
  QUrl thumbnail() const;
  int year() const;
  int trackNumber() const;
  int childCount() const;
  QString mimeType() const;
  QDateTime modificationDate() const;
  int height() const;
  int orientation() const;
  int width() const;

  GrlMedia *media();
  void setMedia(GrlMedia *media);

  Q_INVOKABLE QVariant get(const QString& keyId) const;
  Q_INVOKABLE QVariant get(const quint32 keyId) const;
  Q_INVOKABLE QString serialize();

private:
  QVariant convertValue(const GValue *value) const;

  GrlMedia *m_media;
};

#endif /* GRILO_MEDIA_H */
