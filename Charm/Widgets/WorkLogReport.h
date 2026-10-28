/*
  WorkLogReport.h

  This file is part of Charm, a task-based time tracking application.

  Copyright (C) 2026 Dominik Schmidt <dev@dominik-schmidt.de>

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

#ifndef WORKLOGREPORT_H
#define WORKLOGREPORT_H

#include <Core/Task.h>

#include "ReportPreviewWindow.h"

#include <QDate>

class QUrl;

/** A month, day by day: hours per task and the event descriptions of each day. */
class WorkLogReport : public ReportPreviewWindow
{
    Q_OBJECT

public:
    explicit WorkLogReport(QWidget *parent = nullptr);
    ~WorkLogReport() override;

    void setReportProperties(const QDate &start, const QDate &end, TaskId rootTask);

private Q_SLOTS:
    void slotLinkClicked(const QUrl &which);

private:
    void slotUpdate() override;

    QDate m_start;
    QDate m_end;
    TaskId m_rootTask = {};
};

#endif
