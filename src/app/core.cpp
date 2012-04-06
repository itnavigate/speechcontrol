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

// Qt includes
#include <QFile>
#include <QDebug>
#include <QLibraryInfo>
#include <QSettings>
#include <qtranslator.h>
#include <QApplication>
#include <QMessageBox>

// libspchcntrl includes
#include <config.hpp>
#include <system.hpp>

// local includes
#include "factory.hpp"
#include "sessions/session.hpp"
#include "ui/main-window.hpp"
#include "ui/quickstart-wizard.hpp"
#include "indicator.hpp"
#include "services/engine.hpp"
#include "services/desktopcontrol/agent.hpp"
#include "services/desktopcontrol/service.hpp"
#include "services/dictation/agent.hpp"
#include "services/dictation/service.hpp"

#include "core.hpp"

using namespace SpeechControl;
using namespace SpeechControl::Wizards;

Core* Core::s_inst = 0;

/// @todo Add a check for the default microphone (if provided by the user).
Core::Core (int p_argc, char** p_argv, QApplication* app) : QObject (app),
    m_app (app), m_mw (0), m_settings (0), m_trnsltr (0)
{
    m_app = app;

    if (s_inst) {
        qFatal ("The Core instance of SpeechControl was being invoked again. This is a fatal and funny error.");
    }

    s_inst = this;

    // start application.
    m_trnsltr = new QTranslator (this);
    m_app->installTranslator (m_trnsltr);

    System::start (&p_argc, &p_argv);
    Session::init();

    // Create application's configuration directory.
    QDir configDir;
    configDir.mkdir (configurationPath().path() + "/contents");

    // build settings
    m_settings = new QSettings (QSettings::UserScope, "Synthetic Intellect Institute", "SpeechControl", this);

    hookUpSignals();
    loadTranslations (QLocale::system());

    // Set up indicator.
    qDebug() << "[Core::${constructor}] Show indicator on start? " << configuration ("Indicator/Show").toBool();

    if (configuration ("Indicator/Show").toBool())
        Indicator::show();
}

Core::Core() : QObject()
{
    qFatal ("This constructor shouldn't ever be called.");
}

Core::Core (const Core& p_other) : QObject (p_other.parent()), m_app (p_other.m_app),
    m_mw (p_other.m_mw), m_settings (p_other.m_settings), m_trnsltr (p_other.m_trnsltr)
{

}

void Core::hookUpSignals()
{
    connect (m_app, SIGNAL (aboutToQuit()), this, SLOT (stop()));
    connect (this, SIGNAL (started()), this, SLOT (invokeAutoStart()));
    connect (this, SIGNAL (started()), Services::Engine::instance(), SLOT (start()));
    connect (this, SIGNAL (started()), Plugins::Factory::instance(), SLOT (start()));
    connect (this, SIGNAL (stopped()), Services::Engine::instance(), SLOT (stop()));
    connect (this, SIGNAL (stopped()), Plugins::Factory::instance(), SLOT (stop()));

    DesktopControl::Service::instance();
    Dictation::Service::instance();
}

void Core::start()
{
    // Detect if a first-run wizard should be run.
    if (!QFile::exists (s_inst->m_settings->fileName())) {
        if (QMessageBox::question (mainWindow(), QMessageBox::tr ("First Run"),
                                   QMessageBox::tr ("This seems to be the first time you've run SpeechControl on this system. "
                                           "A wizard allowing you to start SpeechControl will appear."), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
            QuickStart* l_win = new QuickStart (mainWindow());
            l_win->exec();
        }
    }

    mainWindow()->open();
    emit instance()->started();
}

void Core::stop()
{
    if (Core::configuration ("MainWindow/RememberState").toBool()) {
        Core::setConfiguration ("MainWindow/Visible", mainWindow()->isVisible());
    }

    emit instance()->stopped();
}

Windows::Main* Core::mainWindow()
{
    if (instance()->m_mw == 0)
        instance()->m_mw = new Windows::Main;

    return instance()->m_mw;
}

QVariant Core::configuration (const QString& p_attrName, QVariant p_attrDefValue)
{
    instance()->m_settings->sync();
    return instance()->m_settings->value (p_attrName, p_attrDefValue);
}

void Core::setConfiguration (const QString& p_attrName, const QVariant& p_attrValue)
{
    instance()->m_settings->setValue (p_attrName, p_attrValue);
    instance()->m_settings->sync();
}

int Core::exec()
{
    return instance()->m_app->exec();
}

void Core::quit (const int& p_exitCode)
{
    instance()->m_app->exit (p_exitCode);
}

void Core::invokeAutoStart()
{
    const bool dsktpCntrlState = configuration ("DesktopControl/AutoStart").toBool();
    const bool dctnState = configuration ("Dictation/AutoStart").toBool();
    DesktopControl::Agent::instance()->setState ( (dsktpCntrlState) ? SpeechControl::AbstractAgent::Enabled  : SpeechControl::AbstractAgent::Disabled);
    Dictation::Agent::instance()->setState ( (dctnState) ? SpeechControl::AbstractAgent::Enabled  : SpeechControl::AbstractAgent::Disabled);
}

void Core::loadTranslations (const QLocale& p_locale)
{
    qDebug() << "Loading locale" << p_locale.name();
    instance()->m_trnsltr->load ("speechcontrol_" + p_locale.name());
}

QDir Core::configurationPath()
{
    return QDir (QDir::homePath().append ("/.config/speechcontrol"));
}

void Core::setAutoStart (const bool p_toggle)
{
    QFile* autoStartFile = new QFile (QDir::homePath().append ("/.config/autostart/SpeechControl.desktop"));

    if (p_toggle) {
        if (autoStartFile->open (QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream desktopFile (autoStartFile);
            desktopFile << "[Desktop Entry]" << endl
                        << "Name=Start SpeechControl on Launch" << endl
                        << "Icon=speechcontrol" << endl
                        << "Exec=speechcontrol-frontend" << endl
                        << "Terminal=false" << endl
                        << "Type=Application" << endl
                        << "X-GNOME-Autostart-enabled=true" << endl
                        << "X-GNOME-Autostart-Delay=30" << endl;
            autoStartFile->close();
            autoStartFile->setPermissions (autoStartFile->permissions() | QFile::ExeUser | QFile::ExeOwner | QFile::ExeGroup);
        }
    }
    else {
        if (autoStartFile->exists()) {
            autoStartFile->remove();
        }
    }
}

bool Core::doesAutoStart()
{
    QFile* autoStartFile = new QFile (QDir::homePath().append ("/.config/autostart/SpeechControl.desktop"));
    return autoStartFile->exists();
}

Core::~Core ()
{
    m_settings->sync();
}
#include "core.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
