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

#ifndef CHATSLOG_H
#define CHATSLOG_H
#include <QString>
#include <QDesktopServices>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
class ChatsLog
{
public:
    ChatsLog();
    void setUserName(const QString &name);
    void addChatLog(const QString &chat2Name,const QString &chatContent);
    QString getChatLog(const QString &chat2Name);
    QDateTime getLastChatTime(const QString &chat2Name);
private:
    QString userName;
    QString baseDir;
    QString userDir;
    QString chatContent;
    void addLastChatTime(const QString &chat2Name,const QString &chatContent);
    QByteArray calculateXor(const QByteArray &data, const QByteArray &key);
};

#endif // CHATSLOG_H
