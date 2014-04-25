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

#ifndef LOTUS_H
#define LOTUS_H

#include <QWidget>
#include "tablabel.h"
#include "QXmppClient.h"
#include "rosterItemModel.h"
#include "roomitemmodel.h"
#include "personalchatdlg.h"
#include "rosterItem.h"
#include "roomitem.h"
#include "QXmppMessage.h"
#include "searchdlg.h"
#include "groupchatdlg.h"
#include "QXmppVCardManager.h"
#include "QXmppVCardIq.h"
#include "uservcard.h"
#include "rosteritemsortfilter.h"
#include<QSortFilterProxyModel>
#include "aboutdialog.h"
#include <QSystemTrayIcon>
#include <QDropEvent>
#include "roomfavoritecache.h"
#include "chatslog.h"
namespace Ui {
class Lotus;
}

class Lotus : public QWidget
{
    Q_OBJECT
    
public:
    explicit Lotus(QWidget *parent = 0);
    ~Lotus();
    void setFriendGroupOtherList();
    void setClient(QXmppClient *sClient);
    void init();
    PersonalChatDlg* getChatDialog(const QString& bareJid);
    void showChatDialog(const QString &bareJid);
    GroupChatDlg *getGroupChatDialog(const QString &roomJid);

    UserVCard *getOrCreateUserVCardFromBareJid(const QString &bareJid, const QXmppVCardIq &vCard);
    void avatarChanged(QImage image);
    FacePanel facepanel;
    ChatsLog chatsLog;
private:
    Ui::Lotus *ui;
    TabLabel *currenttab;
    QSignalMapper *m_pSignalMapper;
    QPropertyAnimation *m_pProAnima;
    QPropertyAnimation *m_gProAnima;
    QPropertyAnimation *m_tProAnima;
    QXmppClient *client;
    QSortFilterProxyModel rosterItemModelProxy;
    RosterItemModel rosterItemModel;
    RoomItemModel roomItemModel;
    RosterItemSortFilter rosterItemSortFilter;
    QXmppTransferManager xmppTransferManager;
    QXmppDiscoveryManager xmppDiscoveryManager;
    QXmppMucManager xmppMucManager;
    QImage ownAvatar;
    QString ownBareJid;
    QString delBareJid;
    UserVCard* ownVCard;
    QMap<QString, PersonalChatDlg*> personalChatDlgsList;
    QMap<QString, GroupChatDlg*> roomChatDlgsList;
    QMap<QString, QXmppMucRoom*> roomList;
    QMap<QString, UserVCard*> userVCardList;
    QMap<QString, QAction*> messageBoxActions;
    QMultiMap<QString,QXmppMessage> messageBox;
    PersonalChatDlg personalChatDlg;
    SearchDlg searchDlg;
    AboutDialog aboutDlg;
    QSystemTrayIcon *systemTrayIcon;
    QTimer *trayTimer;
    bool trayIconStatus;
    QAction *displayAction;
    QAction *aboutAction;
    QAction *exitAction;
    //QList<QAction*> messageBoxAction;
    QMenu myMenu;

    QPoint dragPos;
    QString selectedBareJid;
    QString selectedBareJid_pre;
    QString selectedRoomJid;
    QString selectedRoomJid_pre;
    RoomFavoriteCache roomFavoriteCache;
    QStringList roomFavoriteList;
    QString dir;
    QString ownStatusText;
    QString unSubscribeTmp;
    QHash<QMovie*, QUrl> urls;
    int lotusWidth;
    int lotusHeight;
    int lotusStatus;
    bool topHide;
    bool rightHide;
    QPropertyAnimation *topHideAnimation;
    QPropertyAnimation *rightHideAnimation;
    void setView();

    void retrunLoginwin();
    void getFaceAnimation();
    void statusTextChanged(const QString &statusText);

public slots:
    void setList();
    void SetAnimation(int nCurveType);
    void rosterReceived();
    void rosterChanged(const QString &bareJid);
    void messageAction(QAction *action);
    void showPersonalChatDlg(const QModelIndex &index);
    void messageReceived(const QXmppMessage &msg);
    void fileReceived(QXmppTransferJob *job);
    void joinRoom(const QString &roomJid);
    void quitRoom(const QString &roomJid);
    void showGroupChatDlg(const QModelIndex &index);
    void vCardReceived(const QXmppVCardIq &vCard);
    void clientVCardReceived();
    void updateAvatar();
    void delButtonClicked(const QModelIndex &index);
    void presenceReceived(const QXmppPresence& qXmppPresence);
    void filterChanged(const QString &filter);
    void presenceChanged(const QString &bareJid, const QString &resource);
    void deleteGroupChatDialog(QString roomJid);
    void clickAboutButton();
    void exitEvent();
private slots:
    void on_searchButton_clicked();
    void trayMessage();
    void setItemSelected(const QModelIndex &index);
    void favoriteButtonClicked(const QModelIndex &index);
    void setRoomSelected(const QModelIndex &index);
    void error(QXmppClient::Error e);
    void favoriteButtonEnter();
    void closePersonalchatdlg(QString &BareJid);
    void on_statusText_editingFinished();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
signals:
    void clickAvatarLabel();
    void retrunLoginWin();
};

#endif // LOTUS_H
