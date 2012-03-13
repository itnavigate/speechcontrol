/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *            (C) 2012 Jacky Alcine <jacky.alcine@thesii.org>
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

// local includes
#include "commands.hpp"

MPRIS_NAMESPACE_BEGIN

MprisCategory* MprisCategory::s_inst = 0;

MprisCategory::MprisCategory () : AbstractCategory (global())
{
}

const QString MprisCategory::id() const
{
    return "mpris";
}

const QString MprisCategory::title()
{
    return "MPRIS";
}

QString PlayStateCommand::id()
{
    return "mpris-playstate";
}

PlayStateCommand::PlayStateCommand () : AbstractCommand (MprisCategory::instance() ,
            QStringList() << tr ("Play music")
            << tr ("Pause music")
            << tr ("Stop playing music")
            << tr ("Next track")
            << tr ("Previous track"))
{
}

/// @todo Detect the arguments.
bool PlayStateCommand::invoke (const QString& p_statement) const
{
    if (!isValidStatement (p_statement)) {
        return false;
    }

    const QString l_tokenArgument = AbstractCommand::santizeStatement (p_statement);

    return true;
}

LibraryCommand::LibraryCommand() : AbstractCommand (MprisCategory::instance(),
            QStringList() << tr ("Play track")
            << tr ("Play album")
            << tr ("Play all by artist")
            << tr ("Play genre"))
{

}

QString LibraryCommand::id()
{
    return "mpris-library";
}

bool LibraryCommand::invoke (const QString& p_statement) const
{
    if (!isValidStatement (p_statement)) {
        return false;
    }

    const QString l_tokenArgument = AbstractCommand::santizeStatement (p_statement);

    return true;
}

#include "commands.moc"

MPRIS_NAMESPACE_END

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
