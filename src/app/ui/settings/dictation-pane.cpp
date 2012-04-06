/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *
 *  SpeechControl is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  SpeechControl is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with SpeechControl .  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "core.hpp"
#include "ui/main-window.hpp"
#include "services/dictation/agent.hpp"
#include "services/desktopcontrol/agent.hpp"
#include "ui_settingspane-dictation.h"

#include "dictation-pane.hpp"

using namespace SpeechControl;
using namespace SpeechControl::Windows;

DictationSettingsPane::DictationSettingsPane() :
    m_ui (new Ui::DictationSettingsPane)
{
    qDebug() << "[DictationSettingsPane::{constructor}] Building dictation settings pane...";
    m_ui->setupUi (this);
    updateUi();
    qDebug() << "[DictationSettingsPane::{constructor}] Built dictation settings pane.";
}

DictationSettingsPane::~DictationSettingsPane()
{
    delete m_ui;
}

void DictationSettingsPane::changeEvent (QEvent* e)
{
    QFrame::changeEvent (e);

    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi (this);
        break;
    default:
        break;
    }
}

QString DictationSettingsPane::title() const
{
    return "Dictation";
}

QString DictationSettingsPane::id() const
{
    return "dctn";
}

QPixmap DictationSettingsPane::pixmap() const
{
    return QIcon::fromTheme ("audio-input-microphone").pixmap (32, 32);
}

void DictationSettingsPane::restoreDefaults()
{
    Core::setConfiguration("Dictation/StartWord","start dictation");
    Core::setConfiguration("Dictation/StartWord","end dictation");
}

void DictationSettingsPane::updateUi()
{
    m_ui->checkBoxEnable->setChecked (Dictation::Agent::instance()->isEnabled() && !DesktopControl::Agent::instance()->isEnabled());
    m_ui->checkBoxEnable->setEnabled (!DesktopControl::Agent::instance()->isEnabled());
    m_ui->groupBoxKeywords->setEnabled(Dictation::Agent::instance()->isEnabled());
    m_ui->groupBoxKeywords->setChecked(Core::configuration("Dictation/UseSafeWords").toBool());
    m_ui->lineEditStart->setText(Core::configuration("Dictation/StartWord").toString());
    m_ui->lineEditEnd->setText(Core::configuration("Dictation/EndWord").toString());
}

void DictationSettingsPane::on_lineEditStart_textChanged (QString p_text)
{
    Core::setConfiguration("Dictation/StartWord",p_text.toLower());
}

void DictationSettingsPane::on_lineEditEnd_textChanged (QString p_text)
{
    Core::setConfiguration("Dictation/EndWord",p_text.toLower());
}

void DictationSettingsPane::on_checkBoxEnable_toggled (bool p_checked)
{
    if (!DesktopControl::Agent::instance()->isEnabled()) {
        Core::setConfiguration ("Dictation/Enabled", p_checked);
        if (!p_checked)
            Dictation::Agent::instance()->stop();
        Core::mainWindow()->updateUi();
    }

    updateUi();
}

void DictationSettingsPane::on_checkBoxEnableStartup_toggled (bool p_checked)
{
    if (DesktopControl::Agent::instance()->isEnabled()) {
        Core::setConfiguration ("Dictation/AutoStart", p_checked);
    }
}

#include "ui/dictation-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
