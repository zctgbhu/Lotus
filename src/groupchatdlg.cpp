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

#include "groupchatdlg.h"
#include "ui_groupchatdlg.h"
#include <QXmppMessage.h>
#include <QXmppClient.h>
#include <QMenu>

GroupChatDlg::GroupChatDlg(QWidget *parent) :
    QDialog(parent),ui(new Ui::GroupChatDlg),
    roomUserModel(this)
{
    ui->setupUi(this);
    dir = QDesktopServices::storageLocation(QDesktopServices::HomeLocation)+"/.Lotus/";
    int ScreenW = QApplication::desktop()->width();
    int ScreenH  = QApplication::desktop()->height();
    move((ScreenW-570)/2,(ScreenH-493)/2);
    setFixedSize(570,493);
    ui->group_memberlist->setModel(&roomUserModel);
    RoomUserItemDelegate *delegate = new RoomUserItemDelegate();
    ui->group_memberlist->setItemDelegate(delegate);
    ui->group_announcement->setContextMenuPolicy(Qt::CustomContextMenu);
    setView();
    //setAttribute(Qt::WA_DeleteOnClose);
}

void GroupChatDlg::refreshFace()
{
    ui->group_announcement->setLineWrapColumnOrWidth(0);
    ui->messageEdit->setLineWrapColumnOrWidth(0);
}

void GroupChatDlg::faceClicked(QString id)
{
    disconnect(facePanel,SIGNAL(faceClicked(QString)),
            this,SLOT(faceClicked(QString)));
    ui->messageEdit->insertHtml("<img src='"+id+"'/>");
}

GroupChatDlg::~GroupChatDlg()
{
    delete ui;
}

void GroupChatDlg::setRoomJid(const QString &RoomJid)
{
    roomJid=RoomJid;
}

void GroupChatDlg::setDisplayName(const QString &DisplayName)
{
    displayName=DisplayName;
    ui->group_name->setText(displayName);
}

void GroupChatDlg::setQXmppClient(QXmppClient *Client)
{
    client=Client;
}

void GroupChatDlg::setQXmppMucRoom(QXmppMucRoom *Room)
{
    room=Room;
    qDebug()<<"setQXmppMucRoom";
    connect(room,SIGNAL(participantChanged(QString) ),
            this,SLOT(participantChanged(QString)));
    connect(room,SIGNAL(participantAdded(QString) ),
            this,SLOT(participantChanged(QString)));
    connect(room,SIGNAL(participantRemoved(QString) ),
            this,SLOT(participantRemoved(QString)));
}

void GroupChatDlg::participantChanged(const QString &jid)
{
    qDebug()<<jid;
    if(!roomUserItem.contains(jid))
    {
        roomUserItem[jid]=new RoomUserItem(jid);
        roomUserModel.appendRow(roomUserItem[jid]);
    }
}

void GroupChatDlg::participantRemoved(const QString &jid)
{
    //m_memberModel.indexFromItem(m_memberMap[jid]).row();
    roomUserModel.removeRow(roomUserModel.indexFromItem(roomUserItem[jid]).row());
    roomUserItem.remove(jid);
}

void GroupChatDlg::messageReceived(const QXmppMessage &msg)
{
//    if(msg.stamp()<=chatsLog->getLastChatTime(roomJid)){
//        qDebug()<<"the message is readed!";
//        return;
//    }
    qDebug()<<"msgtime="<<msg.stamp().toString()<<"||||chatTime="<<chatsLog->getLastChatTime(roomJid).toString();
    QString username=QXmppUtils::jidToUser(QXmppUtils::jidToResource(msg.from()));
    QDateTime dt=msg.stamp().toLocalTime();
    QString msgText;
        if(dt.isNull()){
        msgText=("<font color='blue'>"+username+" </font>"+
                        "<font color='green'>"+
                        QDateTime::currentDateTime().toLocalTime()
                        .toString("yyyy/MM/dd hh:mm:ss")+
                        "</font><font color='black'>"+
                        msg.body()+"</font>");

    }else{
        msgText=("<font color='blue'>"+username+" </font>"+
                                           "<font color='grey'>"+dt.toString("yyyy/MM/dd hh:mm:ss")+
                                           "</font><font color='black'>"+
                                           msg.body()+"</font>");
    }
    ui->group_announcement->append(msgText);
    chatsLog->addChatLog(roomJid,msgText);
}

void GroupChatDlg::sendMessage()
{
    if(ui->messageEdit->toPlainText().isEmpty()){
        return;
    }
    QString message=ui->messageEdit->toHtml();
    room->sendMessage(message);
    qDebug() <<"messageEdit"<< message;
    ui->messageEdit->clear();
}

QString GroupChatDlg::getDisplayName() const
{
    return displayName;
}

void GroupChatDlg::setChatsLog(ChatsLog *chatslog)
{
    chatsLog=chatslog;
}

void GroupChatDlg::setFacePanel(FacePanel *facepanel)
{
    facePanel=facepanel;
}

void GroupChatDlg::setAnimationUrls(QHash<QMovie *, QUrl> &Urls)
{
    urls=&Urls;
    ui->group_announcement->addAnimation(urls);
    ui->messageEdit->addAnimation(urls);
    ui->log->addAnimation(urls);
}

void GroupChatDlg::on_closeButton_clicked()
{
    close();
}

void GroupChatDlg::on_sendButton_clicked()
{
    sendMessage();
}

void GroupChatDlg::on_group_announcement_customContextMenuRequested(const QPoint &pos)
{
    static QMenu *cmenu=NULL;
    if(cmenu==NULL){
        cmenu=new QMenu(ui->group_announcement);
        QAction *selecteAll = cmenu->addAction("全选");
        cmenu->addSeparator();
        QAction *copyText=cmenu->addAction("复制");
        QAction *clearAll = cmenu->addAction("清空");
        connect(selecteAll,SIGNAL(triggered()),ui->group_announcement,SLOT(selectAll()));
        connect(copyText,SIGNAL(triggered()),ui->group_announcement,SLOT(copy()));
        connect(clearAll,SIGNAL(triggered()),ui->group_announcement,SLOT(clear()));
    }
    cmenu->exec(QCursor::pos());//在当前鼠标位置显示
}

void GroupChatDlg::on_face_clicked()
{
    int x= this->x();
    int y= this->y();
    facePanel->move(x-150,y+115);
    connect(facePanel,SIGNAL(faceClicked(QString)),
            this,SLOT(faceClicked(QString)));
    facePanel->show();
}

void GroupChatDlg::on_showChatLog_clicked(bool checked)
{
    qDebug()<<"on_showChatLog_clicked";
    if(checked)
    {
        setFixedSize(1000,493);
        ui->log->start();
        ui->log->append(chatsLog->getChatLog(roomJid));
    }else{
        setFixedSize(570,493);
        ui->log->clear();
        ui->log->stop();
    }
}
