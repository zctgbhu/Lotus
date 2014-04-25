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

#include "giftextedit.h"

GifTextEdit::GifTextEdit(QWidget *parent) :
    QTextEdit(parent),refreshTimer(this)
{
}
void GifTextEdit::addAnimation(QHash<QMovie*, QUrl> *Urls)
{
    urls=Urls;
    connect(&refreshTimer, SIGNAL(timeout()),
                this, SLOT(animate()));
    refreshTimer.start(80);

}

void GifTextEdit::stop()
{
    refreshTimer.stop();
}

void GifTextEdit::start()
{
    refreshTimer.stop();
    refreshTimer.start(80);
}
void GifTextEdit::animate()
{
    //qDebug()<<"animate";
    foreach (QMovie *movie, urls->keys()) {
        document()->addResource(QTextDocument::ImageResource,
            urls->value(movie), movie->currentPixmap());
    }
    setLineWrapColumnOrWidth(0);
        // // causes reload
        //qDebug()<<"lineWrapColumnOrWidth="<< lineWrapColumnOrWidth();
}
