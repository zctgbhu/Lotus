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

#include "facepreview.h"
#include "ui_facepreview.h"

FacePreview::FacePreview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FacePreview)
{
    faceDir = QDesktopServices::storageLocation(QDesktopServices::HomeLocation)+"/.Lotus/face/";
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}

FacePreview::~FacePreview()
{
    delete ui;
}

void FacePreview::setFaceId(const QString &id)
{
    movie.setFileName(faceDir+id+".gif");

    ui->facePreviewLabel->setMovie(&movie);
}

void FacePreview::start()
{
    movie.start();
    this->setFixedSize(movie.frameRect().size());
    ui->facePreviewLabel->setFixedSize(movie.frameRect().size());
    ui->facePreviewLabel->move(0,0);
}

void FacePreview::stop()
{
    movie.stop();
}

QSize FacePreview::getFaceSize()
{
    return movie.frameRect().size();
}
