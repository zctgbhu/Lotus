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

#ifndef PERSONALCHATDLG_H
#define PERSONALCHATDLG_H

#include <QDialog>
#include"QXmppClient.h"
#include "QXmppRosterManager.h"
#include "QXmppTransferManager.h"
#include <qfiledialog.h>
#include "uservcard.h"
#include <QStyle>
#include <QWidget>
#include <QDesktopWidget>
#include "QXmppMessage.h"
#include <QMessageBox>
#include <QMenu>
#include "facepanel.h"
#include <QMouseEvent>
#include "chatslog.h"
namespace Ui {
class PersonalChatDlg;
}

class PersonalChatDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit PersonalChatDlg(QWidget *parent = 0);
    ~PersonalChatDlg();
    
    void setQXmppClient(QXmppClient *Client);
    void setDisplayName(const QString &DisplayName);
    void setBareJid(const QString &BareJid);
    void setFriendSign(const QString &FriendSign);
    QString getBareJid() const;
    QString getDisplayName() const;
    void sendMessage();
    void messageReceived(const QXmppMessage &msg);

    void setJob(QXmppTransferJob *Job);
    void setAnimationUrls(QHash<QMovie*, QUrl> &Urls);
    void setFriendAvatar(const QImage &avatar);
    void setOwnAvatar(const QImage &avatar);
    void setFacePanel(FacePanel *facepanel);
    void setChatsLog(ChatsLog *chatslog);
private slots:
    void on_closeButton_clicked();
    void on_sendMessageButton_clicked();
    void on_sendfileButton_clicked();
    void on_saveFileButton_clicked();
    void on_refuseButton_clicked();

    void slotFinished();
    void slotProgress(qint64 done, qint64 total);
    void slotError(QXmppTransferJob::Error error);
    void on_announcement_customContextMenuRequested(const QPoint &pos);

    void on_face_clicked();
    void faceClicked(QString id);
    void on_chatLogButton_clicked(bool checked);

private:
    Ui::PersonalChatDlg *ui;
    QXmppClient* client;
    QString bareJid;
    QString displayName;
    QStringList resourcesList;
    void setResourcesList();
    bool isClientSet;
    QXmppTransferJob *sendFileJob;
    void setView();
    QPoint dragPos;
    QXmppTransferJob *job;
    FacePanel *facePanel;
    ChatsLog *chatsLog;
    QHash<QMovie*, QUrl> *urls;
    QString dir;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
signals:
    void closePersonalchatdlg(QString &);
};

#endif // PERSONALCHATDLG_H
