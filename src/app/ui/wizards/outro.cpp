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

#include "outro.hpp"
#include "ui_wizardbase-outro.h"

using namespace SpeechControl::Wizards::Pages;

ConclusionPage::ConclusionPage (const QString& summary) :
    QWizardPage(),
    ui (new Ui::Conclusion)
{
    ui->setupUi (this);
    this->setLayout(ui->verticalLayout);
    this->setTitle ("Wizard Complete");
    ui->labelSummary->setText (summary);
}

ConclusionPage::~ConclusionPage()
{
    delete ui;
}

#include "ui/outro.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
