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

#include "searchdlg.h"
#include "ui_searchdlg.h"
#include <QMessageBox>
extern QString ServerAddress;
SearchDlg::SearchDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDlg)
{
    isClient=false;
    ui->setupUi(this);
    searchId="";
    selectUserJid="";
    selectRoomJid="";
    refreshTimer.setInterval(5000);
    searchTimer.setInterval(5000);
    ui->friendTabButton->setFocusPolicy(Qt::NoFocus);
    ui->roomTabButton->setFocusPolicy(Qt::NoFocus);
    ui->roomTabButton->installEventFilter(this);
    ui->friendTabButton->installEventFilter(this);
    ui->closeFriendTabButton->installEventFilter(this);
    ui->addFriendButton->installEventFilter(this);
    ui->closeRoomTabButton->installEventFilter(this);
    ui->joinRoomButton->installEventFilter(this);
    ui->refreshButton->installEventFilter(this);
    ui->sarchUserButton->installEventFilter(this);
    ui->searchUserlistView->setModel(&itemSearchModel);
    ui->searchRoomListView->setModel(&itemRoomModel);
    connect(ui->searchRoomListView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(setSelectRoom(QModelIndex)));
    connect(ui->searchUserlistView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(setSelectSearchItem(QModelIndex)));
    connect(&refreshTimer,SIGNAL(timeout()),this,SLOT(refreshRoomTimeout()));
    connect(&searchTimer,SIGNAL(timeout()),this,SLOT(searchUserTimeout()));
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    ui->friendTab->show();
    ui->roomTab->hide();
    isRefreshing=false;
    isSearching=false;

    ui->searchUserlistView->setSpacing(2);
    ui->searchUserlistView->setFont(QFont("",12));
    ui->searchRoomListView->setSpacing(2);
    ui->searchRoomListView->setFont(QFont("",12));
}

void SearchDlg::iqReceived(QXmppIq iq)
{
    if(searchId==iq.id() && isSearching==true)
    {
        itemSearchModel.clear();
        searchTimer.stop();
        QXmppElementList elementList = iq.extensions();
        QXmppElement item = elementList.first().firstChildElement().firstChildElement("item");
        while (!item.isNull()) {
            //qDebug()<< item.firstChildElement().firstChildElement().value();
            QXmppElement itemName=item.firstChildElement();
            QXmppElement itemEmail=itemName.nextSiblingElement();
            QXmppElement itemUserName=itemEmail.nextSiblingElement();
            QXmppElement itemJid=itemUserName.nextSiblingElement();
            QString name=itemName.firstChildElement().value();
            QString email=itemEmail.firstChildElement().value();
            QString userName=itemUserName.firstChildElement().value();
            QString jid=itemJid.firstChildElement().value();
            // qDebug()<< item.firstChildElement().nextSiblingElement().firstChildElement().value();
            qDebug()<< "name=" << name ;
            qDebug()<< "email=" << email ;
            qDebug()<< "userName=" << userName ;
            qDebug()<< "jid=" << jid ;
            QStandardItem *itemSearch=new QStandardItem(name+" "+email+" "+userName+" "+jid);
            itemSearch->setData(jid,Qt::UserRole);
            itemSearchModel.appendRow(itemSearch);
            //QString email=item.firstChildElement().nextSiblingElement().firstChildElement().value();

            item=item.nextSiblingElement("item");
        }
        isSearching=false;
        ui->sarchUserButton->setEnabled(true);
    }
}

void SearchDlg::setSelectSearchItem(const QModelIndex& index)
{
    selectUserJid= index.data(Qt::UserRole).toString();
}

void SearchDlg::setSelectRoom(const QModelIndex& index){
    qDebug()<<"setSelectRoom"<<index.data().toString();
    //selectRoomJid.append(index.data().toString());
    selectRoomJid=index.data().toString();

}


SearchDlg::~SearchDlg()
{
    delete ui;
}

void SearchDlg::setClient(QXmppClient *Client)
{
    if(!isClient)
    {
        client=Client;
        isClient=true;
        ext = client->findExtension<QXmppDiscoveryManager>();
        rosterManager = client->findExtension<QXmppRosterManager>();
        connect(client,SIGNAL(iqReceived(QXmppIq)),
                this,SLOT(iqReceived(QXmppIq)));
        connect(ext,SIGNAL(itemsReceived(QXmppDiscoveryIq)),
                this, SLOT(itemsReceived(QXmppDiscoveryIq)));
    }
}

