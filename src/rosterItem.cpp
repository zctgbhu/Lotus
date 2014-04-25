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

#include "rosterItem.h"
#include <QImage>
#include <QDebug>
#include <QApplication>
#include <QEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QPainter>
#include <QStyleOptionButton>
#include <QObject>
RosterItem::RosterItem(const QString& bareJid)
{
    setData(bareJid, RosterItem::BareJid);
    setData(QObject::tr("不在线"), RosterItem::StatusText);
    setAvatar(QImage(":/pixmap/image/avatar.png"));
    setIcon(QIcon(":/pixmap/image/gray.png"));
}

void RosterItem::setName(const QString& name)
{
    setText(name);

}

QString RosterItem::getName()
{
    return text();
}

void RosterItem::setPresence(const QXmppPresence &presence)
{
    // determine status text
    QString statusText = presence.statusText();
    if (statusText.isEmpty()) {
        if(presence.type() == QXmppPresence::Available)
            statusText = QObject::tr("在线");
        else if(presence.type() == QXmppPresence::Unavailable)
            statusText = QObject::tr("不在线");
    }

    // store data
    setData(statusText, RosterItem::StatusText);
    setData(static_cast<int>(presence.type()), PresenceType);
    setData(static_cast<int>(presence.availableStatusType()), StatusType);

    // update icon
    QString icon;
    if (presence.type() == QXmppPresence::Available) {
        switch (presence.availableStatusType())
        {
        case QXmppPresence::Online:
        case QXmppPresence::Chat:
            icon = "green";
            break;
        case QXmppPresence::Away:
        case QXmppPresence::XA:
            icon = "orange";
            break;
        case QXmppPresence::DND:
            icon = "red";
            break;
        case QXmppPresence::Invisible:
            icon = "gray";
            break;
        }
    } else {
        icon = "gray";
    }
    if (!icon.isEmpty())
        setIcon(QIcon(":/pixmap/image/"+icon+".png"));
}

void RosterItem::setAvatar(const QImage& image)
{
    setData(QVariant(image), RosterItem::Avatar);
    qDebug()<<"set roster avatar";
}

QImage RosterItem::getAvatar()
{
    return qvariant_cast<QImage>(data(RosterItem::Avatar));
}

RosterItemDelegate::RosterItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    _state =  QStyle::State_Enabled;
}

QSize RosterItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex & index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    if(index.data(Qt::UserRole)==USER_ROLE_GROUP){
        return QSize(20, 20);
    }else{
        return QSize(34, 40);
    }

}
void RosterItemDelegate::paintGroup(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();
    QRect rect = option.rect;
    if(!index.data(Qt::DisplayRole).toString().isEmpty()){
        painter->drawText(rect, index.data(Qt::DisplayRole).toString());
    }else{
        painter->drawText(rect, index.data(RosterItem::BareJid).toString());
    }
    painter->restore();
}
void RosterItemDelegate::paintRoster(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
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
        //qDebug()<<"paint one"<<option.state;
    if (index.data(Qt::UserRole+1).toString() == "1")
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
    if(!index.data(Qt::DisplayRole).toString().isEmpty())
        painter->drawText(rect, index.data(Qt::DisplayRole).toString());
    else
        painter->drawText(rect, index.data(RosterItem::BareJid).toString());

    painter->setPen(statusTextColor);
    rect.setTop(rect.y() + rect.height()/2);
    rect.moveTop(rect.y() - 3);
    QString statusText = index.data(RosterItem::StatusText).toString();
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
    value = index.data(RosterItem::Avatar);
    if(value.type() == QVariant::Image)
    {
        image = qvariant_cast<QImage>(value);
    }

    pixmap = QPixmap(":/pixmap/image/avatar_40.png");
    rect = option.rect;
    rect.setWidth(40);
    rect.setHeight(40);
    rect.moveTo(0,rect.y());
    if(image.isNull())
       painter->drawPixmap(rect, pixmap);
    else
        painter->drawImage(rect, image);

        QStyleOptionButton optButton;
        QRect buttonRect( option.rect);
        buttonRect.setY(option.rect.y()+ 20);
        buttonRect.setX(option.rect.x()+180);
        buttonRect.setHeight(18);
        buttonRect.setWidth(50);
        optButton.state=_state | QStyle::State_Enabled;
        optButton.rect=buttonRect;
        optButton.text=tr("删除");

        QApplication::style()->drawControl(QStyle::CE_PushButton,&optButton,painter,0);
        painter->restore();
}
void RosterItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex &index) const
{
    if(index.data(Qt::UserRole)==USER_ROLE_GROUP){
        paintGroup(painter, option, index);
    }else{
        paintRoster(painter, option, index);
    }
}


bool RosterItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    //qDebug() << "editorEvent::" << event->type();
    if( event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::MouseButtonRelease ||
            event->type() == QEvent::MouseButtonDblClick)
    {
        QRect buttonRect( option.rect);
        buttonRect.setY(option.rect.y()+ 20);
        buttonRect.setX(option.rect.x()+180);
        buttonRect.setHeight( 18);
        buttonRect.setWidth(50);


        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if( !buttonRect.contains( mouseEvent->pos()))
        {
            if(mouseEvent->type()==QEvent::MouseButtonDblClick)
            {
                emit showPersonalChatDlg( index);
            }else if(mouseEvent->type()==QEvent::MouseButtonPress)
            {
                qDebug()<<"MouseButtonPress";
                emit setItemSelected( index);

            }
            _state = QStyle::State_Raised;

            return true;
        }

        if( event->type() == QEvent::MouseButtonPress) {
            _state = QStyle::State_Sunken;
        } else if( event->type() == QEvent::MouseButtonRelease) {
            _state = QStyle::State_Raised;
            emit delButtonClicked( index);
        }
        return true;
    } else {
         _state = QStyle::State_Raised;
        return true;
    }


}
