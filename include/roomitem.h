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
#ifndef ROOMITEM_H
#define ROOMITEM_H

#include <QStandardItem>
#include <QItemDelegate>
#include <QPainter>
#include "QXmppUtils.h"
#include <QApplication>
#include <QMouseEvent>
#include <QModelIndex>
#include <QObject>
#include <QToolTip>
class RoomItem : public QStandardItem
{

public:
    enum userRoles
    {
        RoomJid,
        StatusText = Qt::UserRole + 2,
        StatusType,
        Avatar,
        BareJid,
        ItemSelected,
        ItemFavorite
    };
    RoomItem(const QString& roomJid);
};

class RoomItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    RoomItemDelegate();
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    void favoriteButtonClicked(const QModelIndex &index);
    void showGroupChatDlg(const QModelIndex &index);
    void setItemSelected(const QModelIndex &index);
    void favoriteButtonEnter();
private:
    QStyle::State  _state;
};
#endif // ROOMITEM_H
