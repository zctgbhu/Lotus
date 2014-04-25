/*
 * Copyright (C) 2013 National University of Defense Technology(NUDT) & Kylin Ltd.
 *
 * Authors:
 *	Zhang Chao	zctgbhu@163.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef ROOMITEMMODEL_H
#define ROOMITEMMODEL_H
#include <QStandardItemModel>
#include "roomitem.h"
#include <QStringList>
class RoomItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    RoomItemModel(QObject* parent);
    int updateRoomList(const QString &roomJid);
    int removeRoom(const QString &roomJid);
    RoomItem* getRoomItemFromBareJid(const QString& roomJid);
    Qt::DropActions supportedDropActions()const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QStringList mimeTypes()const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent);
private:
    QMap<QString, RoomItem*> jidRoomItemMap;
signals:
    void quitRoom(const QString&);
};

#endif // ROOMITEMMODEL_H
