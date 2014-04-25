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

#include "groupchatdlg.h"
#include "ui_groupchatdlg.h"
void GroupChatDlg::setView()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    ui->group_announcement->setReadOnly(true);
    ui->btn_close->installEventFilter(this);
    ui->btn_min->installEventFilter(this);
    ui->closeButton->installEventFilter(this);
    ui->sendButton->installEventFilter(this);
    ui->bg->setStyleSheet("border-image:url(:/pixmap/image/chatDlg_Bg.png)");
    ui->btn_close->setStyleSheet("border-image:url(:/pixmap/image/closeButton.png)");
    ui->btn_min->setStyleSheet("border-image:url(:/pixmap/image/miniButton.png)");
    ui->group_vatar->setScaledContents(true);
    ui->group_vatar->setPixmap(QPixmap(":/pixmap/image/roomDefaultAvatar.png"));
}
