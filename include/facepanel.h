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

#ifndef FACEPANEL_H
#define FACEPANEL_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QDesktopServices>
#include "facepreview.h"
#include <QDebug>
namespace Ui {
class FacePanel;
}

class FacePanel : public QDialog
{
    Q_OBJECT
    
public:
    explicit FacePanel(QWidget *parent = 0);
    ~FacePanel();
signals:
    void faceClicked(QString id);
protected:
    void leaveEvent(QEvent *);
private slots:
    void onItemClicked(QTableWidgetItem *item);
    void onItemEntered(QTableWidgetItem *item);
private:
    Ui::FacePanel *ui;
    QString faceDefaultPath;
    FacePreview *facepreview;
    static const int defaultColumn = 15;


    void loadFace();
};
#endif // FACEPANEL_H
