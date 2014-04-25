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

#include "chatslog.h"
#include <QDebug>

ChatsLog::ChatsLog()
{
    baseDir=QDesktopServices::storageLocation(QDesktopServices::HomeLocation)+"/.Lotus/";
    QDir cacheDir(baseDir);
    if(!cacheDir.exists())
        cacheDir.mkpath(baseDir);
}

void ChatsLog::setUserName(const QString &name)
{
    userName=name;
    userDir=baseDir+userName+"/";
    qDebug()<<"userDir="<<userDir;
    QDir cacheDir(userDir);
    if(!cacheDir.exists())
        cacheDir.mkpath(userDir);
}

void ChatsLog::addChatLog(const QString &chat2Name, const QString &chatContent)
{
    QFile file(userDir+chat2Name);
    if(file.open(QIODevice::ReadWrite|QIODevice::Append))
    {
        QDataStream textStream(&file);
        textStream << calculateXor(chatContent.toUtf8(),userName.toUtf8());
        file.close();
    }
    addLastChatTime(chat2Name,chatContent);

}

QString ChatsLog::getChatLog(const QString &chat2Name)
{
    QFile file(userDir+chat2Name);
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream textStream(&file);
        QByteArray chatContentEncrypted;
        QByteArray tmp;
        while (!textStream.atEnd()) {
            textStream>>chatContentEncrypted;
            tmp +=calculateXor(chatContentEncrypted,userName.toUtf8());
        }
        //textStream>>chatContentEncrypted;
        chatContent=tmp;
        file.close();
    }
    qDebug()<<"chatContent="<<chatContent;
    return chatContent;
}

QDateTime ChatsLog::getLastChatTime(const QString &chat2Name)
{
    QFile file(userDir+chat2Name+"_stamp");
    QByteArray tmp;
    if(file.open(QIODevice::ReadOnly))
    {

        QDataStream textStream(&file);
        QByteArray chatContentEncrypted;
        while (!textStream.atEnd()) {
            textStream>>chatContentEncrypted;
            tmp +=calculateXor(chatContentEncrypted,userName.toUtf8());
        }
        file.close();
        qDebug()<<"LastChatTime="<<tmp;
    }
    return QDateTime::fromString((QString)tmp,"yyyy/MM/dd hh:mm:ss");
}

void ChatsLog::addLastChatTime(const QString &chat2Name, const QString &chatContent)
{
    QFile stampFile(userDir+chat2Name+"_stamp");
    if(stampFile.open(QIODevice::ReadWrite|QIODevice::Truncate))
    {
        QDataStream textStream(&stampFile);
        QString tmp=chatContent.section("</font>",1,1).remove("<font color='grey'>").remove("<font color='green'>");
        textStream << calculateXor(tmp.toUtf8(),userName.toUtf8());
        stampFile.close();
    }
}

QByteArray ChatsLog::calculateXor(const QByteArray &data, const QByteArray &key)
{
    if(key.isEmpty())
        return data;

    QByteArray result;
    for(int i = 0 , j = 0; i < data.length(); ++i , ++j)
    {
        if(j == key.length())
            j = 0;
        result.append(data.at(i) ^ key.at(j));
    }
    return result;
}
