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
void Lotus::setView()
{
    int ScreenW = QApplication::desktop()->width();

    move(ScreenW-400,100);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    lotusWidth=this->size().width();
    lotusHeight=this->size().height();
    lotusStatus=0;
    topHideAnimation = new QPropertyAnimation(this,"size");
    topHideAnimation->setDuration(1000);

    rightHideAnimation = new QPropertyAnimation(this,"geometry");
    rightHideAnimation->setDuration(1000);

    ui->btn_close->setStyleSheet("border-image:url(:/pixmap/image/closeButton.png)");
    ui->btn_min->setStyleSheet("border-image:url(:/pixmap/image/miniButton.png)");
    ui->friendSearch->setStyleSheet("border-image:url(:/pixmap/image/friendSearch.png)");
    ui->bg->setStyleSheet("border-image:url(:/pixmap/image/lotus_bg.png)");
    ui->friendLabel->setStyleSheet("border-image:url(:/pixmap/image/friendLabel.png)");
    ui->roomLabel->setStyleSheet("border-image:url(:/pixmap/image/roomLabel.png)");
    ui->otherLabel->setStyleSheet("border-image:url(:/pixmap/image/recentLabel.png)");
    ui->search->setStyleSheet("border-image: url(:/pixmap/image/transparent.png)");
    ui->statusText->setStyleSheet("border-image: url(:/pixmap/image/transparent.png)");
    ui->statusText->setText(tr("编辑此处更新状态/签名"));
}
