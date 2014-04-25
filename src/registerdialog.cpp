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

#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "qmessagebox.h"
RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    ui->passwordlineEdit->setEchoMode(QLineEdit::Password);
    ui->pwConfirmlineEdit->setEchoMode(QLineEdit::Password);
    this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setAttribute(Qt::WA_TranslucentBackground);
    ui->closeButton->installEventFilter(this);
    ui->registerButton->installEventFilter(this);
    connect(&client , SIGNAL(connected()),this,SLOT(clientConnected()));
    connect(&client , SIGNAL(iqReceived(QXmppIq)),this,SLOT(iqReceived(QXmppIq)));
    client.logger()->setLoggingType(QXmppLogger::StdoutLogging);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::setServerIP(const QString &ServerIP)
{
    serverIP=ServerIP;
}

void RegisterDialog::iqReceived(const QXmppIq &iq)
{
    ui->waitinfo->clear();
    if((iq.id()==iqid))
    {

        if(iq.error().code()==0)
        {
        QMessageBox::information(this,tr("注册完成") ,tr("用户注册成功"));
        qDebug()<<"error code="<< iq.error().code();
        close();
        }else if(iq.error().code()==409){
        QMessageBox::information(this,tr("注册失败"),"error code:"+QString::number(iq.error().code())+"\n"+tr("用户已存在！请重新注册"));
        close();
        }else{
        QMessageBox::information(this,tr("注册失败"),"error code:"+iq.error().code());
        qDebug()<<"error code="<< iq.error().code();
        qDebug()<<"error text="<< iq.error().text();
        qDebug()<<"error type="<< iq.error().type();
        }
    }
}


void RegisterDialog::clientConnected()
{
    qDebug("CONNECTED");
    QXmppRegisterIq iq;
    iq.setType(QXmppIq::Set);
    iq.setTo(serverIP);
    iq.setUsername(ui->userNamelineEdit->text());
    iq.setPassword(ui->passwordlineEdit->text());
    //QXMPPManager::getClient()->sendPacket(iq);
    client.sendPacket(iq);
    qDebug()<<"iq id = "<<iq.id();
    iqid=iq.id();


}
void RegisterDialog::on_closeButton_clicked()
{
    close();
}

void RegisterDialog::on_registerButton_clicked()
{
    if(ui->passwordlineEdit->text()!=ui->pwConfirmlineEdit->text())
    {
        QMessageBox::information(this,tr("错误") ,tr("密码确认失败，请重新输入密码！"));
    }else{
        config.setJid("@"+serverIP);
    //config.setPassword(ui->lineEdit_2->text());
        config.setUseSASLAuthentication(false);
        config.setUseNonSASLAuthentication(false);
        client.connectToServer(config);
        ui->waitinfo->setText(tr("正在注册，请稍候..."));
    }
}

void RegisterDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragPos = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }

}

void RegisterDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton )
    {
        move(event->globalPos() - dragPos);
        setWindowOpacity(0.5);
    }
    event->accept();

}

void RegisterDialog::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        setWindowOpacity(1);
    }
    event->accept();
}

bool RegisterDialog::eventFilter(QObject *obj, QEvent *event)
{
    if(obj==ui->closeButton||obj==ui->registerButton)
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
