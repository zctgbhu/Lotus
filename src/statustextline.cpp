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

#include "statustextline.h"

StatusTextLine::StatusTextLine(QWidget *parent) :
    QLineEdit(parent)
{

    //setText(tr("编辑此处更新状态/签名"));
}

void StatusTextLine::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    QLineEdit::selectAll();
}

void StatusTextLine::mousePressEvent(QMouseEvent *event)
{
    QLineEdit::mousePressEvent(event);
    QLineEdit::selectAll();
}
