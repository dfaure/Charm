/*
  WorkTimeContract.h

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

#ifndef WORKTIMECONTRACT_H
#define WORKTIMECONTRACT_H

#include <QDate>
#include <QString>
#include <QVector>

/** The expected work time, from @ref since until the next period starts. */
struct ContractPeriod
{
    QDate since; // always a Monday
    double hoursPerWeek = 0.0;

    bool operator==(const ContractPeriod &other) const
    {
        return since == other.since && qFuzzyCompare(hoursPerWeek, other.hoursPerWeek);
    }
};

/** The contractually expected work time over time, as a list of periods.

    The periods start on Mondays and are sorted by date, so that the expected
    work time for a number of full weeks is a simple sum.
*/
class WorkTimeContract
{
public:
    bool isEmpty() const { return periods.isEmpty(); }

    /** The Monday the counting starts on. */
    QDate startDate() const;

    /** Expected work time, in seconds, from startDate() until @p endExclusive (a Monday). */
    int expectedSeconds(const QDate &endExclusive) const;

    /** Checks that the periods are Mondays, sorted, unique, and have non-negative hours. */
    bool isValid(QString *errorMessage = nullptr) const;

    /** The storage format, "2024-01-01=35;2025-06-02=32.5". Locale-independent. */
    QString toString() const;
    /** Parses toString() output, silently dropping entries that do not parse. */
    static WorkTimeContract fromString(const QString &string);

    bool operator==(const WorkTimeContract &other) const { return periods == other.periods; }
    bool operator!=(const WorkTimeContract &other) const { return !operator==(other); }

    QVector<ContractPeriod> periods; // sorted by `since`
};

#endif
