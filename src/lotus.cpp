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

#include "lotus.h"
#include "ui_lotus.h"
#include "QXmppUtils.h"
#include <QApplication>
#include <QDesktopWidget>
extern QString ServerAddress;
Lotus::Lotus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Lotus),rosterItemModel(this),roomItemModel(this),rosterItemSortFilter(this)
{
    ownVCard=NULL;
    ui->setupUi(this);
    ui->avatar->installEventFilter(this);
    dir = QDesktopServices::storageLocation(QDesktopServices::HomeLocation)+"/.Lotus/";
    currenttab = new TabLabel;
    currenttab = ui->friendLabel;
    setFriendGroupOtherList();
    //TO FIX:系统托盘图标无法响应鼠标点击事件，unity中会直接显示菜单。希望实现点击右键为菜单，双击左键可以还原程序主界面
    systemTrayIcon=new QSystemTrayIcon(this);
    systemTrayIcon->setIcon(QIcon(":/pixmap/image/tray.ico"));
    displayAction  = new QAction(tr("显示"),this);
    connect(displayAction,SIGNAL(triggered()),this,SLOT(showNormal()));
    aboutAction  = new QAction(tr("关于"),this);
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(clickAboutButton()));
    exitAction  = new QAction(tr("退出"),this);
    connect(exitAction,SIGNAL(triggered()),this,SLOT(exitEvent()));
    //messageBoxAction.append(new QAction(tr("未读消息"),this));
    myMenu.addAction(displayAction);
    myMenu.addSeparator();
    //myMenu.addActions(messageBoxAction);
    myMenu.addSeparator();
    myMenu.addAction(aboutAction);
    myMenu.addAction(exitAction);
    connect(&myMenu,SIGNAL(triggered(QAction*)),
            this,SLOT(messageAction(QAction*)));
    systemTrayIcon->setContextMenu(&myMenu);

    //systemTrayIcon->installEventFilter(this);
    systemTrayIcon->show();
    trayTimer= new QTimer(this);
    trayTimer->setInterval(500);
    trayIconStatus=true;
    connect(trayTimer,SIGNAL(timeout()),
            this,SLOT(trayMessage()));
    rosterItemModelProxy.setSourceModel(&rosterItemModel);
    rosterItemModelProxy.setSortRole(RosterItem::PresenceType);
    //rosterItemModelProxy.setDynamicSortFilter(true);

    ui->friendTree->setModel(&rosterItemModelProxy);
    ui->friendTree->setHeaderHidden(TRUE);
    RosterItemDelegate *rosterdelegate = new RosterItemDelegate();
    ui->friendTree->setItemDelegate(rosterdelegate);
    ui->roomList->setModel(&roomItemModel);
    RoomItemDelegate *roomDelegate = new RoomItemDelegate();
    ui->roomList->setItemDelegate(roomDelegate);
    connect(rosterdelegate,SIGNAL(delButtonClicked(QModelIndex)),
            this,SLOT(delButtonClicked(QModelIndex)));
    connect(rosterdelegate,SIGNAL(showPersonalChatDlg(QModelIndex)),
            this,SLOT(showPersonalChatDlg(QModelIndex)));
    connect(rosterdelegate,SIGNAL(setItemSelected(QModelIndex)),
            this,SLOT(setItemSelected(QModelIndex)));
    connect(ui->search,SIGNAL(textChanged(QString)),
            this,SLOT(filterChanged(QString)));
    connect(roomDelegate, SIGNAL(showGroupChatDlg(QModelIndex)),
            this, SLOT(showGroupChatDlg(QModelIndex)));
    connect(roomDelegate, SIGNAL(favoriteButtonClicked(QModelIndex)),
            this, SLOT(favoriteButtonClicked(QModelIndex)));
    connect(roomDelegate, SIGNAL(setItemSelected(QModelIndex)),
            this, SLOT(setRoomSelected(QModelIndex)));
    connect(roomDelegate, SIGNAL(favoriteButtonEnter()),
            this, SLOT(favoriteButtonEnter()));
    connect(&searchDlg, SIGNAL(joinRoom(QString)),
            this,SLOT(joinRoom(QString)));
    connect(this,SIGNAL(clickAvatarLabel()),
            this,SLOT(updateAvatar()));

    setView();

    ui->roomList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->roomList->setDragDropMode(QAbstractItemView::DragDrop);
    ui->roomList->setDragEnabled(true);
    ui->roomList->setAcceptDrops(true);
    ui->roomList->setDropIndicatorShown(true);
    connect(&roomItemModel,SIGNAL(quitRoom(QString)),this,SLOT(quitRoom(QString)));

    this->installEventFilter(this);
    ui->searchButton->installEventFilter(this);
    ui->btn_close->installEventFilter(this);
    ui->btn_min->installEventFilter(this);
    ui->aboutButton->installEventFilter(this);

    ownAvatar=QImage(":/pixmap/image/avatar.png");
    selectedBareJid_pre="0";
    selectedRoomJid_pre="0";
    getFaceAnimation();

}

