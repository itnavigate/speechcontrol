/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
 *
 *  SpeechControl is free software; you can redistribute it and/or
 *  modify it -under the terms of the GNU Library General Public
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

#include <QMenu>
#include <QDebug>
#include <QImage>
#include <QDateTime>
#include <QApplication>
#include <QSystemTrayIcon>

#include "core.hpp"
#include "indicator.hpp"
#include "indicatorprivate.hpp"
#include "ui/settings-dialog.hpp"
#include "ui/about-dialog.hpp"
#include "ui/training-dialog.hpp"
#include "ui/session-manager.hpp"
#include "ui/content-manager.hpp"
#include "ui/adapt-wizard.hpp"
#include "services/dictation/service.hpp"
#include "services/desktopcontrol/service.hpp"
#include "sessions/session.hpp"

using namespace SpeechControl;

Indicator* Indicator::s_inst = 0;

Indicator::Message::Message (const QString& p_keyName) : m_key (p_keyName)
{

}

QString Indicator::Message::description() const
{
    if (Indicator::Message::exists (m_key)) {
        QVariantMap map = Indicator::Message::objectData (m_key);
        return map.value ("description").toString();
    }

    return QString::null;
}

bool Indicator::Message::enabled() const
{
    if (Indicator::Message::exists (m_key)) {
        QVariantMap map = Indicator::Message::objectData (m_key);
        return map.value ("enabled").toBool();
    }

    return true;
}

QString Indicator::Message::key() const
{
    return m_key;
}

void Indicator::Message::setEnabled (const bool& p_isEnabled)
{
    if (Indicator::Message::exists (m_key)) {
        QVariantMap map = Indicator::Message::objectData (m_key);
        map.insert ("enabled", p_isEnabled);
        Core::setConfiguration ("Notifications/" + m_key, map);
    }
}

bool Indicator::Message::exists (const QString& p_keyName)
{
    QVariant msgObj = Core::configuration ("Notifications/" + p_keyName);
    qDebug() << "[Indicator::Message::exists()] Exists 'Notifications/" << p_keyName.toStdString().c_str() << "' ?" << msgObj.isValid();
    return (msgObj.isValid());
}

Indicator::Message* Indicator::Message::create (const QString& p_keyName, const QString& p_keyDescription, const bool& p_isEnabled)
{
    QVariantMap map;
    map.insert ("description", p_keyDescription);
    map.insert ("enabled", p_isEnabled);

    Core::setConfiguration ("Notifications/" + p_keyName, map);
    return new Message (p_keyName);
}

QVariantMap Indicator::Message::objectData (const QString& p_keyName)
{
    return Core::configuration ("Notifications/" + p_keyName).toMap();
}

/// @todo Check for a configuration value to determine whether or not the indicator should be shown on initialization.
Indicator::Indicator () : QObject (Core::instance()),
    d_ptr (new IndicatorPrivate)
{
    Q_D (Indicator);
    s_inst = this;
    d->buildMenu();
}

QIcon Indicator::icon()
{
    const QString state = Core::configuration ("Indicator/Icon").toString();
    qDebug() << "[Indicator::icon] " << state;

    if (state == "White")
        return QIcon (":/indicator/white");
    else if (state == "Black")
        return QIcon (":/indicator/black");
    else if (state == "Default")
        return QIcon (":/logo/sc");

    return QApplication::windowIcon();
}

/// @todo Add an enumeration that allows the callee to specify the kind of message icon they'd  want to appear.
void Indicator::presentMessage (const QString& p_title, const QString& p_message, const int& p_timeout, const Indicator::Message* p_messageIndicator)
{
    if (!Indicator::Message::exists (p_messageIndicator->key()))
        Indicator::Message::create (p_messageIndicator->key(), p_message, true);

    if (p_messageIndicator->enabled())
        instance()->d_func()->icon->showMessage (p_title, p_message, QSystemTrayIcon::Information, p_timeout);
}

void Indicator::addActionForPlugins (QAction* p_action)
{
    instance()->d_func()->menuPlugins->insertAction (0, p_action);
}

void Indicator::removeActionForPlugins (QAction* p_action)
{
    instance()->d_func()->menuPlugins->removeAction (p_action);
}

void Indicator::on_actionDesktopControlOptions_triggered ()
{
    Windows::Settings::displayPane ("dsktpcntrl");
}

void Indicator::on_actionDictationOptions_triggered ()
{
    Windows::Settings::displayPane ("dctn");
}

void Indicator::on_actionDictationToggle_toggled (const bool& p_checked)
{
    p_checked ? Dictation::Service::instance()->start() : Dictation::Service::instance()->stop();
}

void Indicator::on_actionDesktopControlToggle_toggled (const bool& p_checked)
{
    p_checked ? DesktopControl::Service::instance()->start() : DesktopControl::Service::instance()->stop();
}

void Indicator::on_actionOptions_triggered()
{
    Windows::Settings::displayPane();
}

