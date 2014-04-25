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

#include "uservcard.h"

UserVCard::UserVCard()
{

}

QImage UserVCard::getAvatar()
{
    buffer.setData(vCard.photo());
    buffer.open(QIODevice::ReadOnly);
    QImageReader imageReader(&buffer);
    avatar = imageReader.read();
    return avatar;

}

void UserVCard::setVCard(const QXmppVCardIq &VCard)
{
    vCard=VCard;
}

