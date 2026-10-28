/*
  MonthlyTimesheetConfigurationDialog.cpp

  This file is part of Charm, a task-based time tracking application.

  Copyright (C) 2014-2019 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com

  Author: Frank Osterfeld <frank.osterfeld@kdab.com>

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

#include "MonthlyTimesheetConfigurationDialog.h"

#include <QSettings>

#include "SelectTaskDialog.h"
#include "ViewHelpers.h"

#include "CharmCMake.h"

#include "MonthlyTimesheet.h"
#include "WorkLogReport.h"

#include "ui_MonthlyTimesheetConfigurationDialog.h"

MonthlyTimesheetConfigurationDialog::MonthlyTimesheetConfigurationDialog(QWidget *parent)
    : ReportConfigurationDialog(parent)
    , m_ui(new Ui::MonthlyTimesheetConfigurationDialog)
{
    setWindowTitle(tr("Monthly Timesheet"));

    m_ui->setupUi(this);
    connect(m_ui->buttonBox, &QDialogButtonBox::accepted,
            this, &MonthlyTimesheetConfigurationDialog::accept);
    connect(m_ui->buttonBox, &QDialogButtonBox::rejected,
            this, &MonthlyTimesheetConfigurationDialog::reject);

    connect(m_ui->comboBoxMonth, &QComboBox::currentIndexChanged,
            this, &MonthlyTimesheetConfigurationDialog::slotMonthComboItemSelected);
    connect(m_ui->pushButtonChooseTask, &QPushButton::clicked,
            this, &MonthlyTimesheetConfigurationDialog::slotSelectTask);
    connect(m_ui->radioButtonSubtasks, &QRadioButton::toggled,
            this, &MonthlyTimesheetConfigurationDialog::slotSubtasksToggled);

    const QLocale locale;
    for (int month = 1; month <= 12; ++month)
        m_ui->comboBoxOtherMonth->addItem(locale.standaloneMonthName(month, QLocale::LongFormat));

    slotStandardTimeSpansChanged();
    connect(ApplicationCore::instance().dateChangeWatcher(), &DateChangeWatcher::dateChanged,
            this, &MonthlyTimesheetConfigurationDialog::slotStandardTimeSpansChanged);

    // load settings:
    QSettings settings;
    if (settings.contains(MetaKey_TimesheetActiveOnly)) {
        m_ui->checkBoxActiveOnly->setChecked(settings.value(MetaKey_TimesheetActiveOnly).toBool());
    } else {
        m_ui->checkBoxActiveOnly->setChecked(true);
    }
    if (settings.value(MetaKey_MonthlyReportWorkLog).toBool())
        m_ui->radioButtonWorkLog->setChecked(true);
}

MonthlyTimesheetConfigurationDialog::~MonthlyTimesheetConfigurationDialog()
{
}

void MonthlyTimesheetConfigurationDialog::setDefaultMonth(int yearOfMonth, int month)
{
    m_ui->comboBoxOtherMonth->setCurrentIndex(month - 1);
    m_ui->spinBoxYear->setValue(yearOfMonth);
    m_ui->comboBoxMonth->setCurrentIndex(m_monthInfo.size() - 1);
}

void MonthlyTimesheetConfigurationDialog::accept()
{
    // save settings:
    QSettings settings;
    settings.setValue(MetaKey_TimesheetActiveOnly,
                      m_ui->checkBoxActiveOnly->isChecked());
    settings.setValue(MetaKey_TimesheetRootTask,
                      m_rootTask);
    settings.setValue(MetaKey_MonthlyReportWorkLog,
                      m_ui->radioButtonWorkLog->isChecked());

    QDialog::accept();
}

void MonthlyTimesheetConfigurationDialog::showReportPreviewDialog()
{
    const int index = m_ui->comboBoxMonth->currentIndex();
    QDate start;
    if (index == m_monthInfo.size() - 1) {
        start = QDate(m_ui->spinBoxYear->value(), m_ui->comboBoxOtherMonth->currentIndex() + 1, 1);
    } else {
        start = m_monthInfo[index].timespan.first;
    }
    const QDate end = start.addMonths(1);
    if (m_ui->radioButtonWorkLog->isChecked()) {
        auto report = new WorkLogReport();
        report->setReportProperties(start, end, m_rootTask);
        report->show();
        return;
    }
    bool activeOnly = m_ui->checkBoxActiveOnly->isChecked();
    auto report = new MonthlyTimeSheetReport();
    report->setReportProperties(start, end, m_rootTask, activeOnly);
    report->show();
}

void MonthlyTimesheetConfigurationDialog::showEvent(QShowEvent *)
{
    QSettings settings;

    // we only want to do this once a backend is loaded, and we ignore
    // the saved root task if it does not exist anymore
    if (settings.contains(MetaKey_TimesheetRootTask)) {
        TaskId root = settings.value(MetaKey_TimesheetRootTask).toInt();
        const TaskTreeItem &item = DATAMODEL->taskTreeItem(root);
        if (item.isValid()) {
            m_rootTask = root;
            m_ui->labelTaskName->setText(DATAMODEL->fullTaskName(item.task()));
            m_ui->radioButtonSubtasks->setChecked(true);
        }
    }
}

void MonthlyTimesheetConfigurationDialog::slotSubtasksToggled(bool checked)
{
    if (checked && m_rootTask == 0)
        slotSelectTask();

    if (!checked) {
        m_rootTask = 0;
        m_ui->labelTaskName->setText(tr("(no task chosen)"));
    }
}

void MonthlyTimesheetConfigurationDialog::slotStandardTimeSpansChanged()
{
    const TimeSpans timeSpans;
    m_monthInfo = timeSpans.last4Months();
    NamedTimeSpan custom = {
        tr("Other month:"),
        timeSpans.thisMonth().timespan,
        Range
    };
    m_monthInfo << custom;

    const QLocale locale;
    m_ui->comboBoxMonth->clear();
    for (int i = 0; i < m_monthInfo.size(); ++i) {
        const QDate month = m_monthInfo[i].timespan.first;
        const QString monthName = QStringLiteral("%1 %2")
                                  .arg(locale.standaloneMonthName(month.month(), QLocale::LongFormat))
                                  .arg(month.year());
        QString label = m_monthInfo[i].name;
        if (i == 0) {
            label = tr("This month (%1)").arg(monthName);
        } else if (i == 1) {
            label = tr("Last month (%1)").arg(monthName);
        } else if (i < m_monthInfo.size() - 1) {
            label = monthName;
        }
        m_ui->comboBoxMonth->addItem(label);
    }
    // the first month the presets do not cover
    const QDate beforePresets = m_monthInfo[m_monthInfo.size() - 2].timespan.first.addMonths(-1);
    m_ui->comboBoxOtherMonth->setCurrentIndex(beforePresets.month() - 1);
    m_ui->spinBoxYear->setValue(beforePresets.year());
    // Set current index to "Last Month" as that's what you'll usually want
    m_ui->comboBoxMonth->setCurrentIndex(1);
}

void MonthlyTimesheetConfigurationDialog::slotMonthComboItemSelected(int index)
{
    const bool other = index == m_monthInfo.size() - 1;
    m_ui->comboBoxOtherMonth->setVisible(other);
    m_ui->spinBoxYear->setVisible(other);
}

void MonthlyTimesheetConfigurationDialog::slotSelectTask()
{
    SelectTaskDialog dialog(this);
    dialog.setNonTrackableSelectable();
    dialog.setNonValidSelectable();
    if (dialog.exec()) {
        m_rootTask = dialog.selectedTask();
        const TaskTreeItem &item = DATAMODEL->taskTreeItem(m_rootTask);
        m_ui->labelTaskName->setText(DATAMODEL->fullTaskName(item.task()));
    } else {
        if (m_rootTask == 0)
            m_ui->radioButtonAllTasks->setChecked(true);
    }
}
