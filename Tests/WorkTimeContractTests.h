/*
  WorkTimeContractTests.h

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

#ifndef WORKTIMECONTRACTTESTS_H
#define WORKTIMECONTRACTTESTS_H

#include <QObject>

class WorkTimeContractTests : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testEmptyContract();
    void testExpectedSecondsOnePeriod();
    void testExpectedSecondsSeveralPeriods();
    void testExpectedSecondsEdgeCases();
    void testRoundTrip();
    void testRoundTripInCommaDecimalLocale();
    void testFromStringMalformed();
    void testIsValid();
    void testMondayOfWeek();
    void testOvertime();
};

#endif
