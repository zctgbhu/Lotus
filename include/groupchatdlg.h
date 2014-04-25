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

#ifndef GROUPCHATDLG_H
#define GROUPCHATDLG_H

#include <QDialog>
#include "QXmppMucManager.h"
#include "roomitemmodel.h"
#include "qdebug.h"
#include "roomuseritem.h"
#include <QDesktopWidget>
#include "facepanel.h"
#include "QMouseEvent"
#include <QTimer>
#include "chatslog.h"
namespace Ui {
class GroupChatDlg;
}

class GroupChatDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit GroupChatDlg(QWidget *parent = 0);
    ~GroupChatDlg();
    
    void setRoomJid(const QString& RoomJid);
    void setDisplayName(const QString& DisplayName);
    void setQXmppClient(QXmppClient* Client);
    void setQXmppMucRoom(QXmppMucRoom* Room);
    void messageReceived(const QXmppMessage &msg);
    void sendMessage();
    QString getDisplayName() const;
    void setChatsLog(ChatsLog *chatslog);
    void setFacePanel(FacePanel *facepanel);
    void setAnimationUrls(QHash<QMovie*, QUrl> &Urls);
private slots:
    void on_closeButton_clicked();
    void participantChanged(const QString &jid);
    void on_sendButton_clicked();
    void participantRemoved(const QString &jid);
    void on_group_announcement_customContextMenuRequested(const QPoint &pos);

    void on_face_clicked();

    void faceClicked(QString id);
    void refreshFace();
    void on_showChatLog_clicked(bool checked);

private:
    Ui::GroupChatDlg *ui;

    QString roomJid;
    QString displayName;
    QXmppClient* client;
    QXmppMucRoom* room;
    QStandardItemModel roomUserModel;
    QMap<QString, RoomUserItem*> roomUserItem;
    void setView();
    QPoint dragPos;
    QString dir;
    FacePanel *facePanel;
    QHash<QMovie*, QUrl> *urls;
    QTimer timer;
    ChatsLog *chatsLog;
    //QString dir;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
signals:
    void closeGroupChatDlg(QString);
};

#endif // GROUPCHATDLG_H
