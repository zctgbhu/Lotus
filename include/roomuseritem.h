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

#ifndef ROOMUSERITEM_H
#define ROOMUSERITEM_H
#include <QStandardItem>
#include <QItemDelegate>
#include <QPainter>
#include "QXmppUtils.h"
class RoomUserItem : public QStandardItem
{
public:
    enum userRoles
    {
        RoomUserJid,
        StatusText = Qt::UserRole + 2,
        StatusType,
        Avatar,
        BareJid
    };
    RoomUserItem(const QString& roomUserJid);
};

class RoomUserItemDelegate : public QItemDelegate
{
public:
    RoomUserItemDelegate();
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    };
#endif // ROOMUSERITEM_H
