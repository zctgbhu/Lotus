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
#include "roomitemmodel.h"
#include "lotus.h"
#include<QtGui>
#include <QMessageBox>
RoomItemModel::RoomItemModel(QObject *parent): QStandardItemModel(parent)
{
}

int RoomItemModel::updateRoomList(const QString &roomJid)
{
    if(!jidRoomItemMap.contains(roomJid))
    {
        RoomItem* item = new RoomItem(roomJid);
        jidRoomItemMap[roomJid] = item;
        item->setData("0",RoomItem::ItemFavorite);
        appendRow(item);
        return 0;
    }else{
        return 1;
    }
}

/*
 * return 1 if roomJid is not contained
 * return 0 if roomJid is contained and removed
 */
int RoomItemModel::removeRoom(const QString &roomJid)
{
    if(!jidRoomItemMap.contains(roomJid)){
        return 1;
    }else{
        removeRow(this->indexFromItem(jidRoomItemMap[roomJid]).row());
        jidRoomItemMap.remove(roomJid);
        return 0;
    }
}

RoomItem *RoomItemModel::getRoomItemFromBareJid(const QString &roomJid)
{
    if(jidRoomItemMap.contains(roomJid)){
        return jidRoomItemMap[roomJid];
    }else{
        return NULL;
    }
}

Qt::DropActions RoomItemModel::supportedDropActions()const
{
    return Qt::CopyAction|Qt::MoveAction;
}

Qt::ItemFlags RoomItemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags=QStandardItemModel::flags(index);
    if(index.isValid()){
        return Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|defaultFlags;
    }
    return Qt::ItemIsDropEnabled|defaultFlags;
}

QStringList RoomItemModel::mimeTypes()const
{
    QStringList types;
    types<<"application/qxmpp_room";
    return types;
}

QMimeData *RoomItemModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData=new QMimeData();

    QModelIndex idx=indexes[0];
    if(idx.isValid()){
        QByteArray text=data(idx,RoomItem::RoomJid).toByteArray();
        mimeData->setData(QString("application/qxmpp_room"),text);
    }
    return mimeData;
}

bool RoomItemModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                                 int row, int column, const QModelIndex &parent)
{
    if(action==Qt::IgnoreAction)
        return true;
    if(!data->hasFormat("application/qxmpp_room"))
        return false;
    qDebug()<<data->data("application/qxmpp_room");
    emit quitRoom(QString(data->data("application/qxmpp_room")));

    return true;
}
