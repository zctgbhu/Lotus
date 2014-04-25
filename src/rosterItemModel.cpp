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


#include "rosterItemModel.h"
#include   <QtDebug>
RosterItemModel::RosterItemModel(QObject* parent) : QStandardItemModel(parent)
{

}

RosterItem* RosterItemModel::getRosterItemFromBareJid(const QString& bareJid)
{
    if(m_jidRosterItemMap.contains(bareJid))
        return m_jidRosterItemMap[bareJid];
    else
        return 0;
}

RosterItem* RosterItemModel::getOrCreateItem(const QString& bareJid, RosterItem* pitem)
{//only can createItem
    qDebug() << "getOrCreateItem " << bareJid;
        RosterItem* item = new RosterItem(bareJid);
        item->setData(USER_ROLE_ROSTER, Qt::UserRole);
        m_jidRosterItemMap[bareJid] = item;
        pitem->appendRow(item);
        return item;

}

#include <QXmppPresence.h>
void RosterItemModel::updatePresence(const QString& bareJid,
                                     const QXmppPresence& presence)
{
    RosterItem *item = getRosterItemFromBareJid(bareJid);
    //if (!presence.){
        item->setPresence(presence);
    //}else
    //    item->setPresence(QXmppPresence(QXmppPresence::Unavailable));
}
RosterItem*  RosterItemModel::createGroup(const QString& groupname){
        qDebug() << "groupname=" <<groupname;
    RosterItem* groups = new RosterItem(groupname);
    groups->setData(USER_ROLE_GROUP, Qt::UserRole);
    m_jidRosterGroupMap[groupname] = groups;
    appendRow(groups);
    groups->setName(groupname);
    return groups;
}

void RosterItemModel::groupchange(const QString& bareJid, const QString& groupname, const QString& rostername){
    removeRosterEntry(bareJid);
    if(m_jidRosterGroupMap.contains(groupname)){
        getOrCreateItem(bareJid,m_jidRosterGroupMap.value(groupname))->setName(rostername);
    }else{
        getOrCreateItem(bareJid,createGroup(groupname))->setName(rostername);
    }
}

void RosterItemModel::rosterExist(const QString& bareJid, const QXmppRosterIq::Item& rosterEntry)
{   QString groupname;
    if(rosterEntry.groups().isEmpty()){
        groupname=QString::fromUtf8("我的好友");
        if(m_jidRosterUserMap.value(rosterEntry.name())==groupname){
        //group no change
            m_jidRosterItemMap[bareJid]->setName(rosterEntry.name());
        }else{
        //group change
            groupchange(bareJid,groupname,rosterEntry.name());
        }
    }else{
    foreach (groupname, rosterEntry.groups()){//one roster only can in one group
        if(m_jidRosterUserMap.value(rosterEntry.name())==groupname){
        //group no change
            m_jidRosterItemMap[bareJid]->setName(rosterEntry.name());
        }else{
        //group change
            groupchange(bareJid,groupname,rosterEntry.name());
        }
    }
    }
}

void  RosterItemModel::rosterNotExist(const QString& bareJid, const QXmppRosterIq::Item& rosterEntry){
    QString groupname;
    if(rosterEntry.groups().isEmpty()){
         groupname=QString::fromUtf8("我的好友");
         if(m_jidRosterGroupMap.contains(groupname)){
         //group exist
              getOrCreateItem(bareJid,
                              m_jidRosterGroupMap.value(groupname))->setName(rosterEntry.name());
         }else{
         //group not exist
             getOrCreateItem(bareJid,createGroup(groupname))->setName(rosterEntry.name());
         }
     }else{
    foreach (groupname, rosterEntry.groups()){//one roster only can in one group
        if(m_jidRosterGroupMap.contains(groupname)){
        //group exist
             getOrCreateItem(bareJid,m_jidRosterGroupMap.value(groupname))->setName(rosterEntry.name());
        }else{
        //group not exist
            getOrCreateItem(bareJid,createGroup(groupname))->setName(rosterEntry.name());
        }
    }
    }
}

void RosterItemModel::updateRosterEntry(const QString& bareJid, const QXmppRosterIq::Item& rosterEntry)
{
    qDebug() << "subscription="<<rosterEntry.subscriptionType();

        if(getRosterItemFromBareJid(bareJid)){
            qDebug() << "roster exist";
            //roster exist
            rosterExist(bareJid, rosterEntry);
        }else{qDebug() << "roster not exist";
            //roster not exist
            rosterNotExist(bareJid, rosterEntry);
        }

}



void RosterItemModel::updateAvatar(const QString& bareJid, const QImage& image)
{//todo
    getRosterItemFromBareJid(bareJid)->setAvatar(image);
}

void RosterItemModel::updateName(const QString& bareJid, const QString& name)
{

}

void RosterItemModel::clear()
{
    QStandardItemModel::clear();
    m_jidRosterItemMap.clear();
}

void RosterItemModel::removeRosterEntry(const QString& bareJid)
{
        qDebug() << "bareJid=" <<bareJid;
    RosterItem* item = getRosterItemFromBareJid(bareJid);
    qDebug() << "item=" <<item;
    if(item)
    {
         qDebug() << "item->row=" <<item->row();
         qDebug() << "item->index=" <<item->index();
         qDebug() << "removeRosterEntry";
        removeRow(item->row(),item->parent()->index());
        m_jidRosterItemMap.remove(bareJid);
    }
}
