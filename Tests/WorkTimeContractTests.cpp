/*
  WorkTimeContractTests.cpp

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

#include "WorkTimeContractTests.h"

#include "Core/CharmDataModel.h"
#include "Core/Dates.h"
#include "Core/OvertimeCounter.h"
#include "Core/WorkTimeContract.h"

#include <QtTest/QtTest>

static const int OneHour = 3600;

// all of these are Mondays
static const QDate Jan1st2024(2024, 1, 1);
static const QDate Jan8th2024(2024, 1, 8);
static const QDate Feb5th2024(2024, 2, 5);
static const QDate Jun2nd2025(2025, 6, 2);

void WorkTimeContractTests::testEmptyContract()
{
    const WorkTimeContract contract;
    QVERIFY(contract.isEmpty());
    QVERIFY(!contract.startDate().isValid());
    QCOMPARE(contract.expectedSeconds(Feb5th2024), 0);
    QVERIFY(contract.isValid());
    QCOMPARE(contract.toString(), QString());
}

void WorkTimeContractTests::testExpectedSecondsOnePeriod()
{
    const WorkTimeContract contract { { { Jan1st2024, 35.0 } } };
    QCOMPARE(contract.startDate(), Jan1st2024);
    QCOMPARE(contract.expectedSeconds(Jan1st2024), 0);
    QCOMPARE(contract.expectedSeconds(Jan8th2024), 35 * OneHour);
    QCOMPARE(contract.expectedSeconds(Feb5th2024), 5 * 35 * OneHour);
    // fractional hours per week
    const WorkTimeContract halfHours { { { Jan1st2024, 32.5 } } };
    QCOMPARE(halfHours.expectedSeconds(Feb5th2024), qRound(5 * 32.5 * OneHour));
}

void WorkTimeContractTests::testExpectedSecondsSeveralPeriods()
{
    const WorkTimeContract contract { { { Jan1st2024, 35.0 }, { Feb5th2024, 20.0 } } };
    // before the rate change, only the first period counts
    QCOMPARE(contract.expectedSeconds(Jan8th2024), 35 * OneHour);
    // exactly at the rate change
    QCOMPARE(contract.expectedSeconds(Feb5th2024), 5 * 35 * OneHour);
    // one week after the rate change
    QCOMPARE(contract.expectedSeconds(Feb5th2024.addDays(7)), (5 * 35 + 20) * OneHour);
}

void WorkTimeContractTests::testExpectedSecondsEdgeCases()
{
    // a contract that only starts in the future yields 0, not a negative number
    const WorkTimeContract future { { { Jun2nd2025, 35.0 } } };
    QCOMPARE(future.expectedSeconds(Jan8th2024), 0);
    // a period entirely before the start date of the next one is still counted in full
    const WorkTimeContract contract { { { Jan1st2024, 35.0 }, { Jan8th2024, 40.0 } } };
    QCOMPARE(contract.expectedSeconds(Jan1st2024), 0);
    QCOMPARE(contract.expectedSeconds(Jan8th2024), 35 * OneHour);
}

void WorkTimeContractTests::testRoundTrip()
{
    const WorkTimeContract contract { { { Jan1st2024, 35.0 }, { Jun2nd2025, 32.5 } } };
    QCOMPARE(contract.toString(), QStringLiteral("2024-01-01=35;2025-06-02=32.5"));
    QVERIFY(WorkTimeContract::fromString(contract.toString()) == contract);
}

void WorkTimeContractTests::testRoundTripInCommaDecimalLocale()
{
    const QLocale previousLocale = QLocale();
    QLocale::setDefault(QLocale::German);
    const WorkTimeContract contract { { { Jan1st2024, 32.5 } } };
    QCOMPARE(contract.toString(), QStringLiteral("2024-01-01=32.5"));
    QVERIFY(WorkTimeContract::fromString(contract.toString()) == contract);
    QLocale::setDefault(previousLocale);
}

void WorkTimeContractTests::testFromStringMalformed()
{
    QVERIFY(WorkTimeContract::fromString(QString()).isEmpty());
    QVERIFY(WorkTimeContract::fromString(QStringLiteral("nonsense")).isEmpty());
    QVERIFY(WorkTimeContract::fromString(QStringLiteral("2024-01-01")).isEmpty());
    QVERIFY(WorkTimeContract::fromString(QStringLiteral("2024-01-01=abc")).isEmpty());
    QVERIFY(WorkTimeContract::fromString(QStringLiteral("2024-13-01=35")).isEmpty());
    QVERIFY(WorkTimeContract::fromString(QStringLiteral("2024-01-01=-35")).isEmpty());
    // a truncated value keeps the entries that do parse
    const WorkTimeContract truncated
        = WorkTimeContract::fromString(QStringLiteral("2024-01-01=35;2025-06-"));
    QCOMPARE(truncated.periods.size(), 1);
    QCOMPARE(truncated.startDate(), Jan1st2024);
    // unsorted input comes back sorted
    const WorkTimeContract unsorted
        = WorkTimeContract::fromString(QStringLiteral("2025-06-02=32.5;2024-01-01=35"));
    QCOMPARE(unsorted.startDate(), Jan1st2024);
    QVERIFY(unsorted.isValid());
}

void WorkTimeContractTests::testIsValid()
{
    QString error;
    const WorkTimeContract sorted { { { Jan1st2024, 35.0 }, { Jun2nd2025, 32.5 } } };
    QVERIFY(sorted.isValid(&error));
    QVERIFY(error.isEmpty());

    const WorkTimeContract notAMonday { { { QDate(2024, 1, 2), 35.0 } } };
    QVERIFY(!notAMonday.isValid(&error));
    QVERIFY(!error.isEmpty());

    const WorkTimeContract unsorted { { { Jun2nd2025, 32.5 }, { Jan1st2024, 35.0 } } };
    QVERIFY(!unsorted.isValid());

    const WorkTimeContract duplicate { { { Jan1st2024, 35.0 }, { Jan1st2024, 32.5 } } };
    QVERIFY(!duplicate.isValid());

    const WorkTimeContract negativeHours { { { Jan1st2024, -1.0 } } };
    QVERIFY(!negativeHours.isValid());

    const WorkTimeContract invalidDate { { { QDate(), 35.0 } } };
    QVERIFY(!invalidDate.isValid());
}

void WorkTimeContractTests::testMondayOfWeek()
{
    // the Monday of the week of each day of the week of Jan 1st 2024 (a Monday)
    for (int day = 0; day < 7; ++day)
        QCOMPARE(Charm::weekDayInWeekOf(Qt::Monday, Jan1st2024.addDays(day)), Jan1st2024);
    // a Sunday belongs to the week that is still running
    QCOMPARE(Charm::weekDayInWeekOf(Qt::Monday, Jan1st2024.addDays(6)), Jan1st2024);
    QCOMPARE(Charm::weekDayInWeekOf(Qt::Monday, Jan8th2024), Jan8th2024);
}

static Event makeEvent(EventId id, const QDate &date, int seconds)
{
    Event event;
    event.setId(id);
    event.setTaskId(1);
    event.setStartDateTime(QDateTime(date, QTime(9, 0)));
    event.setEndDateTime(QDateTime(date, QTime(9, 0)).addSecs(seconds));
    return event;
}

void WorkTimeContractTests::testOvertime()
{
    CharmDataModel model;
    Task task(1, QStringLiteral("Task"));
    model.setAllTasks(TaskList() << task);

    const WorkTimeContract contract { { { Jan1st2024, 35.0 } } };

    // no events at all: the whole expected time is missing
    OvertimeResult result = computeOvertime(&model, contract, Jan8th2024);
    QVERIFY(result.valid);
    QCOMPARE(result.periodStart, Jan1st2024);
    QCOMPARE(result.periodEndExclusive, Jan8th2024);
    QCOMPARE(result.trackedSeconds, 0);
    QCOMPARE(result.expectedSeconds, 35 * OneHour);
    QCOMPARE(result.balanceSeconds(), -35 * OneHour);

    EventList events;
    events << makeEvent(1, Jan1st2024, 40 * OneHour);
    // an event in the still running week does not count
    events << makeEvent(2, Jan8th2024, 8 * OneHour);
    model.setAllEvents(events);

    // still Monday of the second week, so only the first week counts
    result = computeOvertime(&model, contract, Jan8th2024);
    QCOMPARE(result.trackedSeconds, 40 * OneHour);
    QCOMPARE(result.balanceSeconds(), 5 * OneHour);

    // on the Sunday of the second week, that week is still running
    result = computeOvertime(&model, contract, Jan8th2024.addDays(6));
    QCOMPARE(result.periodEndExclusive, Jan8th2024);
    QCOMPARE(result.trackedSeconds, 40 * OneHour);

    // an empty contract, and a contract that covers no full week yet
    QVERIFY(!computeOvertime(&model, WorkTimeContract(), Jan8th2024).valid);
    QVERIFY(!computeOvertime(&model, contract, Jan1st2024).valid);
}

QTEST_MAIN(WorkTimeContractTests)
