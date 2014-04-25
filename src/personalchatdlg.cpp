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

#include "personalchatdlg.h"
#include "ui_personalchatdlg.h"
#include "QXmppUtils.h"
#include <QPushButton>
#include <QPixmap>
#include <QBitmap>

PersonalChatDlg::PersonalChatDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonalChatDlg)
{
    sendFileJob=NULL;
    job=NULL;
    ui->setupUi(this);
    dir = QDesktopServices::storageLocation(QDesktopServices::HomeLocation)+"/.Lotus/";
    ui->announcement->setContextMenuPolicy(Qt::CustomContextMenu);
    int ScreenW = QApplication::desktop()->width();
    int ScreenH  = QApplication::desktop()->height();
    move((ScreenW-570)/2,(ScreenH-493)/2);
    setFixedSize(570,493);
    setView();
    isClientSet=false;
    setAttribute(Qt::WA_DeleteOnClose);
}

void PersonalChatDlg::faceClicked(QString id)
{
    disconnect(facePanel,SIGNAL(faceClicked(QString)),
            this,SLOT(faceClicked(QString)));
    ui->messageEdit->insertHtml("<img src='"+id+"'/>");
}

PersonalChatDlg::~PersonalChatDlg()
{
    delete ui;
}

void PersonalChatDlg::sendMessage()
{
    QString message=ui->messageEdit->toHtml();
    qDebug()<<"message"<<message;
    if(ui->messageEdit->toPlainText().isEmpty()){
        return;
    }
    QXmppMessage msg("",getBareJid(),message);   /* from , to , message body */
    msg.setStamp(QDateTime::currentDateTime());
    client->sendPacket(msg);
    QString msgText("<font color='blue'>"+tr("我")+" </font><font color='green'>"+
                    QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")+
                    "</font><font color='black'>"+
                    message+"</font>");
    ui->announcement->append(msgText);
    chatsLog->addChatLog(bareJid,msgText);
    ui->messageEdit->clear();
}

void PersonalChatDlg::messageReceived(const QXmppMessage& msg)
{

    QDateTime dt=msg.stamp().toLocalTime();
    QString msgText("<font color='blue'>"+getDisplayName()+" </font>"+
                    "<font color='green'>"+dt.toString("yyyy/MM/dd hh:mm:ss")+
                    "</font><font color='black'>"+msg.body()+
                    "</font>");
    ui->announcement->append(msgText);
    chatsLog->addChatLog(bareJid,msgText);
}

void PersonalChatDlg::setBareJid(const QString& BareJid)
{
    bareJid = BareJid;
    if(isClientSet&&resourcesList.isEmpty())
    {
        setResourcesList();
    }
}

void PersonalChatDlg::setFriendSign(const QString &FriendSign)
{
    ui->friend_sign->setText(FriendSign);
}

void PersonalChatDlg::setFriendAvatar(const QImage& avatar)
{
    if(avatar.isNull())
    {
        ui->avatar->setPixmap(QPixmap(":/pixmap/image/avatar.png"));
        ui->friendAvatar->setPixmap(QPixmap(":/pixmap/image/avatar.png"));
    }else{
        ui->avatar->setPixmap(QPixmap::fromImage(avatar));
        ui->friendAvatar->setPixmap(QPixmap::fromImage(avatar));

    }
}

void PersonalChatDlg::setOwnAvatar(const QImage& avatar)
{
    ui->ownAvatar->setPixmap(QPixmap::fromImage(avatar));
}

void PersonalChatDlg::setFacePanel(FacePanel *facepanel)
{
    facePanel=facepanel;
}

void PersonalChatDlg::setChatsLog(ChatsLog *chatslog)
{
    chatsLog=chatslog;
}

void PersonalChatDlg::setJob(QXmppTransferJob *Job)
{
    job=Job;
    ui->saveFileButton->show();
    ui->refuseButton->show();
    ui->fileTip->setPalette(Qt::blue);
    ui->fileTip->setText(getBareJid()+tr("向您发送了文件：")+job->fileName());
    connect(job, SIGNAL(error(QXmppTransferJob::Error)),
            this, SLOT(slotError(QXmppTransferJob::Error)));


    connect(job, SIGNAL(finished()),
            this, SLOT(slotFinished()));

    connect(job, SIGNAL(progress(qint64,qint64)),
            this, SLOT(slotProgress(qint64,qint64)));

    qDebug()<<"~~~~~~~~~~~~~~~~~~fileReceived~~~~~~~~~~~~~~~~~~~~~~~";
}

void PersonalChatDlg::setAnimationUrls(QHash<QMovie *, QUrl> &Urls)
{
    urls=&Urls;
    ui->messageEdit->addAnimation(urls);
    ui->announcement->addAnimation(urls);
    ui->log->addAnimation(urls);
}

