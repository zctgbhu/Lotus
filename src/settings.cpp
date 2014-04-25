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

#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    ui->cancalButton->installEventFilter(this);
    ui->okButton->installEventFilter(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setAttribute(Qt::WA_TranslucentBackground);
    //ui->bg->setStyleSheet("border-image:url(:/pixmap/image/bg.png)");
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_cancalButton_clicked()
{
    close();
}

void Settings::setDefaultServer()
{
    int index;
    index=ui->serverAddress->findText(serverAddress);
    if(index==-1){
        ui->serverAddress->addItem(serverAddress);
        index=ui->serverAddress->findText(serverAddress);
    }
    ui->serverAddress->setCurrentIndex(index);
}

void Settings::on_okButton_clicked()
{
    serverAddress=ui->serverAddress->currentText().replace(" ","");
    close();
}


void Settings::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragPos = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }

}

void Settings::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton )
    {
        move(event->globalPos() - dragPos);
        setWindowOpacity(0.5);
    }
    event->accept();

}

void Settings::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        setWindowOpacity(1);
    }
    event->accept();
}

bool Settings::eventFilter(QObject *obj, QEvent *event)
{
    if(obj==ui->cancalButton||obj==ui->okButton)
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
