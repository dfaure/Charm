/*
  OvertimeCounter.h

  This file is part of Charm, a task-based time tracking application.

  Copyright (C) 2026 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com

  Author: David Faure <david.faure@kdab.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OVERTIMECOUNTER_H
#define OVERTIMECOUNTER_H

#include <QDate>

class CharmDataModel;
class WorkTimeContract;

struct OvertimeResult
{
    bool valid = false; // false when the contract is empty or covers no full week
    QDate periodStart;
    QDate periodEndExclusive; // the Monday of the current week
    int trackedSeconds = 0;
    int expectedSeconds = 0;

    int balanceSeconds() const { return trackedSeconds - expectedSeconds; }
};

/** Tracked minus expected work time over the full weeks before the week of @p today. */
OvertimeResult computeOvertime(const CharmDataModel *model, const WorkTimeContract &contract,
                               const QDate &today);

#endif
