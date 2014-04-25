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
#include "roomitem.h"
#include <QDebug>


RoomItem::RoomItem(const QString &roomJid)
{
    setData(roomJid,RoomItem::RoomJid);
}


RoomItemDelegate::RoomItemDelegate()
{
}

QSize RoomItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

        return QSize(40, 40);
}

void RoomItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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
        if(index.row() % 2)
            painter->fillRect(option.rect, alternateBg);

    if (index.data(RoomItem::ItemSelected).toString() == "1")
    {
        painter->fillRect(option.rect, selectedBg);
            painter->fillRect(option.rect, option.palette.highlight());
            penDivision.setColor(option.palette.highlight().color());
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
    //qDebug()<<"roomjid="<<index.data(RoomItem::RoomJid).toString();
    painter->drawText(rect,QXmppUtils::jidToUser(index.data(RoomItem::RoomJid).toString()));
    painter->setPen(statusTextColor);
    rect.setTop(rect.y() + rect.height()/2);
    rect.moveTop(rect.y() - 3);
    QString statusText = index.data(RoomItem::StatusText).toString();
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

    //QImage image;
//    value = index.data(RoomItem::Avatar);
//    if(value.type() == QVariant::Image)
//    {
//        image = qvariant_cast<QImage>(value);
//    }




    QRect buttonRect( option.rect);
    buttonRect.setY(option.rect.y()+5);
    buttonRect.setX(option.rect.x()+240);
    buttonRect.setHeight(30);
    buttonRect.setWidth(30);
    if(index.data(RoomItem::ItemFavorite).toString()=="0")
    {
        painter->drawImage(buttonRect,QImage(":/pixmap/image/roomFavorite_b.png"));
    }else{
        painter->drawImage(buttonRect,QImage(":/pixmap/image/roomFavorite.png"));
    }

        //QApplication::style()->drawItemPixmap(painter,buttonRect,QStyle::State_Sunken,QPixmap(":/pixmap/image/roomFavorite_b.png").scaled(30,30));
    painter->restore();
}
bool RoomItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    //qDebug() << "editorEvent::" << event->type();
    if(
        event->type() == QEvent::MouseButtonRelease ||
            event->type() == QEvent::MouseButtonDblClick)
    {
        QRect buttonRect( option.rect);
        buttonRect.setY(option.rect.y()+ 5);
        buttonRect.setX(option.rect.x()+240);
        buttonRect.setHeight(30);
        buttonRect.setWidth(30);

        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if( !buttonRect.contains( mouseEvent->pos()))
        {
            if(mouseEvent->type()==QEvent::MouseButtonDblClick)
            {
                emit showGroupChatDlg(index);
            }else if(mouseEvent->type()==QEvent::MouseButtonRelease)
            {
                qDebug()<<"MouseButtonPress";
                emit setItemSelected(index);

            }
            _state = QStyle::State_Raised;

            return true;
        }
        if( event->type() == QEvent::Enter) {//To Fix:增加收藏提示
            qDebug()<<"asdadasdsadadsadsadsad";
            QToolTip::showText(mouseEvent->pos(),"WWWW");
            emit favoriteButtonEnter();
        }
        if( event->type() == QEvent::MouseButtonRelease) {

            emit favoriteButtonClicked(index);
        }
        return true;
    } else {

        return false;
    }


}
