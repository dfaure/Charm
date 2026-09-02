/*
  WorkTimeContractDialog.cpp

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

#include "WorkTimeContractDialog.h"

#include "Core/Dates.h"

#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSignalBlocker>
#include <QToolButton>

#include <algorithm>

WorkTimeContractDialog::WorkTimeContractDialog(const WorkTimeContract &contract, QWidget *parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);

    connect(m_ui.pbAdd, &QPushButton::clicked, this, &WorkTimeContractDialog::slotAddPeriod);
    connect(m_ui.buttonBox, &QDialogButtonBox::accepted,
            this, &WorkTimeContractDialog::slotAccept);

    for (const ContractPeriod &period : contract.periods)
        addRow(period);
    if (m_rows.isEmpty())
        slotAddPeriod();
}

WorkTimeContractDialog::~WorkTimeContractDialog()
{
}

WorkTimeContract WorkTimeContractDialog::workTimeContract() const
{
    WorkTimeContract contract;
    contract.periods.reserve(m_rows.size());
    for (const PeriodRow &row : m_rows)
        contract.periods << ContractPeriod { row.dateEdit->date(), row.hoursSpinBox->value() };
    std::sort(contract.periods.begin(), contract.periods.end(),
              [](const ContractPeriod &lhs, const ContractPeriod &rhs) {
        return lhs.since < rhs.since;
    });
    return contract;
}

void WorkTimeContractDialog::addRow(const ContractPeriod &period)
{
    PeriodRow row;
    row.widget = new QWidget(this);
    auto *layout = new QHBoxLayout(row.widget);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(new QLabel(tr("Since"), row.widget));
    row.dateEdit = new QDateEdit(period.since, row.widget);
    row.dateEdit->setCalendarPopup(true);
    layout->addWidget(row.dateEdit);

    row.hoursSpinBox = new QDoubleSpinBox(row.widget);
    row.hoursSpinBox->setSuffix(tr(" h/week"));
    row.hoursSpinBox->setMaximum(168.0);
    row.hoursSpinBox->setSingleStep(0.5);
    row.hoursSpinBox->setValue(period.hoursPerWeek);
    layout->addWidget(row.hoursSpinBox);
    layout->addStretch();

    auto *removeButton = new QToolButton(row.widget);
    removeButton->setText(tr("Remove"));
    layout->addWidget(removeButton);

    QDateEdit *dateEdit = row.dateEdit;
    connect(dateEdit, &QDateEdit::dateChanged, this, [dateEdit](const QDate &date) {
        const QDate monday = Charm::weekDayInWeekOf(Qt::Monday, date);
        if (monday == date)
            return;
        QSignalBlocker blocker(dateEdit);
        dateEdit->setDate(monday);
    });
    const QWidget *rowWidget = row.widget;
    connect(removeButton, &QToolButton::clicked, this, [this, rowWidget] {
        removeRow(rowWidget);
    });

    m_ui.periodsLayout->addWidget(row.widget);
    m_rows << row;
}

void WorkTimeContractDialog::removeRow(const QWidget *rowWidget)
{
    const auto it = std::find_if(m_rows.begin(), m_rows.end(), [rowWidget](const PeriodRow &row) {
        return row.widget == rowWidget;
    });
    if (it == m_rows.end())
        return;
    delete it->widget;
    m_rows.erase(it);
}

void WorkTimeContractDialog::slotAddPeriod()
{
    // default to the Monday after the last period, or to the current week
    QDate since = Charm::weekDayInWeekOf(Qt::Monday, QDate::currentDate());
    double hoursPerWeek = 40.0;
    if (!m_rows.isEmpty()) {
        const PeriodRow &last = m_rows.last();
        since = last.dateEdit->date().addDays(7);
        hoursPerWeek = last.hoursSpinBox->value();
    }
    addRow(ContractPeriod { since, hoursPerWeek });
}

void WorkTimeContractDialog::slotAccept()
{
    QString error;
    if (!workTimeContract().isValid(&error)) {
        QMessageBox::warning(this, tr("Invalid Work Time"), error);
        return;
    }
    accept();
}
