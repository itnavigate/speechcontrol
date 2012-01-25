/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *
 * SpeechControl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * SpeechControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with SpeechControl; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <QDebug>

#include "addselect.hpp"
#include "wizard.hpp"
#include "ui_additionselectionpage.h"

using SpeechControl::Wizards::Pages::AdditionSelectionPage;

AdditionSelectionPage::AdditionSelectionPage(QWidget *parent) :
    QWizardPage(parent),
    m_ui(new Ui::AdditionSelectionPage)
{
    m_ui->setupUi(this);
    registerField("selection.wiki",m_ui->radioButtonWiki);
    registerField("selection.file",m_ui->radioButtonFile);
}

AdditionSelectionPage::~AdditionSelectionPage()
{
    delete m_ui;
}
