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

#ifndef USERINIT_HPP
#define USERINIT_HPP

#include <QWizardPage>

namespace Ui
{
class UserInitialization;
}

namespace SpeechControl
{
namespace Windows
{
namespace Wizards
{
namespace Pages
{

class UserInitialization : public QWizardPage
{
    Q_OBJECT

public:
    explicit UserInitialization (QWidget* parent = 0);
    ~UserInitialization();

private slots:
    virtual void initializePage();
    virtual void clearPage();
    virtual bool validatePage();

private:
    void updateLanguages();
    Ui::UserInitialization* ui;
};

}
}
}
}
#endif // USERINIT_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