void Lotus::getFaceAnimation()
{
    for(int i=0;i<105;i++)
    {
        QMovie* movie = new QMovie(this);
        movie->setFileName(dir+"face/"+QString::number(i)+".gif");
        urls.insert(movie, QUrl(QString::number(i)));
        movie->start();
    }
}

void Lotus::favoriteButtonEnter()
{
    setStatusTip("~~~~~~~~~~");
}

void Lotus::setRoomSelected(const QModelIndex &index)
{
    selectedRoomJid=index.data(RoomItem::RoomJid).toString();
    roomItemModel.getRoomItemFromBareJid(selectedRoomJid)
            ->setData("1",RoomItem::ItemSelected);
    if(selectedRoomJid_pre!="0")
    {
        RoomItem *item=roomItemModel.getRoomItemFromBareJid(selectedRoomJid_pre);
        if(item!=NULL){
            item->setData("0",RoomItem::ItemSelected);
        }
    }
    selectedRoomJid_pre=selectedRoomJid;
}

void Lotus::favoriteButtonClicked(const QModelIndex &index)
{
    QString roomJid = index.data(RoomItem::RoomJid).toString();
    if(index.data(RoomItem::ItemFavorite).toString()=="0")
    {
        roomItemModel.getRoomItemFromBareJid(roomJid)
                ->setData("1",RoomItem::ItemFavorite);
        roomFavoriteCache.addRoomFavorite(roomJid);
    }else{
        roomItemModel.getRoomItemFromBareJid(roomJid)
                ->setData("0",RoomItem::ItemFavorite);
        roomFavoriteCache.removeRoomFavorite(roomJid);
    }
}

void Lotus::filterChanged(const QString& filter)
{
    rosterItemSortFilter.setFilterRegExp(filter);
}

void Lotus::updateAvatar()
{
    QString fileFilters = QString("Images (*.png *.jpeg *.jpg *.gif *.bmp);;All Files (*.*)");
    QString file = QFileDialog::getOpenFileName(this, tr("选择头像图片"),
                                                QString(), fileFilters);
    if(file.isEmpty())
        return;

    QImage image;
    if(image.load(file))
    {
        QImage scaled = image.scaled(QSize(96, 96), Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation);
        avatarChanged(scaled);
    }
    else
        QMessageBox::information(this, tr("头像更新"), tr("图像读取错误"));
}

void Lotus::delButtonClicked(const QModelIndex &index)
{
    qDebug()<<"dsadsadsa";
    delBareJid = index.data(RosterItem::BareJid).toString();


    int answer = QMessageBox::question(this, tr("删除好友"),
                                       QString(tr("你确定删除好友： ")+
                                               "<font color=\"red\">%1</font>").arg(delBareJid),
                                       QMessageBox::Yes, QMessageBox::No);
    if(answer == QMessageBox::Yes)
    {
        QXmppRosterIq remove;
        remove.setType(QXmppIq::Set);
        QXmppRosterIq::Item itemRemove;
        itemRemove.setSubscriptionType(QXmppRosterIq::Item::Remove);
        itemRemove.setBareJid(delBareJid);
        remove.addItem(itemRemove);
        client->sendPacket(remove);

        rosterItemModel.removeRosterEntry(delBareJid);
    }
}

void Lotus::avatarChanged(QImage image)
{
    QXmppVCardIq vcard;
    vcard.setType(QXmppIq::Set);

    QByteArray ba;
    QBuffer buffer(&ba);
    if(buffer.open(QIODevice::WriteOnly))
    {
        if(image.save(&buffer, "PNG"))
        {
            vcard.setPhoto(ba);
            client->sendPacket(vcard);
            ui->avatar->setPixmap(QPixmap::fromImage(image));

            //m_vCardCache.getVCard(m_xmppClient.configuration().jidBare()) = vcard;
            // update photo hash
            //QXmppPresence presence = client.clientPresence();
            //addPhotoHash(presence);
            //client.setClientPresence(presence);
        }
    }
}

void Lotus::joinRoom(const QString &roomJid){
    qDebug()<<"joinroom"<<roomJid;
    if(!roomList.contains(roomJid)){
        roomList[roomJid]=xmppMucManager.addRoom(roomJid);
        roomList[roomJid]->setNickName(ownBareJid);
        qDebug()<<"getGroupChatDialog"<<roomJid;

        roomList[roomJid]->join();
        getGroupChatDialog(roomJid);
    }
    if(roomItemModel.updateRoomList(roomJid)){
        QMessageBox::information(NULL,QString::fromUtf8("错误") ,QString::fromUtf8("此聊天室已在列表中"));
    }
    if(roomFavoriteCache.getRoomFavoriteList().contains(roomJid))
        roomItemModel.getRoomItemFromBareJid(roomJid)->setData("1",RoomItem::ItemFavorite);
}

