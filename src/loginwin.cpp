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
#include "loginwin.h"
#include "ui_loginwin.h"
#include "settings.h"
extern QString ServerAddress;
LoginWin::LoginWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWin)
{
    oparcity = 1.0;
    ui->setupUi(this);  


    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    int bgW =450;
    int bgH =330;
    this->resize(bgW, bgH);
    ui->bg->resize(bgW,bgH);
    this->setWindowTitle("Lotus");
    int ScreenW = QApplication::desktop()->width();
    int ScreenH  = QApplication::desktop()->height();
    move((ScreenW-bgW)/2,(ScreenH-bgH)/2);
    ui->btn_min->installEventFilter(this);
    ui->btn_close->installEventFilter(this);
    ui->btn_register->installEventFilter(this);
    ui->btn_setting->installEventFilter(this);
    ui->aboutButton->installEventFilter(this);
    ui->lotus->installEventFilter(this);
    ui->loginButton->installEventFilter(this);
    ui->loginButton->setStyleSheet(QString("QPushButton{border-image:url(:/pixmap/image/loginButton.png);color: rgb(255, 255, 255);}"));
    ui->loginButton->setFocusPolicy(Qt::NoFocus);
    ui->passwd->setEchoMode(QLineEdit::Password);
    ui->loginButton->setShortcut(QKeySequence::InsertParagraphSeparator);

    ServerAddress = "210.209.123.136";

    loadAccountCache();
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(login()));
    connect(&loginloadWin,SIGNAL(cancel()),this,SLOT(show()));
    connect(ui->username,SIGNAL(editTextChanged(QString)),this,SLOT(setPassWordForAccountCache(QString)));

}

LoginWin::~LoginWin()
{
    delete ui;
}


void LoginWin::login()
{
    m_pw = ui->passwd->text();
    m_user = ui->username->currentText();
    m_user += "@" + ServerAddress;



    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(disappear()));
    timer->start(100);

}

void LoginWin::setPassWordForAccountCache(const QString& userName)
{
    QString passwd = accountCache.getPassword(userName+"@"+ServerAddress);
    qDebug()<<"accountpasswd"<<passwd;
    ui->passwd->setText(passwd);
    if(!passwd.isEmpty())
        ui->passwdremember->setChecked(true);
}

void LoginWin::disappear()
{
    if( oparcity < 0)
    {
        this->hide();
        timer->stop();
        oparcity=1;
        this->setWindowOpacity(oparcity);
        loginloadWin.show();
        loginloadWin.setUserName(m_user);
        loginloadWin.setPassWord(m_pw);
        loginloadWin.setPWRemember(ui->passwdremember->isChecked());
        loginloadWin.setAccountCache(accountCache);
        loginloadWin.login();
        return ;
    }
    this->setWindowOpacity(oparcity);
    oparcity = oparcity - 0.4;
    //qDebug()<<"oparcity - 0.1";
}

void LoginWin::loadAccountCache()
{
    accountCache.loadFromFile();
    QStringList accountlist = accountCache.getUserName();
    QCompleter *completer = new QCompleter(accountlist, this);
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->username->setCompleter(completer);
    ui->username->addItems(accountlist);
    if(!accountlist.isEmpty())
    {
        ui->username->setEditText(accountlist.last());
        QString passwd = accountCache.getPassword(accountlist.last()+"@"+ServerAddress);
        qDebug()<<"accountpasswd"<<passwd;
        ui->passwd->setText(passwd);
        if(!passwd.isEmpty())
            ui->passwdremember->setChecked(true);
    }
}

bool LoginWin::eventFilter(QObject *obj, QEvent *event)
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
                    exit(0);
                }else{
                    ui->btn_close->setPixmap(QPixmap(":/pixmap/image/closeButton.png"));
                }
            } else {
                return QObject::eventFilter(obj, event);
            }
        }
    if(obj==ui->btn_register){
        if(event->type() == QEvent::MouseButtonRelease){
            action_register();
        }
    }
    if(obj==ui->btn_setting){
        if(event->type() == QEvent::MouseButtonRelease){
            action_setting();
        }
    }
    if(obj==ui->aboutButton || obj==ui->lotus){
        if(event->type() == QEvent::MouseButtonRelease){
            AboutDialog aboutDlg;
            aboutDlg.exec();
        }
    }
    if(obj==ui->loginButton)
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

void LoginWin::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragPos = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }

}

void LoginWin::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton )
    {
        move(event->globalPos() - dragPos);
        setWindowOpacity(0.5);
    }
    event->accept();

}

void LoginWin::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        setWindowOpacity(1);
    }
    event->accept();
}

void LoginWin::action_setting()
{
    Settings settingsDlg;
    settingsDlg.serverAddress=ServerAddress;
    settingsDlg.setDefaultServer();
    settingsDlg.exec();
    ServerAddress=settingsDlg.serverAddress;
}

void LoginWin::action_register()
{
    RegisterDialog registerdlg;
    registerdlg.setServerIP(ServerAddress);
    registerdlg.exec();
}
