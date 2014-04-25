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

bool Lotus::eventFilter(QObject *obj, QEvent *event)
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
                //this->showMinimized();
                this->hide();
            }else{
                ui->btn_min->setPixmap(QPixmap(":/pixmap/image/miniButton.png"));
            }
        } else {
            return QObject::eventFilter(obj, event);
        }
        return false;
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
                exitEvent();
            }else{
                ui->btn_close->setPixmap(QPixmap(":/pixmap/image/closeButton.png"));
            }
        } else {
            return QObject::eventFilter(obj, event);
        }
        return false;
    }

    if(obj==ui->avatar)
    {
        if(event->type() == QEvent::MouseButtonDblClick)
        {
            //qDebug()<<"click avatar";
            emit clickAvatarLabel();
        }
        return false;
    }
    if(obj==ui->aboutButton)
    {
        if(event->type() == QEvent::MouseButtonRelease)
        {
            //qDebug()<<"click avatar";
            clickAboutButton();
        }
        return false;
    }
    if(obj==systemTrayIcon)
    {
        if(event->type() == QEvent::MouseButtonPress||event->type() == QEvent::MouseButtonRelease)
        {
            qDebug()<<"systemTrayIcon systemTrayIcon";
            this->showNormal();
        }
        return false;
    }
    if(obj==ui->friendLabel||obj==ui->roomLabel||obj==ui->otherLabel)
    {
        if(event->type() == QEvent::MouseButtonRelease)
        {
            setWindowOpacity(1);
        }
        return false;
    }
    if(obj==ui->searchButton)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *me = (QMouseEvent *)event;
            dragPos = me->globalPos() - frameGeometry().topLeft();
        }else if(event->type() == QEvent::MouseButtonRelease)
        {
            setWindowOpacity(1);
        }
        return false;
    }
    if(event->type() == QEvent::Enter){

         qDebug()<<"eventFilter~~~~Enter";
         if(lotusStatus==-1){
             if(topHide==true){
         topHideAnimation->setStartValue(QSize( lotusWidth,this->size().height()));
         topHideAnimation->setEndValue(QSize( lotusWidth, lotusHeight));
         topHideAnimation->start();
         lotusStatus=1;
             }else if(rightHide==true){
                 rightHideAnimation->setStartValue(QRect(this->x(),this->y(), this->size().width(),lotusHeight));
                 rightHideAnimation->setEndValue(QRect(QApplication::desktop()->width()-lotusWidth,this->y(), lotusWidth, lotusHeight));
                 rightHideAnimation->start();
                 lotusStatus=1;
             }
         }

    }else if(event->type() == QEvent::Leave){
            qDebug()<<"eventFilter~~~~Leave";
            if(lotusStatus==1){
                if(topHide==true){
            topHideAnimation->setStartValue(QSize( lotusWidth, lotusHeight));
            topHideAnimation->setEndValue(QSize( lotusWidth, 5));
            topHideAnimation->start();
            lotusStatus=-1;
                }else if(rightHide==true){
                    rightHideAnimation->setStartValue(QRect(this->x(),this->y(),lotusWidth,lotusHeight));
                    rightHideAnimation->setEndValue(QRect(QApplication::desktop()->width()-5,this->y(),5,lotusHeight));
                    rightHideAnimation->start();
                    lotusStatus=-1;
                }
            }

        }
    //return QObject::eventFilter(obj, event);
    return false;


}

void Lotus::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragPos = event->globalPos() - frameGeometry().topLeft();

        event->accept();
    }

}

void Lotus::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton )
    {
        move(event->globalPos() - dragPos);
        if(this->y()<50)
        {
            move(this->x(),0);
            topHide=true;
            lotusStatus=1;
        }else{
            topHide=false;
            lotusStatus=0;
        }
        if(this->x() > QApplication::desktop()->width()-this->width()-50)
        {
            move(QApplication::desktop()->width()-this->width(),this->y());
            rightHide=true;
            lotusStatus=1;
        }else{
            rightHide=false;
            //lotusStatus=0;
        }
        setWindowOpacity(0.5);
    }
    event->accept();

}

void Lotus::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        setWindowOpacity(1);
    }
    event->accept();
}


