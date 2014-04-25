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

#include "rosteritemsortfilter.h"

RosterItemSortFilter::RosterItemSortFilter(QObject *parent) :
    QSortFilterProxyModel(parent),
    m_sortByName(false)
{
    setDynamicSortFilter(true);
    setFilterRole(Qt::DisplayRole);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}
bool RosterItemSortFilter::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if(m_sortByName)
    {
        int compare = left.data().toString().compare(right.data().toString(), Qt::CaseInsensitive);
        if(compare < 0)
            return true;
        else
            return false;
    }
    return false;
}

bool RosterItemSortFilter::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    if(!filterRegExp().isEmpty())
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    return false;
}

void RosterItemSortFilter::sortByName(bool sortByName)
{
    m_sortByName = sortByName;
    invalidate();
}