void Indicator::on_actionAboutSpeechControl_triggered()
{
    Windows::AboutDialog dialog;
    dialog.exec();
}

void Indicator::on_actionAdaptModels_triggered()
{
    Windows::Wizards::AdaptWizard adaptWizard;
    adaptWizard.exec();
}

void Indicator::on_actionStartTraining_triggered()
{
    Session* session = 0;

    if (!Session::allSessions().isEmpty()) {
        session = Windows::Managers::SessionManager::pickSession();
    }
    else {
        Content* content = 0;
        content = Windows::Managers::ContentManager::pickContent();

        if (content) {
            session = Session::create (content);
        }
        else {
            return;
        }
    }

    if (session) {
        Windows::TrainingDialog::startTraining (session);
    }
}

Indicator::~Indicator()
{

}

IndicatorPrivate::IndicatorPrivate() : icon (new QSystemTrayIcon (QApplication::windowIcon())),
    actionDesktopControlOptions (0), actionDesktopControlToggle (0),
    actionDictationToggle (0), actionDictationOptions (0),
    actionPluginOptions (0), actionAboutSpeechControl (0),
    actionAboutQt (0), actionHelpManual (0)
{
    icon->setIcon (Indicator::icon().pixmap (48, 48));
    icon->show();
}

void IndicatorPrivate::buildActions()
{
    actionDesktopControlOptions = new QAction (QIcon::fromTheme ("configure"), "&Options", Indicator::instance());
    actionDesktopControlToggle = new QAction ("&Active", Indicator::instance());
    actionDictationOptions = new QAction (QIcon::fromTheme ("configure"), "&Options", Indicator::instance());
    actionDictationToggle = new QAction ("&Active", Indicator::instance());

    Indicator::instance()->connect (actionDesktopControlToggle, SIGNAL (toggled (bool)), SLOT (on_actionDesktopControlToggle_toggled (bool)));
    Indicator::instance()->connect (actionDictationToggle, SIGNAL (toggled (bool)), SLOT (on_actionDictationToggle_toggled (bool)));
    Indicator::instance()->connect (actionDesktopControlOptions, SIGNAL (triggered (bool)), SLOT (on_actionDesktopControlOptions_triggered()));
    Indicator::instance()->connect (actionDictationOptions, SIGNAL (triggered (bool)), SLOT (on_actionDictationOptions_triggered ()));

    actionDesktopControlToggle->setCheckable (true);
    actionDictationToggle->setCheckable (true);
    actionDesktopControlToggle->setChecked (DesktopControl::Service::instance()->isActive());
    actionDictationToggle->setChecked (Dictation::Service::instance()->isActive());
}

void IndicatorPrivate::buildMenu()
{
    buildActions();
    menuBase = new QMenu;

    menuDesktopControl = menuBase->addMenu (QIcon::fromTheme ("audio-headset"), "Desktop Control");
    menuDesktopControl->addActions (QList<QAction*>()
                                    << actionDesktopControlToggle
                                    << actionDesktopControlOptions
                                   );

    menuDictation      = menuBase->addMenu (QIcon::fromTheme ("audio-input-microphone"), "Dictation");
    menuDictation->addActions (QList<QAction*>()
                               << actionDictationToggle
                               << actionDictationOptions
                              );

    menuPlugins        = menuBase->addMenu (QIcon::fromTheme ("configure"), "Plug-ins");
    menuPlugins->addSeparator();
    menuPlugins->addAction (actionPluginOptions);

    menuHelp           = menuBase->addMenu (QIcon::fromTheme ("help"), "Help");
    menuTraining        = menuBase->addMenu ("&Training");
    menuTraining->addAction ("&Start Training...", Indicator::instance(), SLOT (on_actionStartTraining_triggered()));
    menuTraining->addAction ("&Adapt Models", Indicator::instance(), SLOT (on_actionAdaptModels_triggered()));


    actionAboutQt = menuHelp->addAction (QIcon::fromTheme ("qt"), "About &Qt", QApplication::instance(), SLOT (aboutQt()));
    actionAboutSpeechControl = menuHelp->addAction (QApplication::windowIcon(), "&About SpeechControl", Indicator::instance(), SLOT (on_actionAboutSpeechControl_triggered()));

    menuBase->addMenu (menuDesktopControl);
    menuBase->addMenu (menuDictation);
    menuBase->addMenu (menuPlugins);
    menuBase->addSeparator();
    menuBase->addAction (QIcon::fromTheme ("configure"), "&Options", Indicator::instance() , SLOT (on_actionOptions_triggered()));
    menuBase->addMenu (menuHelp);
    menuBase->addSeparator();
    menuBase->addAction (QIcon::fromTheme ("application-exit"), "Quit", Core::instance(), SLOT (quit()));

    icon->setContextMenu (menuBase);
}

IndicatorPrivate::~IndicatorPrivate()
{
    icon->hide();
}


#include "indicator.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;

