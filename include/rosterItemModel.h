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

#ifndef ROSTERITEMMODEL_H
#define ROSTERITEMMODEL_H

#include <QStandardItemModel>
#include "rosterItem.h"
#include "QXmppRosterManager.h"
#include "QXmppPresence.h"
#include <QDragEnterEvent>
#include <QDropEvent>

class RosterItemModel : public QStandardItemModel
{
public:
    RosterItemModel(QObject* parent);
    RosterItem* getRosterItemFromBareJid(const QString& bareJid);

    void updatePresence(const QString& bareJid, const QXmppPresence &presence);
    void updateRosterEntry(const QString& bareJid, const QXmppRosterIq::Item& rosterEntry);
    void updateAvatar(const QString& bareJid, const QImage& image);
    void updateName(const QString& bareJid, const QString& name);
    void removeRosterEntry(const QString& bareJid);
    void clear();

private:
    RosterItem* getOrCreateItem(const QString& bareJid, RosterItem *pitem);
    void  rosterExist(const QString& bareJid, const QXmppRosterIq::Item& rosterEntry);
    void  rosterNotExist(const QString& bareJid, const QXmppRosterIq::Item& rosterEntry);
    void  groupchange(const QString& bareJid, const QString& groupname, const QString& rostername);
    RosterItem*  createGroup(const QString& groupname);
    QMap<QString, RosterItem*> m_jidRosterItemMap;
    QMap<QString, RosterItem*> m_jidRosterGroupMap;
    QMap<QString, QString> m_jidRosterUserMap;
};

#endif // ROSTERITEMMODEL_H
