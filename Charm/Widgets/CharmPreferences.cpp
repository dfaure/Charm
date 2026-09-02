/*
  CharmPreferences.cpp

  This file is part of Charm, a task-based time tracking application.

  Copyright (C) 2014-2019 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com

  Author: Frank Osterfeld <frank.osterfeld@kdab.com>
  Author: Mathias Hasselmann <mathias.hasselmann@kdab.com>

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

#include "CharmPreferences.h"
#include "ApplicationCore.h"
#include "MessageBox.h"

#include "Core/Configuration.h"
#include "Core/Dates.h"
#include "Idle/IdleDetector.h"
#include "Lotsofcake/Configuration.h"

#include <QCheckBox>
#include <QColorDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QInputDialog>
#include <QPixmap>
#include <QPushButton>
#include <QSignalBlocker>

CharmPreferences::CharmPreferences(const Configuration &config, QWidget *parent_)
    : QDialog(parent_)
{
    m_ui.setupUi(this);
    const bool haveIdleDetection = ApplicationCore::instance().idleDetector()->available();
    const bool haveCommandInterface = (ApplicationCore::instance().commandInterface() != nullptr);
    const bool httpJobPossible = Lotsofcake::Configuration().isConfigured();

    m_ui.lbWarnUnuploadedTimesheets->setVisible(httpJobPossible);
    m_ui.cbWarnUnuploadedTimesheets->setVisible(httpJobPossible);
    m_ui.cbIdleDetection->setEnabled(haveIdleDetection);
    m_ui.lbIdleDetection->setEnabled(haveIdleDetection);
    m_ui.cbIdleDetection->setChecked(config.detectIdling && m_ui.cbIdleDetection->isEnabled());
    m_ui.cbWarnUnuploadedTimesheets->setChecked(config.warnUnuploadedTimesheets);
    m_ui.cbRequestEventComment->setChecked(config.requestEventComment);
    m_ui.lbCommandInterface->setVisible(haveCommandInterface);
    m_ui.cbEnableCommandInterface->setEnabled(haveCommandInterface);
    m_ui.cbEnableCommandInterface->setVisible(haveCommandInterface);
    m_ui.cbEnableCommandInterface->setChecked(haveCommandInterface
                                              && config.enableCommandInterface);

    connect(m_ui.cbWarnUnuploadedTimesheets, &QCheckBox::toggled,
            this, &CharmPreferences::slotWarnUnuploadedChanged);
    connect(m_ui.cbEnableOvertimeCounter, &QCheckBox::toggled,
            this, &CharmPreferences::slotOvertimeCounterToggled);
    connect(m_ui.deContractStartDate, &QDateEdit::dateChanged,
            this, &CharmPreferences::slotContractStartDateChanged);
    connect(m_ui.pbTimeTrackerBackgroundColor, &QPushButton::clicked,
            this, &CharmPreferences::slotChooseTimeTrackerBackgroundColor);
    connect(m_ui.pbResetTimeTrackerBackgroundColor, &QPushButton::clicked,
            this, &CharmPreferences::slotResetTimeTrackerBackgroundColor);

    m_timeTrackerBackgroundColor = config.timeTrackerBackgroundColor;
    updateTimeTrackerBackgroundColorButton();

    // this would not need a switch, but i hate casting enums to int:
    switch (config.timeTrackerFontSize) {
    case Configuration::TimeTrackerFont_Small:
        m_ui.cbTimeTrackerFontSize->setCurrentIndex(0);
        break;
    case Configuration::TimeTrackerFont_Regular:
        m_ui.cbTimeTrackerFontSize->setCurrentIndex(1);
        break;
    case Configuration::TimeTrackerFont_Large:
        m_ui.cbTimeTrackerFontSize->setCurrentIndex(2);
        break;
    }

    switch (config.durationFormat) {
    case Configuration::Minutes:
        m_ui.cbDurationFormat->setCurrentIndex(0);
        break;
    case Configuration::Decimal:
        m_ui.cbDurationFormat->setCurrentIndex(1);
        break;
    }

    switch (config.toolButtonStyle) {
    case Qt::ToolButtonIconOnly:
        m_ui.cbToolButtonStyle->setCurrentIndex(0);
        break;
    case Qt::ToolButtonTextOnly:
        m_ui.cbToolButtonStyle->setCurrentIndex(1);
        break;
    case Qt::ToolButtonTextUnderIcon:
        m_ui.cbToolButtonStyle->setCurrentIndex(2);
        break;
    case Qt::ToolButtonTextBesideIcon:
        m_ui.cbToolButtonStyle->setCurrentIndex(3);
        break;
    case Qt::ToolButtonFollowStyle:
        m_ui.cbToolButtonStyle->setCurrentIndex(4);
        break;
    }

    m_ui.sbNumberOfTaskSelectorEntries->setValue(config.numberOfTaskSelectorEntries);

    const WorkTimeContract &contract = config.workTimeContract;
    const bool trackOvertime = !contract.isEmpty();
    m_ui.cbEnableOvertimeCounter->setChecked(trackOvertime);
    if (trackOvertime) {
        m_ui.sbHoursPerWeek->setValue(contract.periods.first().hoursPerWeek);
        m_ui.deContractStartDate->setDate(contract.startDate());
    } else {
        m_ui.deContractStartDate->setDate(Charm::weekDayInWeekOf(Qt::Monday,
                                                                 QDate::currentDate()));
    }
    slotOvertimeCounterToggled(trackOvertime);

    // resize( minimumSize() );
}

CharmPreferences::~CharmPreferences()
{
}

bool CharmPreferences::detectIdling() const
{
    return m_ui.cbIdleDetection->isChecked();
}

bool CharmPreferences::warnUnuploadedTimesheets() const
{
    return m_ui.cbWarnUnuploadedTimesheets->isChecked();
}

bool CharmPreferences::requestEventComment() const
{
    return m_ui.cbRequestEventComment->isChecked();
}

bool CharmPreferences::enableCommandInterface() const
{
    return m_ui.cbEnableCommandInterface->isChecked();
}

int CharmPreferences::numberOfTaskSelectorEntries() const
{
    return m_ui.sbNumberOfTaskSelectorEntries->value();
}

WorkTimeContract CharmPreferences::workTimeContract() const
{
    WorkTimeContract contract;
    if (m_ui.cbEnableOvertimeCounter->isChecked())
        contract.periods << ContractPeriod { m_ui.deContractStartDate->date(),
                                             m_ui.sbHoursPerWeek->value() };
    return contract;
}

void CharmPreferences::slotOvertimeCounterToggled(bool enabled)
{
    m_ui.lbHoursPerWeek->setEnabled(enabled);
    m_ui.sbHoursPerWeek->setEnabled(enabled);
    m_ui.lbContractStartDate->setEnabled(enabled);
    m_ui.deContractStartDate->setEnabled(enabled);
}

void CharmPreferences::slotContractStartDateChanged(const QDate &date)
{
    const QDate monday = Charm::weekDayInWeekOf(Qt::Monday, date);
    if (monday == date)
        return;
    QSignalBlocker blocker(m_ui.deContractStartDate);
    m_ui.deContractStartDate->setDate(monday);
}

Configuration::DurationFormat CharmPreferences::durationFormat() const
{
    switch (m_ui.cbDurationFormat->currentIndex()) {
    case 0:
        return Configuration::Minutes;
    case 1:
        return Configuration::Decimal;
    default:
        Q_ASSERT(!"Unexpected combobox item for DurationFormat");
    }
    return Configuration::Minutes;
}

Configuration::TimeTrackerFontSize CharmPreferences::timeTrackerFontSize() const
{
    switch (m_ui.cbTimeTrackerFontSize->currentIndex()) {
    case 0:
        return Configuration::TimeTrackerFont_Small;
        break;
    case 1:
        return Configuration::TimeTrackerFont_Regular;
        break;
    case 2:
        return Configuration::TimeTrackerFont_Large;
        break;
    default:
        Q_ASSERT(false);   // somebody added an item
    }
    // always return something, to avoid compiler warning:
    return Configuration::TimeTrackerFont_Regular;
}

QColor CharmPreferences::timeTrackerBackgroundColor() const
{
    return m_timeTrackerBackgroundColor;
}

void CharmPreferences::slotChooseTimeTrackerBackgroundColor()
{
    const QColor initial = m_timeTrackerBackgroundColor.isValid()
                           ? m_timeTrackerBackgroundColor
                           : palette().window().color();
    const QColor color = QColorDialog::getColor(initial, this,
                                                tr("Time Tracker Window Background"));
    if (!color.isValid()) // dialog cancelled
        return;
    m_timeTrackerBackgroundColor = color;
    updateTimeTrackerBackgroundColorButton();
}

void CharmPreferences::slotResetTimeTrackerBackgroundColor()
{
    m_timeTrackerBackgroundColor = QColor();
    updateTimeTrackerBackgroundColorButton();
}

void CharmPreferences::updateTimeTrackerBackgroundColorButton()
{
    QPushButton *button = m_ui.pbTimeTrackerBackgroundColor;
    if (m_timeTrackerBackgroundColor.isValid()) {
        const qreal dpr = devicePixelRatioF();
        QPixmap swatch(button->iconSize() * dpr);
        swatch.setDevicePixelRatio(dpr);
        swatch.fill(m_timeTrackerBackgroundColor);
        button->setIcon(QIcon(swatch));
        button->setText(m_timeTrackerBackgroundColor.name());
    } else {
        button->setIcon(QIcon());
        button->setText(tr("Choose..."));
    }
    m_ui.pbResetTimeTrackerBackgroundColor->setEnabled(m_timeTrackerBackgroundColor.isValid());
}

Qt::ToolButtonStyle CharmPreferences::toolButtonStyle() const
{
    switch (m_ui.cbToolButtonStyle->currentIndex()) {
    case 0:
        return Qt::ToolButtonIconOnly;
        break;
    case 1:
        return Qt::ToolButtonTextOnly;
        break;
    case 2:
        return Qt::ToolButtonTextUnderIcon;
        break;
    case 3:
        return Qt::ToolButtonTextBesideIcon;
        break;
    case 4:
        return Qt::ToolButtonFollowStyle;
        break;
    default:
        Q_ASSERT(false);   // somebody added an item
    }
    // always return something, to avoid compiler warning:
    return Qt::ToolButtonIconOnly;
}

void CharmPreferences::slotWarnUnuploadedChanged(bool enabled)
{
    if (!Lotsofcake::Configuration().isConfigured())
        return;

    if (!enabled) {
        const int response = MessageBox::question(this,
                                                  tr("Bill is sad :(."),
                                                  tr(
                                                      "Bill has always been misunderstood. All he really wants is your reports, and even when he doesn't get them you only have to evade him once per hour. I'm sure you want to keep Bill's gentle reminders?"),
                                                  tr("Mmmmkay"),
                                                  tr("No, Stop Bill"),
                                                  QMessageBox::Yes);
        if (response == QMessageBox::Yes)
            m_ui.cbWarnUnuploadedTimesheets->setCheckState(Qt::Checked);
    }
}