void SearchDlg::itemsReceived(const QXmppDiscoveryIq &qxmppdisiq){
    qDebug()<<"~~~~~~~~~~~~~~~~itemsReceived~~~~~~~~~~~~~~~~~~~~~~~~~";
    if(isRefreshing==false)
        return;
    refreshTimer.stop();
    for(int i=0;i<qxmppdisiq.items().size();i++){
        qDebug()<< qxmppdisiq.items().at(i).jid();
        QStandardItem *item=new QStandardItem(qxmppdisiq.items().at(i).jid());
        itemRoomModel.appendRow(item);
    }
    ui->refreshButton->setEnabled(true);
    isRefreshing=false;
    qDebug()<<"~~~~~~~~~~~~~~~itemsReceived~~~~~~~~~~~~~~~~~~~~~~~~~~";
}

bool SearchDlg::isValidBareJid(const QString& bareJid)
{
    QRegExp re("^[^@]+@[^@]+$");
    return re.exactMatch(bareJid);
}

void SearchDlg::on_addFriendButton_clicked()
{

    //    if(!isValidBareJid(bareJid))
    //    {
    //        QMessageBox::information(this, tr("帐号格式错误"), tr("输入的帐号<font color=\"red\">")+bareJid + tr(" </font> 是错误的."));
    //        return;
    //    }
    if(!selectUserJid.isEmpty())
    {
        bool ok;
        QString reason=QInputDialog::getText(this,tr("好友请求"),tr("验证信息："),QLineEdit::Normal,tr("我想认识你^_^"),&ok);
        if(ok){
        rosterManager->subscribe(selectUserJid,reason);
        QMessageBox::information(this,"提示","添加好友请求已经发送,等待对方确认",QMessageBox::Ok);
        }
        //        QXmppPresence subscribe;
        //        subscribe.setTo(bareJid);
        //        subscribe.setType(QXmppPresence::Subscribe);
        //        qDebug()<<"sendPacke";
        //        client->sendPacket(subscribe);
        //close();
    }
}

void SearchDlg::on_closeFriendTabButton_clicked()
{
    ui->userNameLineEdit->clear();
    itemSearchModel.clear();
    itemRoomModel.clear();
    close();
}

void SearchDlg::on_closeRoomTabButton_clicked()
{
    ui->userNameLineEdit->clear();
    itemSearchModel.clear();
    itemRoomModel.clear();
    close();
}

void SearchDlg::on_refreshButton_clicked()
{
    itemRoomModel.clear();
    ui->refreshButton->setEnabled(false);
    isRefreshing=true;
    refreshTimer.start();
    ext->requestItems("conference."+ServerAddress);
}

void SearchDlg::on_joinRoomButton_clicked()
{
    if(!selectRoomJid.isEmpty()){
        emit joinRoom(selectRoomJid);
        //close();
    }
}

void SearchDlg::on_sarchUserButton_clicked()
{
    itemSearchModel.clear();
    searchTimer.start();
    ui->sarchUserButton->setEnabled(false);
    QString searchText = ui->userNameLineEdit->text();
    if(searchText.isEmpty()){
        searchText="*";
    }
    QXmppIq iq=serachUser(searchText);
    searchId=iq.id();
    isSearching=true;
    client->sendPacket(iq);

}

