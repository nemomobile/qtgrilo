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

#ifndef GRILO_DATA_SOURCE_H
#define GRILO_DATA_SOURCE_H

#include <QObject>
#include <grilo.h>
#include <QVariant>

class GriloMedia;
class GriloModel;
class GriloRegistry;
class GriloDataSourcePrivate;

class GriloDataSource : public QObject {
  Q_OBJECT

  Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged);
  Q_PROPERTY(int skip READ skip WRITE setSkip NOTIFY skipChanged);
  Q_PROPERTY(QVariantList metadataKeys READ metadataKeys WRITE setMetadataKeys NOTIFY metadataKeysChanged);
  Q_PROPERTY(QVariantList typeFilter READ typeFilter WRITE setTypeFilter NOTIFY typeFilterChanged);

  Q_ENUMS(MetadataKeys);
  Q_ENUMS(TypeFilter);
  // TODO: metadata resolution flags ?

public:
  enum MetadataKeys {
    Album = GRL_METADATA_KEY_ALBUM,
    Artist = GRL_METADATA_KEY_ARTIST,
    Author = GRL_METADATA_KEY_AUTHOR,
    Bitrate = GRL_METADATA_KEY_BITRATE,
    Certificate = GRL_METADATA_KEY_CERTIFICATE,
    Childcount = GRL_METADATA_KEY_CHILDCOUNT,
    PublicationDate = GRL_METADATA_KEY_PUBLICATION_DATE,
    Description = GRL_METADATA_KEY_DESCRIPTION,
    Duration = GRL_METADATA_KEY_DURATION,
    ExternalPlayer = GRL_METADATA_KEY_EXTERNAL_PLAYER,
    ExternalUrl = GRL_METADATA_KEY_EXTERNAL_URL,
    Framerate = GRL_METADATA_KEY_FRAMERATE,
    Genre = GRL_METADATA_KEY_GENRE,
    Height = GRL_METADATA_KEY_HEIGHT,
    Id = GRL_METADATA_KEY_ID,
    LastPlayed = GRL_METADATA_KEY_LAST_PLAYED,
    LastPosition = GRL_METADATA_KEY_LAST_POSITION,
    License = GRL_METADATA_KEY_LICENSE,
    Lyrics = GRL_METADATA_KEY_LYRICS,
    Mime = GRL_METADATA_KEY_MIME,
    PlayCount = GRL_METADATA_KEY_PLAY_COUNT,
    Rating = GRL_METADATA_KEY_RATING,
    Site = GRL_METADATA_KEY_SITE,
    Source = GRL_METADATA_KEY_SOURCE,
    Studio = GRL_METADATA_KEY_STUDIO,
    Thumbnail = GRL_METADATA_KEY_THUMBNAIL,
    ThumbnailBinary = GRL_METADATA_KEY_THUMBNAIL_BINARY,
    Title = GRL_METADATA_KEY_TITLE,
    Url = GRL_METADATA_KEY_URL,
    Width = GRL_METADATA_KEY_WIDTH,
    Season = GRL_METADATA_KEY_SEASON,
    Episode = GRL_METADATA_KEY_EPISODE,
    Show = GRL_METADATA_KEY_SHOW,
    CreationDate = GRL_METADATA_KEY_CREATION_DATE,
    CameraModel = GRL_METADATA_KEY_CAMERA_MODEL,
    Orientation = GRL_METADATA_KEY_ORIENTATION,
    FlashUsed = GRL_METADATA_KEY_FLASH_USED,
    ExposureTime = GRL_METADATA_KEY_EXPOSURE_TIME,
    IsoSpeed = GRL_METADATA_KEY_ISO_SPEED,
    TrackNumber = GRL_METADATA_KEY_TRACK_NUMBER,
    ModificationDate = GRL_METADATA_KEY_MODIFICATION_DATE,
    StartTime = GRL_METADATA_KEY_START_TIME,
  };

  enum TypeFilter {
    None = GRL_TYPE_FILTER_NONE,
    Audio = GRL_TYPE_FILTER_AUDIO,
    Video = GRL_TYPE_FILTER_VIDEO,
    Image = GRL_TYPE_FILTER_IMAGE,
    All = GRL_TYPE_FILTER_ALL,
  };

  GriloDataSource(GriloDataSourcePrivate *d, QObject *parent = 0);
  virtual ~GriloDataSource();

  const QList<GriloMedia *> *media() const;

  void addModel(GriloModel *model);
  void removeModel(GriloModel *model);

  GriloRegistry *registry() const;

  int count() const;
  void setCount(int count);

  int skip() const;
  void setSkip(int skip);

  QVariantList metadataKeys() const;
  void setMetadataKeys(const QVariantList& keys);

  QVariantList typeFilter() const;
  void setTypeFilter(const QVariantList& filter);

  virtual bool refresh() = 0;

public slots:
  void cancelRefresh();

  virtual void availableSourcesChanged() = 0;

signals:
  void registryChanged();
  void countChanged();
  void skipChanged();
  void metadataKeysChanged();
  void typeFilterChanged();

protected:
  GriloDataSourcePrivate *d_ptr;

  enum OperationType {
    Browse = GRL_OP_BROWSE,
    Search = GRL_OP_SEARCH,
  };

  static void grilo_source_result_cb(GrlSource *source, guint browse_id,
				     GrlMedia *media, guint remaining,
				     gpointer user_data, const GError *error);

  void addMedia(GriloMedia *media);
  void clearMedia();

  GrlOperationOptions *operationOptions(GrlSource *src, const OperationType& type);
  GList *keysAsList();

  QVariantList listToVariantList(const GList *keys) const;

  QVariantList m_metadataKeys;
  QVariantList m_typeFilter;

private:
  void setRegistry(GriloRegistry *registry);
};

#endif /* GRILO_DATA_SOURCE_H */
