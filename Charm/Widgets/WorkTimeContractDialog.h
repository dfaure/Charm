/*
  WorkTimeContractDialog.h

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

#ifndef WORKTIMECONTRACTDIALOG_H
#define WORKTIMECONTRACTDIALOG_H

#include <QDialog>

#include "Core/WorkTimeContract.h"
#include "ui_WorkTimeContractDialog.h"

class QDateEdit;
class QDoubleSpinBox;

/** Edits the list of (since-Monday, hours per week) periods. */
class WorkTimeContractDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WorkTimeContractDialog(const WorkTimeContract &contract, QWidget *parent = nullptr);
    ~WorkTimeContractDialog() override;

    /** The edited contract, sorted by date. An empty one means the counter is off. */
    WorkTimeContract workTimeContract() const;

private Q_SLOTS:
    void slotAddPeriod();
    void slotAccept();

private:
    struct PeriodRow
    {
        QWidget *widget;
        QDateEdit *dateEdit;
        QDoubleSpinBox *hoursSpinBox;
    };

    void addRow(const ContractPeriod &period);
    void removeRow(const QWidget *rowWidget);

    Ui::WorkTimeContractDialog m_ui;
    QVector<PeriodRow> m_rows;
};

#endif