void PersonalChatDlg::slotError(QXmppTransferJob::Error error)
{
    qDebug() << "Transmission failed:" << error;
    ui->fileTip->setText(tr("文件传送失败:")+error);
}

void PersonalChatDlg::slotProgress(qint64 done, qint64 total)
{
    qDebug() << "Transmission progress:" << done << "/" << total;
    ui->fileTip->setText(tr("文件传送进度：")+QString::number(done) + "/" + QString::number(total));
}

void PersonalChatDlg::slotFinished()
{
    qDebug() << "Transmission finished";
    ui->fileTip->setText(tr("文件传送完成！"));
}

void PersonalChatDlg::setDisplayName(const QString& DisplayName)
{
    displayName = DisplayName;
    //setWindowTitle(QString(tr("聊天： %1")).arg(bareJid));
    ui->friend_name->setText(DisplayName);
}

void PersonalChatDlg::setQXmppClient(QXmppClient* Client)
{
    client = Client;
    isClientSet=true;
    if(!bareJid.isEmpty()&&resourcesList.isEmpty())
    {
        setResourcesList();
    }
}

void PersonalChatDlg::setResourcesList()
{
    resourcesList =client->rosterManager().getResources(getBareJid());
}

QString PersonalChatDlg::getBareJid() const
{
    return bareJid;
}

QString PersonalChatDlg::getDisplayName() const
{
    return displayName;
}

void PersonalChatDlg::on_closeButton_clicked()
{
    close();
}

void PersonalChatDlg::on_sendMessageButton_clicked()
{
    sendMessage();
}

void PersonalChatDlg::on_sendfileButton_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this);

    if(!filename.isEmpty()){
        setResourcesList();
        if(!resourcesList.isEmpty()){
            QXmppTransferManager *transferManager= client->findExtension<QXmppTransferManager>();
            // TO FIX:
            //   transferManager->setProxyOnly(true);
           sendFileJob = transferManager->sendFile(getBareJid()+"/"+resourcesList[0],filename);
           if(sendFileJob!=NULL)
           {
               connect(sendFileJob, SIGNAL(error(QXmppTransferJob::Error)),
                       this, SLOT(slotError(QXmppTransferJob::Error)));
               connect(sendFileJob, SIGNAL(finished()),
                       this, SLOT(slotFinished()));
               connect(sendFileJob, SIGNAL(progress(qint64,qint64)),
                       this, SLOT(slotProgress(qint64,qint64)));

           }
        }else{
            QMessageBox::warning(this,tr("发送失败!"),tr("请确认对方是否在线，您只能向在线用户发送文件!"),
                                 QMessageBox::Ok);
        }
    }
}

void PersonalChatDlg::on_saveFileButton_clicked()
{
    QString fileName;
    fileName=QFileDialog::getSaveFileName(this, tr("Save File"),job->fileName());
    if (!fileName.isNull()){
        QFile *file=new QFile(fileName);
        file->open(QIODevice::WriteOnly);
        job->accept(file);
    }else{
        job->abort();

    }
    ui->saveFileButton->hide();
    ui->refuseButton->hide();
    ui->fileTip->clear();
}

void PersonalChatDlg::on_refuseButton_clicked()
{
    job->abort();
    ui->saveFileButton->hide();
    ui->refuseButton->hide();
}

void PersonalChatDlg::on_announcement_customContextMenuRequested(const QPoint &pos)
{
    static QMenu *cmenu=NULL;
    if(cmenu==NULL){
        qDebug()<<"create menu!";
        cmenu=new QMenu(ui->announcement);
        QAction *selecteAll = cmenu->addAction("全选");
        cmenu->addSeparator();
        QAction *copyText=cmenu->addAction("复制");
        QAction *clearAll = cmenu->addAction("清空");
        connect(selecteAll,SIGNAL(triggered()),ui->announcement,SLOT(selectAll()));
        connect(copyText,SIGNAL(triggered()),ui->announcement,SLOT(copy()));
        connect(clearAll,SIGNAL(triggered()),ui->announcement,SLOT(clear()));
    }
    cmenu->exec(QCursor::pos());//在当前鼠标位置显示
}

void PersonalChatDlg::on_face_clicked()
{
    int x= this->x();
    int y= this->y();
    facePanel->move(x-150,y+115);
    connect(facePanel,SIGNAL(faceClicked(QString)),
            this,SLOT(faceClicked(QString)));
    facePanel->show();
}

void PersonalChatDlg::on_chatLogButton_clicked(bool checked)
{
    if(checked)
    {
        setFixedSize(1000,493);
        ui->log->start();
        ui->log->append(chatsLog->getChatLog(bareJid));
    }else{
        setFixedSize(570,493);
        ui->log->clear();
        ui->log->stop();
    }

}
