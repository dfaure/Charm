/*
  WorkLogReport.cpp

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

#include "WorkLogReport.h"
#include "ViewHelpers.h"

#include <QDomDocument>
#include <QDomElement>
#include <QLocale>
#include <QMap>
#include <QPushButton>
#include <QUrl>

namespace {

struct TaskDay {
    int seconds = 0;
    EventIdList events;
};

typedef QMap<TaskId, TaskDay> TasksOfDay;

QDomElement addElement(QDomElement &parent, const QString &tag, const QString &text = QString(),
                       const QString &className = QString())
{
    QDomElement element = parent.ownerDocument().createElement(tag);
    if (!className.isEmpty())
        element.setAttribute(QStringLiteral("class"), className);
    if (!text.isEmpty())
        element.appendChild(parent.ownerDocument().createTextNode(text));
    parent.appendChild(element);
    return element;
}

QDomElement addCell(QDomElement &row, const QString &tag, const QString &text,
                    const QString &align = QStringLiteral("left"),
                    const QString &className = QString())
{
    QDomElement cell = addElement(row, tag, text, className);
    cell.setAttribute(QStringLiteral("align"), align);
    cell.setAttribute(QStringLiteral("valign"), QStringLiteral("top"));
    return cell;
}

const QString Nowrap = QStringLiteral("nowrap");
const QString MutedNowrap = QStringLiteral("muted nowrap");

QDomElement addNarrowHeader(QDomElement &row, const QString &text,
                            const QString &align = QStringLiteral("left"))
{
    QDomElement cell = addCell(row, QStringLiteral("th"), text, align, Nowrap);
    cell.setAttribute(QStringLiteral("width"), QStringLiteral("1%"));
    return cell;
}

}

WorkLogReport::WorkLogReport(QWidget *parent)
    : ReportPreviewWindow(parent)
{
    saveToXmlButton()->hide();
    saveToTextButton()->hide();
    uploadButton()->hide();
    connect(this, &ReportPreviewWindow::anchorClicked, this, &WorkLogReport::slotLinkClicked);
}

WorkLogReport::~WorkLogReport()
{
}

void WorkLogReport::setReportProperties(const QDate &start, const QDate &end, TaskId rootTask)
{
    m_start = start;
    m_end = end;
    m_rootTask = rootTask;
    slotUpdate();
}

void WorkLogReport::slotUpdate()
{
    EventIdList matchingEvents = DATAMODEL->eventsThatStartInTimeFrame(m_start, m_end);
    if (m_rootTask != 0)
        matchingEvents = Charm::filteredBySubtree(matchingEvents, m_rootTask);
    matchingEvents = Charm::eventIdsSortedBy(matchingEvents, Charm::SortOrder::StartTime);

    QMap<QDate, TasksOfDay> days;
    QMap<TaskId, int> secondsPerTask;
    int totalSeconds = 0;
    for (const EventId id : std::as_const(matchingEvents)) {
        const Event &event = DATAMODEL->eventForId(id);
        TaskDay &taskDay = days[event.startDateTime().date()][event.taskId()];
        taskDay.seconds += event.duration();
        taskDay.events.append(id);
        secondsPerTask[event.taskId()] += event.duration();
        totalSeconds += event.duration();
    }

    const QLocale locale;
    QDomDocument doc = createReportTemplate();
    QDomElement body = doc.documentElement().firstChildElement(QStringLiteral("body"));

    addElement(body, QStringLiteral("h1"), tr("Work Log"));
    addElement(body, QStringLiteral("h3"),
               tr("Report for %1, %2 %3 (%4 to %5)")
               .arg(CONFIGURATION.user.name(),
                    locale.standaloneMonthName(m_start.month(), QLocale::LongFormat),
                    QString::number(m_start.year()),
                    m_start.toString(Qt::TextDate),
                    m_end.addDays(-1).toString(Qt::TextDate)));
    QDomElement previousLink = addElement(body, QStringLiteral("a"), tr("<Previous Month>"));
    previousLink.setAttribute(QStringLiteral("href"), QStringLiteral("Previous"));
    body.appendChild(doc.createTextNode(QStringLiteral(" ")));
    QDomElement nextLink = addElement(body, QStringLiteral("a"), tr("<Next Month>"));
    nextLink.setAttribute(QStringLiteral("href"), QStringLiteral("Next"));
    addElement(body, QStringLiteral("h4"),
               tr("Total: %1, %2 days with entries")
               .arg(hoursAndMinutes(totalSeconds)).arg(days.size()));

    QDomElement table = addElement(body, QStringLiteral("table"));
    table.setAttribute(QStringLiteral("width"), QStringLiteral("100%"));
    table.setAttribute(QStringLiteral("align"), QStringLiteral("left"));
    table.setAttribute(QStringLiteral("cellpadding"), QStringLiteral("3"));
    table.setAttribute(QStringLiteral("cellspacing"), QStringLiteral("0"));

    QDomElement headerRow = addElement(table, QStringLiteral("tr"), QString(),
                                       QStringLiteral("header_row"));
    addNarrowHeader(headerRow, tr("Day"));
    addNarrowHeader(headerRow, tr("Total"), QStringLiteral("right"));
    addNarrowHeader(headerRow, tr("Task"));
    addNarrowHeader(headerRow, tr("Hours"), QStringLiteral("right"));
    addCell(headerRow, QStringLiteral("th"), tr("Description"));
    addNarrowHeader(headerRow, tr("Time"));

    bool alternate = false;
    for (QDate day = m_start; day < m_end; day = day.addDays(1)) {
        const bool weekend = day.dayOfWeek() >= Qt::Saturday;
        const TasksOfDay tasks = days.value(day);
        if (weekend && tasks.isEmpty())
            continue;

        if (day.dayOfWeek() == Qt::Monday || day == m_start) {
            QDomElement weekRow = addElement(table, QStringLiteral("tr"), QString(),
                                             QStringLiteral("week_row"));
            QDomElement weekCell = addCell(weekRow, QStringLiteral("td"),
                                           tr("Week %1").arg(day.weekNumber()));
            weekCell.setAttribute(QStringLiteral("colspan"), 6);
            alternate = false;
        }
        const QString rowClass = alternate ? QStringLiteral("alternate_row") : QString();
        alternate = !alternate;

        QDomElement row = addElement(table, QStringLiteral("tr"), QString(), rowClass);
        int daySeconds = 0;
        int dayRows = 0;
        for (const TaskDay &taskDay : tasks) {
            daySeconds += taskDay.seconds;
            dayRows += taskDay.events.size();
        }
        QDomElement dayCell = addCell(row, QStringLiteral("td"), QString(),
                                      QStringLiteral("left"), Nowrap);
        dayCell.setAttribute(QStringLiteral("rowspan"), qMax(1, dayRows));
        const QString dayNumber = QStringLiteral("%1").arg(day.day(), 2, 10, QLatin1Char('0'));
        if (weekend) {
            addElement(dayCell, QStringLiteral("span"), dayNumber, QStringLiteral("muted"));
        } else {
            addElement(dayCell, QStringLiteral("b"), dayNumber);
        }
        dayCell.appendChild(doc.createTextNode(QStringLiteral(" ")));
        addElement(dayCell, QStringLiteral("span"),
                   locale.dayName(day.dayOfWeek(), QLocale::ShortFormat), QStringLiteral("muted"));

        if (tasks.isEmpty()) {
            QDomElement emptyCell = addCell(row, QStringLiteral("td"), tr("no entries"));
            emptyCell.setAttribute(QStringLiteral("colspan"), 5);
            emptyCell.setAttribute(QStringLiteral("class"), QStringLiteral("muted"));
            continue;
        }

        QDomElement totalCell = addCell(row, QStringLiteral("td"), QString(),
                                        QStringLiteral("right"), Nowrap);
        totalCell.setAttribute(QStringLiteral("rowspan"), dayRows);
        addElement(totalCell, QStringLiteral("b"), hoursAndMinutes(daySeconds));

        bool firstRowOfDay = true;
        for (auto it = tasks.constBegin(); it != tasks.constEnd(); ++it) {
            const Task &task = DATAMODEL->getTask(it.key());
            bool firstRowOfTask = true;
            for (const EventId eventId : it->events) {
                if (!firstRowOfDay)
                    row = addElement(table, QStringLiteral("tr"), QString(), rowClass);
                firstRowOfDay = false;
                if (firstRowOfTask) {
                    QDomElement taskCell = addCell(row, QStringLiteral("td"),
                                                   DATAMODEL->smartTaskName(task),
                                                   QStringLiteral("left"), Nowrap);
                    taskCell.setAttribute(QStringLiteral("rowspan"), it->events.size());
                    QDomElement hoursCell = addCell(row, QStringLiteral("td"),
                                                    hoursAndMinutes(it->seconds),
                                                    QStringLiteral("right"), Nowrap);
                    hoursCell.setAttribute(QStringLiteral("rowspan"), it->events.size());
                }
                firstRowOfTask = false;

                const Event &event = DATAMODEL->eventForId(eventId);
                QDomElement descriptionCell = addCell(row, QStringLiteral("td"), QString());
                const QString comment = event.comment().trimmed();
                if (comment.isEmpty()) {
                    addElement(descriptionCell, QStringLiteral("div"), tr("no description"),
                               QStringLiteral("muted"));
                } else {
                    const QStringList lines = comment.split(QLatin1Char('\n'));
                    for (const QString &line : lines) {
                        if (!line.trimmed().isEmpty())
                            addElement(descriptionCell, QStringLiteral("div"), line.trimmed());
                    }
                }
                addCell(row, QStringLiteral("td"),
                        QStringLiteral("%1-%2")
                        .arg(locale.toString(event.startDateTime().time(), QLocale::ShortFormat),
                             locale.toString(event.endDateTime().time(), QLocale::ShortFormat)),
                        QStringLiteral("left"), MutedNowrap);
            }
        }
    }

    QDomElement totalsRow = addElement(table, QStringLiteral("tr"), QString(),
                                       QStringLiteral("header_row"));
    addCell(totalsRow, QStringLiteral("th"), tr("Total:"));
    addCell(totalsRow, QStringLiteral("th"), hoursAndMinutes(totalSeconds), QStringLiteral("right"));
    addCell(totalsRow, QStringLiteral("th"), QString());
    addCell(totalsRow, QStringLiteral("th"), QString());
    addCell(totalsRow, QStringLiteral("th"), QString());
    addCell(totalsRow, QStringLiteral("th"), QString());

    if (!secondsPerTask.isEmpty()) {
        QMap<TaskId, int> daysPerTask;
        for (const TasksOfDay &tasks : std::as_const(days)) {
            for (auto it = tasks.constBegin(); it != tasks.constEnd(); ++it)
                daysPerTask[it.key()] += 1;
        }

        addElement(body, QStringLiteral("br"));
        addElement(body, QStringLiteral("h4"), tr("Totals by task"));
        QDomElement taskTable = addElement(body, QStringLiteral("table"));
        taskTable.setAttribute(QStringLiteral("width"), QStringLiteral("100%"));
        taskTable.setAttribute(QStringLiteral("align"), QStringLiteral("left"));
        taskTable.setAttribute(QStringLiteral("cellpadding"), QStringLiteral("3"));
        taskTable.setAttribute(QStringLiteral("cellspacing"), QStringLiteral("0"));
        QDomElement taskHeaderRow = addElement(taskTable, QStringLiteral("tr"), QString(),
                                               QStringLiteral("header_row"));
        addCell(taskHeaderRow, QStringLiteral("th"), tr("Task"));
        addNarrowHeader(taskHeaderRow, tr("Days"), QStringLiteral("right"));
        addNarrowHeader(taskHeaderRow, tr("Hours"), QStringLiteral("right"));
        int taskIndex = 0;
        for (auto it = secondsPerTask.constBegin(); it != secondsPerTask.constEnd(); ++it) {
            QDomElement row = addElement(taskTable, QStringLiteral("tr"), QString(),
                                         taskIndex++ % 2 ? QStringLiteral("alternate_row") : QString());
            addCell(row, QStringLiteral("td"), DATAMODEL->smartTaskName(DATAMODEL->getTask(it.key())));
            addCell(row, QStringLiteral("td"), QString::number(daysPerTask.value(it.key())),
                    QStringLiteral("right"), Nowrap);
            addCell(row, QStringLiteral("td"), hoursAndMinutes(it.value()), QStringLiteral("right"),
                    Nowrap);
        }
        QDomElement taskTotalsRow = addElement(taskTable, QStringLiteral("tr"), QString(),
                                               QStringLiteral("header_row"));
        addCell(taskTotalsRow, QStringLiteral("th"), tr("Total:"));
        addCell(taskTotalsRow, QStringLiteral("th"), QString::number(days.size()),
                QStringLiteral("right"));
        addCell(taskTotalsRow, QStringLiteral("th"), hoursAndMinutes(totalSeconds),
                QStringLiteral("right"));
    }

    QTextDocument report;
    report.setDefaultStyleSheet(Charm::reportStylesheet(palette()));
    report.setHtml(doc.toString());
    setDocument(&report);
}

void WorkLogReport::slotLinkClicked(const QUrl &which)
{
    const int direction = which.toString() == QLatin1String("Previous") ? -1 : 1;
    setReportProperties(m_start.addMonths(direction), m_end.addMonths(direction), m_rootTask);
}
