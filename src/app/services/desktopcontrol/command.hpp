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
 *  along with SpeechControl .  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <QUuid>
#include <QList>
#include <QMap>
#include <QObject>
#include <QStringList>

namespace SpeechControl
{
namespace DesktopControl
{
class AbstractCommand;
class AbstractCategory;

typedef QList<AbstractCommand*> CommandList;
typedef QList<AbstractCategory*> CategoryList;

/**
 * @brief Abstract class defining commands for desktop control.
 *
 * The abstract command interface provides a common base for all possible
 * commands to be defined with desktop control. All commands are expected to
 * have a definable statement, something that the user can know to say to
 * expect a command to be invoked, a verification method that checks if the
 * input string fits closely to what is expected to be, and an invocation slot
 * that takes one argument optionally to solidify the action's potential.
 *
 * @see AbstractCategory
 **/
class AbstractCommand : public QObject
{
    Q_OBJECT
    friend class AbstractCategory;

public:
    virtual ~AbstractCommand();
    virtual QString id() = 0;
    virtual const QStringList statements() const;
    bool isValidStatement (const QString&) const;

public slots:
    virtual bool invoke (const QString& p_statement = QString::null) const = 0;

protected:
    explicit AbstractCommand (AbstractCategory* p_parentCategory , QStringList p_commands);
    QString obtainArgumentFromStatement (const QString p_statement) const;
    QString obtainCommandFromStatement (const QString p_statement) const;
    bool areStatementsEquivalent (const QString p_command, const QString p_statement) const;
    QStringList m_commands;     ///< The commands defined by this AbstractCommand.
};

/**
 * @brief A collection of commands in a defining manner.
 *
 * Categories are used here to organize specific commands into groups.
 * Commands are not confined to remain to a specific group, thus allowing
 * a sense of flexibility when assigning commands to groups. The use of
 * categories permits a global categories (really, a collection of all
 * implemented categories) for immediate searching.
 *
 * @see AbstractCommand
 * @see SpeechControl::DesktopControl::Agent
 */
class AbstractCategory : public QObject
{
    Q_OBJECT
    Q_PROPERTY (CommandList Commands READ commands)   ///< A property representing all of the immediate commands held by this category.
    friend class AbstractCommand;

public:
    virtual ~AbstractCategory();

    /**
     * @brief Obtains the title of this AbstractCategory.
     **/
    virtual const QString title() = 0;

    /**
     * @brief Obtains the unique ID of this AbstractCategory.
     **/
    virtual const QString id() const = 0;

    /**
     * @brief Attempts to match up a potential command.
     *
     * Queries all of the commands available by invoking commands() if
     * they can handle p_statement as a valid command.
     *
     * @param p_command The command to match up.
     * @return CommandList A list of commands.
     **/
    CommandList matchCommands (const QString& p_command);

    /**
     * @brief Returns a list of Command objects held by this AbstractCategory.
     * @return CommandList The list of commands.
     **/
    CommandList commands();

    static CommandList matchAllCommands (const QString& p_command);
    static AbstractCategory* global();
    static CategoryList categories();
    bool hasCommand (AbstractCommand* p_command);
    bool hasCommand (const QString& p_id);

protected:
    explicit AbstractCategory (AbstractCategory* p_parentCategory);
    AbstractCategory();

    void addCommand (AbstractCommand* p_command);
    void removeCommand (AbstractCommand* p_command);
    void removeCommand (const QString& p_id);

private:
    static QMap<QString, AbstractCategory*> s_ctgrs;    ///< A global listing of all categories.
    QMap<QString, AbstractCommand*> m_map;              ///< The commands held by the category.
};
}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
