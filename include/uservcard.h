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

#ifndef USERVCARD_H
#define USERVCARD_H
#include <QString>
#include <QImage>
#include <QImageReader>
#include <QBuffer>
#include "QXmppVCardIq.h"
#include <QDebug>
class UserVCard
{
public:
    UserVCard();
    QXmppVCardIq vCard;
    QImage getAvatar();
    void setVCard(const QXmppVCardIq &VCard);
private:
    QString fullName;
    QImage avatar;
    QBuffer buffer;

};

#endif // USERVCARD_H