QXmppIq SearchDlg::serachUser(const QString &searchText)
{
    QXmppIq iq;
    iq.setType(QXmppIq::Set);
    //iq.setFrom("aaa@192.168.30.234/QXmpp");
    iq.setTo("search."+ServerAddress);
    //iq.setId("search4");
    //iq.setLang("en");

    QXmppElement qxmppelement;
    qxmppelement.setTagName("query");
    qxmppelement.setAttribute("xmlns","jabber:iq:search");

    QXmppElement qxmppelement2;
    qxmppelement2.setTagName("x");
    qxmppelement2.setAttribute("xmlns","jabber:x:data");
    qxmppelement2.setAttribute("type","submit");

    QXmppElement qxmppelement3;
    qxmppelement3.setTagName("field");
    qxmppelement3.setAttribute("var","FORM_TYPE");
    qxmppelement3.setAttribute("type","hidden");

    QXmppElement qxmppelement4;
    qxmppelement4.setTagName("value");
    qxmppelement4.setValue("jabber:iq:search");
    qxmppelement3.appendChild(qxmppelement4);
    qxmppelement2.appendChild(qxmppelement3);

    QXmppElement qxmppelement33;
    qxmppelement33.setTagName("field");
    qxmppelement33.setAttribute("var","search");
    qxmppelement33.setAttribute("type","text-single");
    QXmppElement qxmppelement44;
    qxmppelement44.setTagName("value");
    qxmppelement44.setValue(searchText);
    qxmppelement33.appendChild(qxmppelement44);
    qxmppelement2.appendChild(qxmppelement33);

    QXmppElement qxmppelement3u;
    qxmppelement3u.setTagName("field");
    qxmppelement3u.setAttribute("var","Username");
    qxmppelement3u.setAttribute("type","boolean");
    QXmppElement qxmppelement4u;
    qxmppelement4u.setTagName("value");
    qxmppelement4u.setValue("1");
    qxmppelement3u.appendChild(qxmppelement4u);
    qxmppelement2.appendChild(qxmppelement3u);

    QXmppElement qxmppelement3n;
    qxmppelement3n.setTagName("field");
    qxmppelement3n.setAttribute("var","Name");
    qxmppelement3n.setAttribute("type","boolean");
    QXmppElement qxmppelement4n;
    qxmppelement4n.setTagName("value");
    qxmppelement4n.setValue("1");
    qxmppelement3n.appendChild(qxmppelement4n);
    qxmppelement2.appendChild(qxmppelement3n);

    QXmppElement qxmppelement3e;
    qxmppelement3e.setTagName("field");
    qxmppelement3e.setAttribute("var","Email");
    qxmppelement3e.setAttribute("type","boolean");
    QXmppElement qxmppelement4e;
    qxmppelement4e.setTagName("value");
    qxmppelement4e.setValue("1");
    qxmppelement3e.appendChild(qxmppelement4e);
    qxmppelement2.appendChild(qxmppelement3e);

    qxmppelement.appendChild(qxmppelement2);
    QXmppElementList qel;
    qel.append(qxmppelement);
    iq.setExtensions(qel);
    //QXMPPManager::getClient()->sendPacket(iq);
    //client.sendPacket(iq);
    return iq;
}

void SearchDlg::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        dragPos = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }

}

void SearchDlg::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton )
    {
        move(event->globalPos() - dragPos);
        setWindowOpacity(0.5);
    }
    event->accept();

}

void SearchDlg::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        setWindowOpacity(1);
    }
    event->accept();
}

bool SearchDlg::eventFilter(QObject *obj, QEvent *event)
{
    if(obj==ui->closeFriendTabButton||
            obj==ui->addFriendButton||
            obj==ui->closeRoomTabButton||
            obj==ui->joinRoomButton||
            obj==ui->refreshButton||
            obj==ui->sarchUserButton||
            obj==ui->friendTabButton||
            obj==ui->roomTabButton)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *me = (QMouseEvent *)event;
            dragPos = me->globalPos() - frameGeometry().topLeft();
        }else if(event->type() == QEvent::MouseButtonRelease)
        {
            setWindowOpacity(1);
        }
    }
    return QObject::eventFilter(obj, event);
}

void SearchDlg::on_friendTabButton_clicked()
{
    ui->friendTab->show();
    ui->roomTab->hide();
}

void SearchDlg::on_roomTabButton_clicked()
{
    ui->friendTab->hide();
    ui->roomTab->show();
}

void SearchDlg::refreshRoomTimeout()
{
    isRefreshing=false;
    ui->refreshButton->setEnabled(true);
    refreshTimer.stop();
    QMessageBox::information(this,"提示","刷新聊天室列表超时",QMessageBox::Ok);
}

void SearchDlg::searchUserTimeout()
{
    isSearching=false;
    ui->sarchUserButton->setEnabled(true);
    searchTimer.stop();
    QMessageBox::information(this,"提示","搜索用户超时",QMessageBox::Ok);
}
