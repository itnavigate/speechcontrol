/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
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
 *  along with SpeechControl.  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef SPEECHCONTROL_DICTATION_SERVICE_HPP
#define SPEECHCONTROL_DICTATION_SERVICE_HPP

#include <app/macros.hpp>
#include <app/services/module.hpp>

namespace SpeechControl
{

namespace Dictation
{

class Service : public SpeechControl::Services::AbstractModule
{
    Q_OBJECT
    Q_ENUMS (SafetyMode)
    Q_DISABLE_COPY (Service)
    SC_SINGLETON (Service)

protected:
    virtual void deinitialize();
    virtual void initialize();

public:
    enum SafetyMode {
        Undefined = -1,
        Enabled,
        Active,
        Inactive,
        Disabled
    };

    bool isSafetyModeActive() const;
    bool isSafetyModeEnabled() const;
    SafetyMode safetyMode() const;
    virtual bool isEnabled() const;
    virtual QString id() const;
    virtual QPixmap pixmap() const;
    virtual QString name() const;
    virtual ~Service();

public slots:
    void handleText (const QString& p_text);
    void setSafetyMode (const SafetyMode& p_mode);
};

}

}

#endif // SPEECHCONTROL_DICTATION_SERVICE_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
