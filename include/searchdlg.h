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

#ifndef SEARCHDLG_H
#define SEARCHDLG_H
#include <QWidget>
#include <QDialog>
#include <QInputDialog>
#include "QXmppClient.h"
#include "QXmppDiscoveryManager.h"
#include <QStandardItemModel>
#include "QXmppMucManager.h"
#include "QXmppRosterManager.h"
#include <QMouseEvent>
#include <QTimer>
#include <QNoDebug>
namespace Ui {
class SearchDlg;
}

class SearchDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit SearchDlg(QWidget *parent = 0);
    ~SearchDlg();
    void setClient(QXmppClient *Client);
    bool isValidBareJid(const QString &bareJid);
    QXmppIq serachUser(const QString &searchText);
public slots:
    void iqReceived(QXmppIq iq);
    void setSelectSearchItem(const QModelIndex &index);
private slots:
    void on_addFriendButton_clicked();
    void on_closeFriendTabButton_clicked();
    void on_closeRoomTabButton_clicked();
    void on_refreshButton_clicked();
    void setSelectRoom(const QModelIndex &index);
    void itemsReceived(const QXmppDiscoveryIq &qxmppdisiq);
    void on_joinRoomButton_clicked();
    void on_sarchUserButton_clicked();
    void on_friendTabButton_clicked();
    void on_roomTabButton_clicked();
    void refreshRoomTimeout();
    void searchUserTimeout();
signals:
    void joinRoom(const QString &roomJid);
private:
    Ui::SearchDlg *ui;
    QXmppClient *client;
    QStandardItemModel itemRoomModel;
    QStandardItemModel itemSearchModel;
    QXmppMucRoom *room;
    QXmppDiscoveryManager* ext;
    QXmppRosterManager *rosterManager;
    QString selectRoomJid;
    QString selectUserJid;
    bool isClient;
    QString searchId;
    QPoint dragPos;
    bool isRefreshing;
    bool isSearching;
    QTimer refreshTimer;
    QTimer searchTimer;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

};

#endif // SEARCHDLG_H
