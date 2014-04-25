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
void PersonalChatDlg::setView()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    ui->refuseButton->hide();
    ui->saveFileButton->hide();
    ui->announcement->setReadOnly(true);
    ui->btn_close->installEventFilter(this);
    ui->btn_min->installEventFilter(this);
    ui->closeButton->installEventFilter(this);
    ui->sendMessageButton->installEventFilter(this);
    ui->saveFileButton->installEventFilter(this);
    ui->sendfileButton->installEventFilter(this);
    ui->refuseButton->installEventFilter(this);
    ui->bg->setStyleSheet("border-image:url(:/pixmap/image/chatDlg_Bg.png)");
    ui->btn_close->setStyleSheet("border-image:url(:/pixmap/image/closeButton.png)");
    ui->btn_min->setStyleSheet("border-image:url(:/pixmap/image/miniButton.png)");
    ui->avatar->setScaledContents(true);
    ui->friendAvatar->setScaledContents(true);
    ui->ownAvatar->setScaledContents(true);
}
