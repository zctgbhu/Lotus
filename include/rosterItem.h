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


#ifndef ROSTERITEM_H
#define ROSTERITEM_H

#include <QStandardItem>
#include <QItemDelegate>
#include <QApplication>
#include <QPainter>
#include "QXmppPresence.h"
#define USER_ROLE_GROUP 1
#define USER_ROLE_ROSTER 0
class RosterItem : public QStandardItem
{
public:
    enum userRoles
    {
        StatusText = Qt::UserRole + 2,
        StatusType,
        PresenceType,
        BareJid,
        Avatar
    };

    RosterItem(const QString& bareJid);

    void setAvatar(const QImage& image);
    void setPresence(const QXmppPresence &presence);
    void setName(const QString& name);
    QImage getAvatar();
    QString getName();
};

class RosterItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit RosterItemDelegate(QObject *parent = 0);
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void paintGroup(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void paintRoster(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    void delButtonClicked(const QModelIndex &index);
    void showPersonalChatDlg(const QModelIndex &index);
    void setItemSelected(const QModelIndex &index);
private:
    QStyle::State  _state;
    bool  selectedState;

};

#endif // ROSTERITEM_H
