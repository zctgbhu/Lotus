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
#ifndef LOGINLOADWIN_H
#define LOGINLOADWIN_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QtGui>
#include "lotus.h"
#include "QXmppClient.h"
#include "accountcache.h"
namespace Ui {
class LoginloadWin;
}

class LoginloadWin : public QWidget
{
    Q_OBJECT
    
public:
    explicit LoginloadWin(QWidget *parent = 0);
    ~LoginloadWin();
    
    void login();
    void setUserName(QString name);
    void setPassWord(QString pw);
    void setPWRemember(bool PWRemember);
    void setAccountCache(AccountCache ACache);
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void loginTimeOut();
    void cancel_login();
    void disappear();
    void connected();
    void loginError(QXmppClient::Error);
private:
    Ui::LoginloadWin *ui;
    QList<QPixmap> *loading;
    int i;
    QPoint dragPos;
    float oparcity;
     QTimer timer;
    QTimer loginTimer;
    QString userName;
    QString passWord;
    QXmppClient  client;
    QXmppConfiguration config;

    Lotus lotusMainUI;
    void addAccountToCache();
    AccountCache accountCache;
    bool pwremember;
signals:
    void cancel();
};

#endif // LOGINLOADWIN_H
