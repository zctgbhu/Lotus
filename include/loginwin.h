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
#ifndef LOGINWIN_H
#define LOGINWIN_H

#include <QWidget>
#include <QtGui>
#include <QStyle>
#include "QXmppClient.h"
#include "QXmppLogger.h"
#include "QXmppPresence.h"
#include "QXmppMessage.h"
#include "QXmppUtils.h"
#include "QXmppVCardManager.h"
#include "QXmppRosterManager.h"
#include "loginloadwin.h"
#include "registerdialog.h"
#include "aboutdialog.h"
#include "accountcache.h"

namespace Ui {
class LoginWin;
}

class LoginWin : public QWidget
{
    Q_OBJECT
    
public:
    explicit LoginWin(QWidget *parent = 0);
    ~LoginWin();
    QString m_pw;
    QString m_user;


    void action_register();
    void action_setting();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void login();
    void disappear();

private slots:





    void setPassWordForAccountCache(const QString& userName);

private:
    Ui::LoginWin *ui;
    QPoint dragPos;
    float oparcity;
    LoginloadWin loginloadWin;
    QTimer *timer;
    AccountCache accountCache;
    void loadAccountCache();

signals:
    void login_clicked();


};

#endif // LOGINWIN_H
