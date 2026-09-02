/*
  OvertimeCounter.cpp

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

#include "OvertimeCounter.h"
#include "CharmDataModel.h"
#include "Dates.h"
#include "WorkTimeContract.h"

OvertimeResult computeOvertime(const CharmDataModel *model, const WorkTimeContract &contract,
                               const QDate &today)
{
    OvertimeResult result;
    if (contract.isEmpty())
        return result;

    // A Sunday belongs to the week that is still running, so it is not counted.
    result.periodEndExclusive = Charm::weekDayInWeekOf(Qt::Monday, today);
    result.periodStart = contract.startDate();
    if (result.periodStart >= result.periodEndExclusive)
        return result;

    const EventIdList events = model->eventsThatStartInTimeFrame(result.periodStart,
                                                                 result.periodEndExclusive);
    for (EventId id : events)
        result.trackedSeconds += model->eventForId(id).duration();

    result.expectedSeconds = contract.expectedSeconds(result.periodEndExclusive);
    result.valid = true;
    return result;
}
