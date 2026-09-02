/*
  WorkTimeContract.cpp

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

#include "WorkTimeContract.h"

#include <QCoreApplication>
#include <QStringList>

#include <algorithm>

QDate WorkTimeContract::startDate() const
{
    return periods.isEmpty() ? QDate() : periods.first().since;
}

int WorkTimeContract::expectedSeconds(const QDate &endExclusive) const
{
    double seconds = 0.0;
    for (int i = 0; i < periods.size(); ++i) {
        const ContractPeriod &period = periods.at(i);
        const QDate nextStart = (i + 1 < periods.size()) ? periods.at(i + 1).since : endExclusive;
        const QDate end = qMin(nextStart, endExclusive);
        const int weeks = qMax(0, static_cast<int>(period.since.daysTo(end) / 7));
        seconds += weeks * period.hoursPerWeek * 3600;
    }
    return qRound(seconds);
}

bool WorkTimeContract::isValid(QString *errorMessage) const
{
    QDate previous;
    for (const ContractPeriod &period : periods) {
        QString error;
        if (!period.since.isValid())
            error = QCoreApplication::translate("WorkTimeContract", "Invalid date.");
        else if (period.since.dayOfWeek() != Qt::Monday)
            error = QCoreApplication::translate("WorkTimeContract", "%1 is not a Monday.")
                    .arg(period.since.toString(Qt::ISODate));
        else if (previous.isValid() && period.since == previous)
            error = QCoreApplication::translate("WorkTimeContract", "%1 is listed twice.")
                    .arg(period.since.toString(Qt::ISODate));
        else if (previous.isValid() && period.since < previous)
            error = QCoreApplication::translate("WorkTimeContract",
                                                "The periods are not sorted by date.");
        else if (period.hoursPerWeek < 0)
            error = QCoreApplication::translate("WorkTimeContract",
                                                "The number of hours per week cannot be negative.");
        if (!error.isEmpty()) {
            if (errorMessage)
                *errorMessage = error;
            return false;
        }
        previous = period.since;
    }
    return true;
}

QString WorkTimeContract::toString() const
{
    QStringList entries;
    entries.reserve(periods.size());
    for (const ContractPeriod &period : periods) {
        entries << period.since.toString(Qt::ISODate) + QLatin1Char('=')
            + QString::number(period.hoursPerWeek);
    }
    return entries.join(QLatin1Char(';'));
}

WorkTimeContract WorkTimeContract::fromString(const QString &string)
{
    WorkTimeContract contract;
    const QStringList entries = string.split(QLatin1Char(';'), Qt::SkipEmptyParts);
    for (const QString &entry : entries) {
        const int separator = entry.indexOf(QLatin1Char('='));
        if (separator == -1)
            continue;
        ContractPeriod period;
        period.since = QDate::fromString(entry.left(separator).trimmed(), Qt::ISODate);
        if (!period.since.isValid())
            continue;
        bool ok = false;
        period.hoursPerWeek = entry.mid(separator + 1).trimmed().toDouble(&ok);
        if (!ok || period.hoursPerWeek < 0)
            continue;
        contract.periods << period;
    }
    std::sort(contract.periods.begin(), contract.periods.end(),
              [](const ContractPeriod &lhs, const ContractPeriod &rhs) {
        return lhs.since < rhs.since;
    });
    return contract;
}
