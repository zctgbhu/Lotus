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

#include "roomfavoritecache.h"

RoomFavoriteCache::RoomFavoriteCache()
{
    dir = QDesktopServices::storageLocation(QDesktopServices::HomeLocation)+"/.Lotus/";
}

void RoomFavoriteCache::removeRoomFavorite(const QString &roomJid)
{
    if(RoomFavoriteDocument.documentElement().isNull())
    {
        return;
    }
    QDomElement element = RoomFavoriteDocument.documentElement().firstChildElement("roomFavorite");
    while(!element.isNull())
    {
        if(element.firstChildElement("roomJid").text() == roomJid)
        {
            RoomFavoriteDocument.documentElement().removeChild(element);
        }
        element = element.nextSiblingElement("roomFavorite");
    }
    saveToFile();
}

void RoomFavoriteCache::addRoomFavorite(const QString &roomJid)
{
    if(RoomFavoriteDocument.documentElement().isNull()
     ||RoomFavoriteDocument.documentElement().firstChildElement("roomFavorite").isNull())
    {
        RoomFavoriteDocument.appendChild(RoomFavoriteDocument.createElement("cache"));
    }
    QDomElement newElement = RoomFavoriteDocument.createElement("roomFavorite");
    QDomElement newElementRoomJid = RoomFavoriteDocument.createElement("roomJid");
    newElementRoomJid.appendChild(RoomFavoriteDocument.createTextNode(roomJid));
    newElement.appendChild(newElementRoomJid);
    RoomFavoriteDocument.documentElement().appendChild(newElement);
    saveToFile();
}

QStringList RoomFavoriteCache::getRoomFavoriteList()
{
    QStringList list;
    QDomElement element = RoomFavoriteDocument.documentElement().firstChildElement("roomFavorite");
    while(!element.isNull())
    {
        list << element.firstChildElement("roomJid").text();
        element = element.nextSiblingElement("roomFavorite");
    }
    return list;
}

void RoomFavoriteCache::loadFromFile(const QString &Account)
{
    account=Account;
    QDir cacheDir(dir);
    if(cacheDir.exists())
    {
        QFile file(dir+ "roomFavoriteCache_"+account);
        if(file.open(QIODevice::ReadOnly))
        {
            RoomFavoriteDocument.setContent(&file, true);
        }
    }
}

void RoomFavoriteCache::changeStatusText(const QString &statusText)
{
    if(RoomFavoriteDocument.documentElement().isNull()
     ||RoomFavoriteDocument.documentElement().firstChildElement("statusText").isNull())
    {
        RoomFavoriteDocument.appendChild(RoomFavoriteDocument.createElement("cache"));
        QDomElement ElementStatusText = RoomFavoriteDocument.createElement("statusText");
        ElementStatusText.appendChild(RoomFavoriteDocument.createTextNode(statusText));
        RoomFavoriteDocument.documentElement().appendChild(ElementStatusText);
    }else{
        RoomFavoriteDocument.documentElement().firstChildElement("statusText").firstChild().setNodeValue(statusText);
    }
    saveToFile();
}

QString RoomFavoriteCache::getStatusText()
{
    if(RoomFavoriteDocument.documentElement().isNull()
     ||RoomFavoriteDocument.documentElement().firstChildElement("statusText").isNull())
    {
        return NULL;
    }
    return RoomFavoriteDocument.documentElement().firstChildElement("statusText").text();
}

void RoomFavoriteCache::saveToFile()
{
    QDir cacheDir;
    if(!cacheDir.exists(dir))
        cacheDir.mkpath(dir);

    QString fileAccounts =dir + "roomFavoriteCache_"+account;
    QFile file(fileAccounts);
    if(file.open(QIODevice::ReadWrite))
    {
        QTextStream textStream(&file);
        RoomFavoriteDocument.save(textStream, 2);
        file.close();
    }
}
