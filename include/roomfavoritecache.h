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

#ifndef ROOMFAVORITECACHE_H
#define ROOMFAVORITECACHE_H
#include <QStringList>
#include <QDomElement>
#include <QDir>
#include <QFile>
#include <QDesktopServices>
#include <QTextStream>
class RoomFavoriteCache
{
public:
    RoomFavoriteCache();
    void removeRoomFavorite(const QString& roomJid);
    void addRoomFavorite(const QString& roomJid);
    QStringList getRoomFavoriteList();
    void loadFromFile(const QString& Account);
    void changeStatusText(const QString& statusText);
    QString getStatusText();
private:
    void saveToFile();
    QDomDocument RoomFavoriteDocument;
    QString dir ;
    QString account;
};

#endif // ROOMFAVORITECACHE_H