void Lotus::quitRoom(const QString &roomJid)
{
    if(QMessageBox::information(this,"提示","退出聊天室 "+roomJid+" ?",
                                QMessageBox::Yes,QMessageBox::No)==
            QMessageBox::No)
        return;
    if(roomList.contains(roomJid)){
        qDebug()<<"quitRoom"<<roomJid;
        roomList[roomJid]->leave();
        xmppMucManager.rooms().removeAll(roomList[roomJid]);
        delete roomList[roomJid];
        roomList.remove(roomJid);
        deleteGroupChatDialog(roomJid);
    }
    if(roomItemModel.removeRoom(roomJid)){
        QMessageBox::information(this,tr("错误"),"您未加入 "+roomJid);
    }
}

void Lotus::showGroupChatDlg(const QModelIndex& index)
{
    QString roomJid = index.data(RoomItem::RoomJid).toString();
    if(!roomJid.isEmpty())
        getGroupChatDialog(roomJid)->show();
    GroupChatDlg *dialog = roomChatDlgsList[roomJid];
    if (dialog) {

        for(int i=messageBox.values(roomJid).size()-1;i>=0;i--){

            dialog->messageReceived(messageBox.values(roomJid).takeAt(i));
        }
//        foreach (QXmppMessage msg, messageBox.values(roomJid)) {
//            dialog->messageReceived(msg);
//        }

        messageBox.remove(roomJid);
        myMenu.removeAction(messageBoxActions[roomJid]);
        messageBoxActions.remove(roomJid);
    }
    if(messageBox.empty()){
        trayTimer->stop();
        systemTrayIcon->setIcon(QIcon(":/pixmap/image/tray.ico"));
    }
}

GroupChatDlg* Lotus::getGroupChatDialog(const QString& roomJid)
{
    if(!roomChatDlgsList.contains(roomJid)){
        qDebug()<<"new roomChatDlgs";
        roomChatDlgsList[roomJid] = new GroupChatDlg();
        roomChatDlgsList[roomJid]->setRoomJid(roomJid);
        roomChatDlgsList[roomJid]->setDisplayName(roomJid);
        roomChatDlgsList[roomJid]->setQXmppClient(client);
        roomChatDlgsList[roomJid]->setQXmppMucRoom(roomList[roomJid]);
        roomChatDlgsList[roomJid]->setAnimationUrls(urls);
        roomChatDlgsList[roomJid]->setFacePanel(&facepanel);
        roomChatDlgsList[roomJid]->setChatsLog(&chatsLog);
        connect(roomChatDlgsList[roomJid],SIGNAL(closeGroupChatDlg(QString)),
                this,SLOT(deleteGroupChatDialog(QString)));

    }
    qDebug()<<"return roomChatDlg";
    return roomChatDlgsList[roomJid];
}

void Lotus::deleteGroupChatDialog(QString roomJid)
{
    if(roomChatDlgsList.contains(roomJid)){
        disconnect(roomChatDlgsList[roomJid],SIGNAL(closeGroupChatDlg(QString)),
                   this,SLOT(deleteGroupChatDialog(QString)));
        delete roomChatDlgsList[roomJid];
        roomChatDlgsList.remove(roomJid);
        qDebug()<<"remove roomChatDlgs";
    }else{
        qDebug()<<"should never come here!!!!";
    }
}

