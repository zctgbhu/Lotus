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

#include "facepanel.h"
#include "ui_facepanel.h"

FacePanel::FacePanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FacePanel)
{
    ui->setupUi(this);
    faceDefaultPath= QDesktopServices::storageLocation(QDesktopServices::HomeLocation)+"/.Lotus/";
    ui->faceTable->setIconSize(QSize(30,30));
    ui->faceTable->setColumnCount(defaultColumn);
    ui->faceTable->setRowCount(1);
    ui->faceTable->setMouseTracking(true);

    connect(ui->faceTable, SIGNAL(itemClicked(QTableWidgetItem*)),
            this, SLOT(onItemClicked(QTableWidgetItem*)));
    connect(ui->faceTable,SIGNAL(itemEntered(QTableWidgetItem*)),
            this,SLOT(onItemEntered(QTableWidgetItem*)));
    loadFace();
    setWindowOpacity(1);
    setWindowFlags(Qt::FramelessWindowHint);
    facepreview = new FacePreview();
}

void FacePanel::onItemEntered(QTableWidgetItem *item)
{qDebug()<<"onItemEntered";
    facepreview->stop();
    facepreview->setFaceId(item->data(Qt::UserRole).toString());
    facepreview->start();
    int faceHeight=facepreview->getFaceSize().height();
    int faceWidth=facepreview->getFaceSize().width();
    int row=item->row();
    int column=item->column();
    int x= this->x();
    int y= this->y();
    facepreview->move(x-faceWidth-50+column*30,y-faceHeight-50+row*30);

    facepreview->show();

}

void FacePanel::onItemClicked(QTableWidgetItem *item)
{
    this->hide();
    facepreview->stop();
    facepreview->close();
    emit faceClicked(item->data(Qt::UserRole).toString());
}

void FacePanel::loadFace()
{
    for ( unsigned int i = 0; i < 105; i++ )
    {
        int row = i / defaultColumn;
        int column = i % defaultColumn;

        if (row > ui->faceTable->rowCount()-1)
        {
            ui->faceTable->insertRow(ui->faceTable->rowCount());
        }

        QTableWidgetItem *item = new QTableWidgetItem();
        item->setIcon(QIcon(faceDefaultPath+"/face/" + QString::number(i+1) + ".gif"));
        item->setData(Qt::UserRole, QString::number(i+1));

        ui->faceTable->setItem(row, column,item);
    }
}
FacePanel::~FacePanel()
{
    delete ui;
}
void FacePanel::leaveEvent(QEvent *)
{
    this->hide();
    facepreview->stop();
    facepreview->close();
}
