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
#ifndef ADAPT_MODELSELECTION_HPP
#define ADAPT_MODELSELECTION_HPP

#include <QWizardPage>

namespace Ui
{
class ModelSelection;
}

namespace SpeechControl
{

class AcousticModel;
namespace Wizards
{
namespace Pages
{

class ModelSelection : public QWizardPage
{
    Q_OBJECT

public:
    explicit ModelSelection (QWidget* parent = 0);
    AcousticModel* model();
    ~ModelSelection();
    virtual void initializePage();
    virtual void cleanupPage();
    virtual bool isComplete() const;

private slots:
    void on_comboBoxModel_currentIndexChanged(const int index);

private:
    Ui::ModelSelection* ui;
    AcousticModel* m_model;
};

}
}
}

#endif // ADAPT_MODELSELECTION_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