void Lotus::exitEvent()
{
    if(QMessageBox::information(this,tr("提示"),tr("确认退出程序?"),
                                QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
        exit(0);
}

void Lotus::setItemSelected(const QModelIndex& index)
{
    if(index.data(Qt::UserRole)!=USER_ROLE_GROUP)
    {

        selectedBareJid = index.data(RosterItem::BareJid).toString();
        rosterItemModel.getRosterItemFromBareJid(selectedBareJid)->setData("1",Qt::UserRole+1);
        qDebug()<<"selectedBareJid_pre"<<selectedBareJid_pre;
        if(selectedBareJid_pre!="0"){
            rosterItemModel.getRosterItemFromBareJid(selectedBareJid_pre)->setData("0",Qt::UserRole+1);
        }
        selectedBareJid_pre=selectedBareJid;
    }
}

void Lotus::showPersonalChatDlg(const QModelIndex& index)
{
    if(index.data(Qt::UserRole)!=USER_ROLE_GROUP)
    {
        qDebug() << "showPersonalChatDlg";
        QString bareJid = index.data(RosterItem::BareJid).toString();
        showChatDialog(bareJid);

        PersonalChatDlg *dialog =personalChatDlgsList[bareJid];
        if (dialog) {
            for(int i=messageBox.values(bareJid).size()-1;i>=0;i--){

                dialog->messageReceived(messageBox.values(bareJid).takeAt(i));
            }
//            foreach (QXmppMessage msg, messageBox.values(bareJid)) {
//                dialog->messageReceived(msg);
//            }

            messageBox.remove(bareJid);
            myMenu.removeAction(messageBoxActions[bareJid]);
            messageBoxActions.remove(bareJid);
        }
        if(messageBox.empty()){
            trayTimer->stop();
            systemTrayIcon->setIcon(QIcon(":/pixmap/image/tray.ico"));
        }
    }
}

void Lotus::showChatDialog(const QString& bareJid)
{
    qDebug() << "showChatDialog " << bareJid;
    if(!bareJid.isEmpty()){
        getChatDialog(bareJid)->show();
    }
}

PersonalChatDlg* Lotus::getChatDialog(const QString& bareJid)
{
    if(!personalChatDlgsList.contains(bareJid))
    {
        personalChatDlgsList[bareJid] = new PersonalChatDlg();
        personalChatDlgsList[bareJid]->setBareJid(bareJid);
        personalChatDlgsList[bareJid]->setAnimationUrls(urls);
        personalChatDlgsList[bareJid]->setFacePanel(&facepanel);
        personalChatDlgsList[bareJid]->setChatsLog(&chatsLog);
        connect(personalChatDlgsList[bareJid],SIGNAL(closePersonalchatdlg(QString&)),
                this,SLOT(closePersonalchatdlg(QString&)));
        RosterItem *tmp=rosterItemModel.getRosterItemFromBareJid(bareJid);
        personalChatDlgsList[bareJid]->setFriendSign(tmp->data(RosterItem::StatusText).toString());
        QImage friendAvatar=tmp->getAvatar();
        personalChatDlgsList[bareJid]->setFriendAvatar(friendAvatar);
        personalChatDlgsList[bareJid]->setOwnAvatar(ownAvatar);
        if(!rosterItemModel.getRosterItemFromBareJid(bareJid))
            return 0;

        if(!rosterItemModel.getRosterItemFromBareJid(bareJid)->
                getName().isEmpty())
            personalChatDlgsList[bareJid]->setDisplayName(rosterItemModel.
                                                          getRosterItemFromBareJid(bareJid)->getName());
        else
            personalChatDlgsList[bareJid]->setDisplayName(QXmppUtils::jidToUser(bareJid));

        personalChatDlgsList[bareJid]->setQXmppClient(client);
    }

    return personalChatDlgsList[bareJid];
}

void Lotus::closePersonalchatdlg(QString &BareJid)
{
    disconnect(personalChatDlgsList[BareJid],SIGNAL(closePersonalchatdlg(QString&)),
               this,SLOT(closePersonalchatdlg(QString&)));
    if(personalChatDlgsList.contains(BareJid)){
        //delete personalChatDlgsList[BareJid];
        personalChatDlgsList.remove(BareJid);
    }

    //qDebug()<<"closePersonalchatdlg="<<BareJid;
}

void Lotus::init()
{
    connect(client,SIGNAL(error(QXmppClient::Error)),
            this,SLOT(error(QXmppClient::Error)));
    connect(&client->rosterManager(),SIGNAL(rosterReceived()),
            this, SLOT(rosterReceived()));
    connect(&client->rosterManager(),SIGNAL(itemChanged(QString)),
            this, SLOT(rosterChanged(QString)));
    connect(client,SIGNAL(presenceReceived(QXmppPresence)),
            this,SLOT(presenceReceived(QXmppPresence)));
//    connect(&client->rosterManager(),SIGNAL(subscriptionReceived(QString)),
//            this, SLOT(subscriptionReceived(QString)));
    connect(&client->rosterManager(),SIGNAL(presenceChanged(QString,QString)),
            this, SLOT(presenceChanged(QString,QString)));
    connect(&client->vCardManager(), SIGNAL(vCardReceived(QXmppVCardIq)),
            SLOT(vCardReceived(QXmppVCardIq)));
    connect(&client->vCardManager(), SIGNAL(clientVCardReceived()),
            SLOT(clientVCardReceived()));
    connect(client, SIGNAL(messageReceived(QXmppMessage)),
            this,SLOT(messageReceived(QXmppMessage)));
    xmppTransferManager.setProxy("proxy."+ServerAddress);
    xmppTransferManager.setSupportedMethods(QXmppTransferJob::AnyMethod);
    client->addExtension(&xmppTransferManager);
    client->addExtension(&xmppDiscoveryManager);
    client->addExtension(&xmppMucManager);
    connect(&xmppTransferManager,SIGNAL(fileReceived(QXmppTransferJob*)),
            this,SLOT(fileReceived(QXmppTransferJob*)));

    client->vCardManager().requestClientVCard();
    ownBareJid = client->configuration().jidBare();
    chatsLog.setUserName(ownBareJid);
    ui->avatar->setScaledContents(true);
    roomFavoriteCache.loadFromFile(ownBareJid);
    roomFavoriteList = roomFavoriteCache.getRoomFavoriteList();
    if(!roomFavoriteList.isEmpty())
    {
        foreach (QString roomJid, roomFavoriteList) {
            qDebug()<<"roomFavoriteList="<<roomJid;
            joinRoom(roomJid);
            //roomItemModel.getRoomItemFromBareJid(roomJid)->setData("1",RoomItem::ItemFavorite);
        }
    }
    ownStatusText=roomFavoriteCache.getStatusText();
    if(!ownStatusText.isEmpty())
    {
        ui->statusText->setText(ownStatusText);
        statusTextChanged(ownStatusText);
    }
}

void Lotus::retrunLoginwin()
{
    disconnect(client,SIGNAL(error(QXmppClient::Error)),this,SLOT(error(QXmppClient::Error)));
    client->disconnectFromServer();
    emit retrunLoginWin();
    close();
}

void Lotus::error(QXmppClient::Error e)
{
    if(e==QXmppClient::SocketError){
        retrunLoginwin();
        QMessageBox::warning(this,tr("连接错误!"),tr("连接断开!"),
                             QMessageBox::Ok);

        return;
    }else if(e==QXmppClient::KeepAliveError){
        QMessageBox::warning(this,tr("连接错误!"),tr("网络连接错误! 请检查网络状态并重新登录"),
                             QMessageBox::Ok);
    }else if(e==QXmppClient::XmppStreamError){
        retrunLoginwin();
        QMessageBox::warning(this,tr("连接错误!"),tr("服务器断开连接！"),
                             QMessageBox::Ok);

        return;
    }else{
        qDebug()<<"unknown error";
    }
    retrunLoginwin();
}

void Lotus::presenceChanged(const QString& bareJid, const QString& resource)
{
    if(bareJid == ownBareJid)
        return;
    if(!rosterItemModel.getRosterItemFromBareJid(bareJid))
        return;
    QXmppPresence presence = client->rosterManager().getPresence(bareJid,resource);
    rosterItemModel.updatePresence(bareJid, presence);
    rosterItemModelProxy.sort(0);
}

void Lotus::presenceReceived(const QXmppPresence& qXmppPresence)
{   //TODO：不使用subscriptionReceived信号槽，暂用QInputDialog代替QMessageBox
    const QString jid = qXmppPresence.from();
    const QString bareJid = QXmppUtils::jidToBareJid(jid);
    if(qXmppPresence.type()==QXmppPresence::Subscribe){

        if(rosterItemModel.getRosterItemFromBareJid(bareJid))
        {
            client->rosterManager().acceptSubscription(bareJid);
            QMessageBox::about(NULL, "成功添加好友", QString("<font color=\"red\">%1</font>"+tr("已成为你的好友\n验证信息：<font color=\"red\">%2</font> "))
                                                       .arg(bareJid).arg(qXmppPresence.statusText()));
            return;
        }
        QInputDialog inputDialog;
        inputDialog.setOkButtonText(tr("同意"));
        inputDialog.setCancelButtonText(tr("拒绝"));
        inputDialog.setWindowTitle(tr("申请成为好友"));
        inputDialog.setLabelText(QString("<font color=\"red\">%1</font>"+tr("想和你成为好友，是否同意？\n验证信息：<font color=\"red\">%2</font> "))
                                 .arg(bareJid).arg(qXmppPresence.statusText()));
        inputDialog.setTextValue(tr("验证信息"));

        if(inputDialog.exec()){
            client->rosterManager().acceptSubscription(bareJid);
            client->rosterManager().subscribe(bareJid,inputDialog.textValue());
        }else{
            client->rosterManager().refuseSubscription(bareJid,inputDialog.textValue());
        }
    }else if(qXmppPresence.type()==QXmppPresence::Unsubscribe){
        unSubscribeTmp=bareJid;
    }else if (qXmppPresence.type()==QXmppPresence::Unsubscribed) {
        if(unSubscribeTmp==bareJid){
            QMessageBox::about(NULL, "你被好友删除", QString("<font color=\"red\">%1</font>"+tr("将你从好友列表中删除！"))
                                                       .arg(bareJid));
            return;
        }
        QMessageBox::about(NULL, "好友申请被拒", QString("<font color=\"red\">%1</font>"+tr("拒绝了你的好友申请！\n验证信息：<font color=\"red\">%2</font> "))
                                                   .arg(bareJid).arg(qXmppPresence.statusText()));
        return;
    }

}

void Lotus::fileReceived(QXmppTransferJob *job){
    qDebug()<<"~~~~~~~~~~~~~~~~~~fileReceived~~~~~~~~~~~~~~~~~~~~~~~";
    bool check;
    Q_UNUSED(check);

    qDebug() << "Got transfer request from:" << job->jid();
    PersonalChatDlg *dialog = getChatDialog(QXmppUtils::jidToBareJid(job->jid()));
    if (dialog) {
        dialog->show();
        dialog->setJob(job);
    }
}
void Lotus::clientVCardReceived()
{

    ownVCard= getOrCreateUserVCardFromBareJid(ownBareJid,client->vCardManager().clientVCard());

    qDebug()<<"clientVCardReceived set own avatar!!!!!";
    if(!ownVCard->getAvatar().isNull()){
        ui->avatar->setPixmap(QPixmap::fromImage(ownVCard->getAvatar()));
        qDebug()<<"clientVCardReceived getAvatar()!=Null";
        ownAvatar=ownVCard->getAvatar();
    }
    QStringList userinfo=ownBareJid.split("@");
    QString username=userinfo.at(0);
    QString servername="@"+userinfo.at(1);
    if(username.length()>10){
        username=username.left(10)+"...";
    }
    if(servername.length()>17){
        servername=servername.left(17)+"...";
    }
    ui->username->setText(username);
    ui->servername->setText(servername);
}

void Lotus::vCardReceived(const QXmppVCardIq& vCard)
{
    if(!vCard.from().isEmpty())
    {
        QString bareJid = vCard.from();
        UserVCard* userVCard= getOrCreateUserVCardFromBareJid(bareJid,vCard);
        rosterItemModel.getRosterItemFromBareJid(bareJid)->setAvatar(userVCard->getAvatar());
    }
}

UserVCard* Lotus::getOrCreateUserVCardFromBareJid(const QString& bareJid,const QXmppVCardIq& vCard)
{
    if(!userVCardList.contains(bareJid))
    {
        userVCardList[bareJid]= new UserVCard();
        userVCardList[bareJid]->setVCard(vCard);
    }
    return userVCardList[bareJid];
}

void Lotus::messageReceived(const QXmppMessage& msg)
{
    if (msg.body().isEmpty())
        return;
    //systemTrayIcon->showMessage(msg.from(),tr("一条新消息"),QSystemTrayIcon::Information,300);
    //messageBox.insert(QXmppUtils::jidToBareJid(msg.from()),msg);

//    foreach (QString msgFrom, messageBox.uniqueKeys()) {
//        qDebug()<<"msgFrom"<<msgFrom<<"    共有："<<messageBox.count(msgFrom);


//    }
    QString msgFrom=QXmppUtils::jidToBareJid(msg.from());
    if(msg.type()==QXmppMessage::Chat){
        if(!personalChatDlgsList.contains(msgFrom))
        {
            if(!messageBox.contains(msgFrom)){
                messageBoxActions[msgFrom]=new QAction(msgFrom,this);
                myMenu.insertAction(aboutAction,messageBoxActions[msgFrom]);


            }
            messageBox.insert(msgFrom,msg);
            trayTimer->start();
            messageBoxActions[msgFrom]->setText(msgFrom+"    "+QString::number(messageBox.count(msgFrom))+tr("条未读"));
        }else{
            PersonalChatDlg *dialog =personalChatDlgsList[msgFrom];
            if (dialog) {
                dialog->messageReceived(msg);
                return;
            }
        }
    }
    if(msg.type()==QXmppMessage::GroupChat){
        if(msg.stamp()<=chatsLog.getLastChatTime(msgFrom)
           &&!msg.stamp().isNull()){
            qDebug()<<"the message is readed!msg.stamp="<<msg.stamp().toString();
            return;
        }
        if(!roomChatDlgsList[msgFrom]->isVisible()){
            if(!messageBox.contains(msgFrom)){
                messageBoxActions[msgFrom]=new QAction(msgFrom,this);
                myMenu.insertAction(aboutAction,messageBoxActions[msgFrom]);
            }
            messageBox.insert(msgFrom,msg);
            trayTimer->start();
            messageBoxActions[msgFrom]->setText(msgFrom+"    "+QString::number(messageBox.count(msgFrom))+tr("条未读"));
        }else{
            GroupChatDlg *dialog = roomChatDlgsList[msgFrom];
            //qDebug()<<"groupchat from : "<<QXmppUtils::jidToResource(msg.from());
            if (dialog) {
                dialog->messageReceived(msg);
                return;
            }
        }

    }
    if(messageBox.count(msgFrom)==1){
        systemTrayIcon->showMessage(msgFrom,tr("一条新消息"),QSystemTrayIcon::Information,300);
    }
}

void Lotus::rosterReceived()
{
    qDebug() << "rosterReceived is called";
    QStringList list = client->rosterManager().getRosterBareJids();
    QString bareJid;
    foreach(bareJid, list)
    {
        if(client->rosterManager().getRosterEntry(bareJid).subscriptionType()
                ==QXmppRosterIq::Item::Both ||client->rosterManager().
                getRosterEntry(bareJid).subscriptionType()==QXmppRosterIq::Item::To  )
        {
            rosterChanged(bareJid);
            client->vCardManager().requestVCard(bareJid);
        }
    }
}

void Lotus::rosterChanged(const QString& bareJid)
{
    if(client->rosterManager().getRosterEntry(bareJid).subscriptionType()==
            QXmppRosterIq::Item::Both ||
            client->rosterManager().getRosterEntry(bareJid).subscriptionType()==
            QXmppRosterIq::Item::To  )
    {
        rosterItemModel.updateRosterEntry(bareJid,
                                          client->rosterManager().getRosterEntry(bareJid));
        client->rosterManager().getResources(bareJid);
        presenceChanged(bareJid,*client->rosterManager().getResources(bareJid).begin());
    }
    // if available in cache, update it else based on presence it will request if not available
    //    if(m_vCardCache.isVCardAvailable(bareJid))
    //        updateVCard(bareJid);

}

void Lotus::messageAction(QAction *action)
{

    QString msgfrom= messageBoxActions.key(action);
    QXmppMessage::Type type=messageBox.value(msgfrom).type();
    if(type==QXmppMessage::Chat){
    showChatDialog(msgfrom);
    PersonalChatDlg *dialog =personalChatDlgsList[msgfrom];
    if (dialog) {
//        foreach (QXmppMessage msg, messageBox.values(msgfrom)) {
//            dialog->messageReceived(msg);
//        }

        for(int i=messageBox.values(msgfrom).size()-1;i>=0;i--){

            dialog->messageReceived(messageBox.values(msgfrom).takeAt(i));
        }
        messageBox.remove(msgfrom);
        myMenu.removeAction(messageBoxActions[msgfrom]);
        messageBoxActions.remove(msgfrom);
    }
    }else if(type==QXmppMessage::GroupChat){
        getGroupChatDialog(msgfrom)->show();
        GroupChatDlg *dialog = roomChatDlgsList[msgfrom];
        if (dialog) {
//            foreach (QXmppMessage msg, messageBox.values(msgfrom)) {
//                dialog->messageReceived(msg);
//            }

            for(int i=messageBox.values(msgfrom).size()-1;i>=0;i--){

                dialog->messageReceived(messageBox.values(msgfrom).takeAt(i));
            }
            messageBox.remove(msgfrom);
            myMenu.removeAction(messageBoxActions[msgfrom]);
            messageBoxActions.remove(msgfrom);
        }
    }
    if(messageBox.empty()){
        trayTimer->stop();
        systemTrayIcon->setIcon(QIcon(":/pixmap/image/tray.ico"));
    }
}

void Lotus::setFriendGroupOtherList()
{
    m_pProAnima = new QPropertyAnimation(ui->friendTree,"geometry");
    m_gProAnima = new QPropertyAnimation(ui->roomList,"geometry");
    m_tProAnima = new QPropertyAnimation(ui->otherView,"geometry");
    m_pSignalMapper = new QSignalMapper(this);
    ui->friendLabel->setAlignment(Qt::AlignCenter);
    ui->roomLabel->setAlignment(Qt::AlignCenter);
    ui->otherLabel->setAlignment(Qt::AlignCenter);
    ui->friendLabel->installEventFilter(this);
    ui->roomLabel->installEventFilter(this);
    ui->otherLabel->installEventFilter(this);
    connect(ui->friendLabel, SIGNAL(clicked()), this, SLOT(setList()));
    connect(ui->roomLabel, SIGNAL(clicked()), this, SLOT(setList()));
    connect(ui->otherLabel, SIGNAL(clicked()), this, SLOT(setList()));
    connect(ui->friendLabel, SIGNAL(clicked()), m_pSignalMapper, SLOT(map()));
    connect(ui->roomLabel, SIGNAL(clicked()), m_pSignalMapper, SLOT(map()));
    connect(ui->otherLabel, SIGNAL(clicked()), m_pSignalMapper, SLOT(map()));
    m_pSignalMapper->setMapping(ui->friendLabel,1);
    m_pSignalMapper->setMapping(ui->roomLabel,2);
    m_pSignalMapper->setMapping(ui->otherLabel,3);
    connect(m_pSignalMapper, SIGNAL(mapped(int)),this, SLOT(SetAnimation(int)));
}

void Lotus::setClient(QXmppClient *sClient)
{
    client=sClient;
}

void Lotus::SetAnimation(int nCurveType)
{
    if(m_pProAnima->state() == QPropertyAnimation::Running)
    {
        m_pProAnima->stop();
    }
    if(m_gProAnima->state() == QPropertyAnimation::Running)
    {
        m_gProAnima->stop();
    }
    if(m_tProAnima->state() == QPropertyAnimation::Running)
    {
        m_tProAnima->stop();
    }
    //设置新的动画
    if(nCurveType == 1)
    {
        m_pProAnima->setDuration(500);
        m_pProAnima->setStartValue(QRect(ui->friendTree->pos().x(),ui->friendTree->pos().y(),290,451));
        m_pProAnima->setEndValue(QRect(0,200,290,451));
        m_pProAnima->setEasingCurve(QEasingCurve::OutSine);
        m_pProAnima->start();
        m_gProAnima->setDuration(500);
        m_gProAnima->setStartValue(QRect(ui->roomList->pos().x(),ui->roomList->pos().y(),290,451));
        m_gProAnima->setEndValue(QRect(290,200,290,451));
        m_gProAnima->setEasingCurve(QEasingCurve::OutSine);
        m_gProAnima->start();
        m_tProAnima->setDuration(500);
        m_tProAnima->setStartValue(QRect(ui->otherView->pos().x(),ui->otherView->pos().y(),290,451));
        m_tProAnima->setEndValue(QRect(580,200,290,451));
        m_tProAnima->setEasingCurve(QEasingCurve::OutSine);
        m_tProAnima->start();
    }
    if(nCurveType == 2)
    {
        m_pProAnima->setDuration(500);
        m_pProAnima->setStartValue(QRect(ui->friendTree->pos().x(),ui->friendTree->pos().y(),290,451));
        m_pProAnima->setEndValue(QRect(-290,200,290,451));
        m_pProAnima->setEasingCurve(QEasingCurve::OutSine);
        m_pProAnima->start();
        m_gProAnima->setDuration(500);
        m_gProAnima->setStartValue(QRect(ui->roomList->pos().x(),ui->roomList->pos().y(),290,451));
        m_gProAnima->setEndValue(QRect(0,200,290,451));
        m_gProAnima->setEasingCurve(QEasingCurve::OutSine);
        m_gProAnima->start();
        m_tProAnima->setDuration(500);
        m_tProAnima->setStartValue(QRect(ui->otherView->pos().x(),ui->otherView->pos().y(),290,451));
        m_tProAnima->setEndValue(QRect(290,200,290,451));
        m_tProAnima->setEasingCurve(QEasingCurve::OutSine);
        m_tProAnima->start();
    }
    if(nCurveType == 3)
    {
        m_pProAnima->setDuration(500);
        m_pProAnima->setStartValue(QRect(ui->friendTree->pos().x(),ui->friendTree->pos().y(),290,451));
        m_pProAnima->setEndValue(QRect(-580,200,290,451));
        m_pProAnima->setEasingCurve(QEasingCurve::OutSine);
        m_pProAnima->start();
        m_gProAnima->setDuration(500);
        m_gProAnima->setStartValue(QRect(ui->roomList->pos().x(),ui->roomList->pos().y(),290,451));
        m_gProAnima->setEndValue(QRect(-290,200,290,451));
        m_gProAnima->setEasingCurve(QEasingCurve::OutSine);
        m_gProAnima->start();
        m_tProAnima->setDuration(500);
        m_tProAnima->setStartValue(QRect(ui->otherView->pos().x(),ui->otherView->pos().y(),290,451));
        m_tProAnima->setEndValue(QRect(0,200,290,451));
        m_tProAnima->setEasingCurve(QEasingCurve::OutSine);
        m_tProAnima->start();
    }

}

void Lotus::setList()
{

    TabLabel *lb = (TabLabel *)sender();
    if(lb == ui->friendLabel)
    {
        lb->setStyleSheet("border-image:url(:/pixmap/image/friendLabel_b.png)");
    }else if(lb == ui->roomLabel)
    {
        lb->setStyleSheet("border-image:url(:/pixmap/image/roomLabel_b.png)");
    }else if(lb == ui->otherLabel)
    {
        lb->setStyleSheet("border-image:url(:/pixmap/image/recentLabel_b.png)");
    }

    if(currenttab == ui->friendLabel)
    {
        currenttab->setStyleSheet("border-image:url(:/pixmap/image/friendLabel.png)");
    }else if(currenttab == ui->roomLabel)
    {
        currenttab->setStyleSheet("border-image:url(:/pixmap/image/roomLabel.png)");
    }else if(currenttab == ui->otherLabel)
    {
        currenttab->setStyleSheet("border-image:url(:/pixmap/image/recentLabel.png)");
    }
    currenttab = lb;

}

Lotus::~Lotus()
{
    delete ui;
}

void Lotus::on_searchButton_clicked()
{
    searchDlg.setClient(client);
    searchDlg.show();
    int ScreenW = QApplication::desktop()->width();
    int ScreenH  = QApplication::desktop()->height();
    searchDlg.move((ScreenW-443)/2,(ScreenH-311)/2);
}

void Lotus::trayMessage()
{
    if(trayIconStatus){
        systemTrayIcon->setIcon(QIcon(":/pixmap/image/roomFavorite_b.ico"));
        trayIconStatus=false;
    }else{
        systemTrayIcon->setIcon(QIcon(":/pixmap/image/roomFavorite.ico"));
        trayIconStatus=true;
    }
}

void Lotus::clickAboutButton()
{
    aboutDlg.show();
}

void Lotus::statusTextChanged(const QString& statusText)
{
    if(!ui->statusText->text().isEmpty()
      &&ui->statusText->text()!=tr("编辑此处更新状态/签名"))
    {
    qDebug()<<"~~~~~~~~~~~~~更新签名："<< statusText;
    QXmppPresence presence = client->clientPresence();
    presence.setStatusText(statusText);
    client->setClientPresence(presence);
    roomFavoriteCache.changeStatusText(statusText);
    }else if(ui->statusText->text().isEmpty())
    {
        roomFavoriteCache.changeStatusText(statusText);
        ui->statusText->setText(tr("编辑此处更新状态/签名"));
    }
}

void Lotus::on_statusText_editingFinished()
{
    statusTextChanged(ui->statusText->text());
    setFocus();
}
