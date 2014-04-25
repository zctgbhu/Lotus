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
#include "loginloadwin.h"
#include "ui_loginloadwin.h"

LoginloadWin::LoginloadWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginloadWin)
{
    pwremember=false;
    int ScreenW = QApplication::desktop()->width();

    move(ScreenW-400,100);
    oparcity = 1.0;
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    connect(ui->btn_cancel, SIGNAL(clicked()), this, SLOT(cancel_login()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(disappear()));
    connect(&loginTimer, SIGNAL(timeout()), this, SLOT(loginTimeOut()));
    connect(&client, SIGNAL(connected()), this, SLOT(connected()));
    connect(&lotusMainUI,SIGNAL(retrunLoginWin()),
            this,SLOT(cancel_login()));
    ui->btn_min->installEventFilter(this);
    ui->btn_close->installEventFilter(this);

    ui->btn_cancel->setFocusPolicy(Qt::NoFocus);
    ui->btn_cancel->setStyleSheet(QString("QPushButton{border-image:url(:/pixmap/image/cancal.png);color: rgb(255, 255, 255);}"));
    ui->btn_cancel->installEventFilter(this);
}


LoginloadWin::~LoginloadWin()
{
    delete ui;
}

void LoginloadWin::connected()
{
    qDebug()<<"show lotus main UI";
    qDebug()<<"setclient";
    lotusMainUI.setClient(&client);
    lotusMainUI.init();
    lotusMainUI.show();
    addAccountToCache();
    disconnect(&client,SIGNAL(error(QXmppClient::Error)),this,SLOT(loginError(QXmppClient::Error)));
    this->close();
}

void LoginloadWin::addAccountToCache()
{
    QString bareJid = userName;
    QString pw = passWord;
    if(!pwremember)
        pw = "";
    accountCache.addAccount(bareJid, pw);
}

void LoginloadWin::loginTimeOut()
{
    if (QMessageBox::warning  (this,
                              tr("连接超时！"),
                              tr("连接超时！"),
                              QMessageBox::Ok) == QMessageBox::Ok)
    {
        cancel_login();
    }
}

void LoginloadWin::loginError(QXmppClient::Error e)
{
    if(e==QXmppClient::SocketError){
        cancel_login();
        QMessageBox::warning(this,tr("登录出错!"),tr("连接断开!"),
                                  QMessageBox::Ok);
        return;
    }else if(e==QXmppClient::KeepAliveError){
        QMessageBox::warning(this,tr("登录出错!"),tr("网络连接错误! 请检查网络状态并重新登录"),
                                  QMessageBox::Ok);
    }else if(e==QXmppClient::XmppStreamError){
        cancel_login();
        QMessageBox::warning(this,tr("登录出错!"),tr("请检查用户名、密码"),
                                  QMessageBox::Ok);
        return;
    }else{
        qDebug()<<"unknown error";
    }
    cancel_login();
}

void LoginloadWin::login()
{
    connect(&client,SIGNAL(error(QXmppClient::Error)),this,SLOT(loginError(QXmppClient::Error)));
    //client.logger()->setLoggingType(QXmppLogger::StdoutLogging);
    config.setJid(userName);
    config.setPassword(passWord);
    client.connectToServer(config);
    //TODO: connect time out
    //loginTimer.start(5000);
}

void LoginloadWin::setUserName(QString name)
{
    userName=name;
}

void LoginloadWin::setPassWord(QString pw)
{
    passWord=pw;
}

void LoginloadWin::setPWRemember(bool PWRemember)
{
    pwremember=PWRemember;
}

void LoginloadWin::setAccountCache(AccountCache ACache)
{
    accountCache=ACache;
}

void LoginloadWin::cancel_login()
{
    disconnect(&client,SIGNAL(error(QXmppClient::Error)),this,SLOT(loginError(QXmppClient::Error)));
    loginTimer.stop();
    timer.start(100);
    client.disconnectFromServer();
}

void LoginloadWin::disappear()
{
    if ( oparcity < 0)
    {

        timer.stop();
        oparcity =1;
        setWindowOpacity(oparcity);
        emit cancel();
        this->close();
        return;
    }
    setWindowOpacity(oparcity);
    oparcity = oparcity - 0.4;
}



void LoginloadWin::paintEvent(QPaintEvent *)
{
//    QPainter painter(this);
//    painter.drawPixmap(0,0,300,254,QPixmap(":/img/up.png"));
//    painter.drawPixmap(0,255,300,500,QPixmap(":/img/mid.png"));
//    painter.drawPixmap(0,501,300,660,QPixmap(":/img/down.png"));
}

bool LoginloadWin::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->btn_min){                          //最小化、最大化、关闭按钮换图
            if(event->type() == QEvent::Enter){
                ui->btn_min->setPixmap(QPixmap(":/pixmap/image/miniButton-hover.png"));
            }else if(event->type() == QEvent::Leave){
                ui->btn_min->setPixmap(QPixmap(":/pixmap/image/miniButton.png"));
            }else if(event->type() == QEvent::MouseButtonPress){
                ui->btn_min->setPixmap(QPixmap(":/pixmap/image/miniButton-hover.png"));
            }else if(event->type() == QEvent::MouseButtonRelease){
                QMouseEvent *me = (QMouseEvent *)event;
                QLabel *lb = (QLabel *)obj;
                if(me->x() > 0 && me->x() < lb->width() && me->y() > 0 && me->y() < lb->height()){
                    this->showMinimized();
                }else{
                    ui->btn_min->setPixmap(QPixmap(":/pixmap/image/miniButton.png"));
                }
            } else {
                return QObject::eventFilter(obj, event);
            }
        }
    if(obj == ui->btn_close){                          //最小化、最大化、关闭按钮换图
            if(event->type() == QEvent::Enter){
                ui->btn_close->setPixmap(QPixmap(":/pixmap/image/closeButton-hover.png"));
            }else if(event->type() == QEvent::Leave){
                ui->btn_close->setPixmap(QPixmap(":/pixmap/image/closeButton.png"));
            }else if(event->type() == QEvent::MouseButtonPress){
                ui->btn_close->setPixmap(QPixmap(":/pixmap/image/closeButton-hover.png"));
            }else if(event->type() == QEvent::MouseButtonRelease){
                QMouseEvent *me = (QMouseEvent *)event;
                QLabel *lb = (QLabel *)obj;
                if(me->x() > 0 && me->x() < lb->width() && me->y() > 0 && me->y() < lb->height()){
                    this->close();
                }else{
                    ui->btn_close->setPixmap(QPixmap(":/pixmap/image/closeButton.png"));
                }
            } else {
                return QObject::eventFilter(obj, event);
            }
        }

    if(obj==ui->btn_cancel)
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

void LoginloadWin::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragPos = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }

}

void LoginloadWin::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton )
    {
        move(event->globalPos() - dragPos);
        setWindowOpacity(0.5);
    }
    event->accept();

}

void LoginloadWin::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        setWindowOpacity(1);
    }
    event->accept();
}
