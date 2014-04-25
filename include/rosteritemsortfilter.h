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

#ifndef ROSTERITEMSORTFILTER_H
#define ROSTERITEMSORTFILTER_H

#include <QSortFilterProxyModel>

class RosterItemSortFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit RosterItemSortFilter(QObject *parent = 0);
    
signals:
    
public slots:
    void sortByName(bool);

private:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    bool filterAcceptsRow(int, const QModelIndex&) const;

    bool m_sortByName;
};

#endif // ROSTERITEMSORTFILTER_H
