/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 Adrian Borucki <gentoolx@gmail.com>
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

#include <acousticmodel.hpp>
#include <languagemodel.hpp>
#include "core.hpp"
#include "sphinx.hpp"
#include "command.hpp"

using SpeechControl::Core;
using SpeechControl::AcousticModel;
using SpeechControl::LanguageModel;
using namespace SpeechControl::DesktopControl;

Sphinx::Sphinx (QObject* p_parent) : AbstractSphinx (p_parent)
{
    init();
}

Sphinx::Sphinx (const QString& p_description, QObject* p_parent) : AbstractSphinx (p_description, p_parent)
{
    init();
}

Sphinx::Sphinx (QGst::PipelinePtr p_pipeline, QObject* p_parent) : AbstractSphinx (p_pipeline, p_parent)
{
    init();
}

void Sphinx::init()
{
    QString aModelPath;
    QString lModelPath;

    QVariantMap models = Core::configuration ("DesktopControl/Models").toMap();

    if (!models.empty()) {
        aModelPath = models.value ("Acoustic").toString();
        lModelPath = models.value ("Language").toString();
    }
    else {
        aModelPath = Core::configuration ("Models/Acoustic").toString();
        lModelPath = Core::configuration ("Models/Language").toString();
    }

    setAcousticModel (aModelPath);
    setLanguageModel (lModelPath);
}

void Sphinx::applicationMessage (const QGst::MessagePtr& p_message)
{
    QString l_msgType    = p_message->internalStructure()->name();
    QString l_hypothesis = p_message->internalStructure()->value ("hyp").toString();
    QString l_uttid      = p_message->internalStructure()->value ("uttid").toString();

    if (l_msgType == "result") {
        qDebug() << "[DesktopControl::Sphinx::applicationMessage()] Obtained hypothesis" << l_hypothesis << "from user.";
        emit finished (l_hypothesis);
    }
}

#include "services/desktopcontrol/sphinx.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
