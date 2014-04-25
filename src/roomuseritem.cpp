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

#include "roomuseritem.h"
#include<QDebug>

RoomUserItem::RoomUserItem(const QString &roomUserJid)
{
    setData(roomUserJid,RoomUserItem::RoomUserJid);
}


RoomUserItemDelegate::RoomUserItemDelegate()
{

}

QSize RoomUserItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

        return QSize(34, 30);
}

void RoomUserItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::TextAntialiasing);
    QVariant value = index.data(Qt::DecorationRole);
    QColor selectedBg(60, 140, 222);
    QColor alternateBg(239, 245, 254);
    QColor selectedText(Qt::white);

    QColor nameTextColor(Qt::black);
    QColor statusTextColor(Qt::darkGray);

    QPixmap pixmap;
    if(value.type() == QVariant::Icon)
    {
        QIcon icon = qvariant_cast<QIcon>(value);
        pixmap = icon.pixmap(QSize(16, 16), QIcon::Normal, QIcon::On);
    }

    QPen penDivision;
//        if(index.row() % 2)
//            painter->fillRect(option.rect, alternateBg);

    if (option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, selectedBg);
//            painter->fillRect(option.rect, option.palette.highlight());
//            penDivision.setColor(option.palette.highlight().color());
        penDivision.setColor(selectedBg);
        nameTextColor = selectedText;
        statusTextColor = selectedText;
    }
    else
    {
        penDivision.setColor(QColor(244, 244, 244));
    }

    QRect rect = option.rect;
    rect.setWidth(pixmap.width());
    rect.setHeight(pixmap.height());
    rect.moveTop(rect.y() + (option.rect.height() - pixmap.height())/2);
    rect.moveLeft(rect.left() + 2);
    painter->drawPixmap(rect, pixmap);

    rect = option.rect;
    rect.setLeft(rect.x() + pixmap.width() + 8);
    rect.moveTop(rect.y() + 3);
    QFont font;
    painter->setFont(font);
    painter->setPen(nameTextColor);
//    qDebug()<<"jidToBareJid="<<QXmppUtils::jidToBareJid(index.data(RoomUserItem::RoomUserJid).toString());
//    qDebug()<<"jidToDomain="<<QXmppUtils::jidToDomain(index.data(RoomUserItem::RoomUserJid).toString());
//    qDebug()<<"jidToResource="<<QXmppUtils::jidToResource(index.data(RoomUserItem::RoomUserJid).toString());
//    qDebug()<<"jidToUser="<<QXmppUtils::jidToUser(index.data(RoomUserItem::RoomUserJid).toString());
//    qDebug()<<"jid="<<index.data(RoomUserItem::RoomUserJid).toString();

    painter->drawText(rect,QXmppUtils::jidToUser(QXmppUtils::jidToResource(index.data(RoomUserItem::RoomUserJid).toString())));
    painter->setPen(statusTextColor);
    rect.setTop(rect.y() + rect.height()/2);
    rect.moveTop(rect.y() - 3);
    QString statusText = index.data(RoomUserItem::StatusText).toString();
    QFontMetrics fontMetrics(font);
    statusText = fontMetrics.elidedText(statusText, Qt::ElideRight, rect.width() - 34);
    painter->drawText(rect, statusText);

    penDivision.setWidth(0);
    painter->setPen(penDivision);

    rect = option.rect;
    QPoint left = rect.bottomLeft();
    left.setX(left.x() + 4);
    QPoint right = rect.bottomRight();
    right.setX(right.x() - 4);
    painter->drawLine(left, right);

    QImage image;
    value = index.data(RoomUserItem::Avatar);
    if(value.type() == QVariant::Image)
    {
        image = qvariant_cast<QImage>(value);
    }

    pixmap = QPixmap(":/icons/resource/avatar.png");
    rect = option.rect;
    rect.setWidth(pixmap.width());
    rect.setHeight(pixmap.height());
    rect.moveTop(rect.y() + (option.rect.height() - pixmap.height())/2);
    rect.moveLeft(option.rect.x() + option.rect.width() - pixmap.width() - 2);

//    if(image.isNull())
//        painter->drawPixmap(rect, pixmap);
//    else
        painter->drawImage(rect, image);

    painter->restore();
}
