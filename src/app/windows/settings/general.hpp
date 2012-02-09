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

#ifndef GENERAL_SETTINGS_HPP
#define GENERAL_SETTINGS_HPP

#include <QFrame>

namespace Ui {
    class GeneralSettingsPane;
}

namespace SpeechControl {
namespace Windows {

class GeneralSettingsPane : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(const QString Title READ title)
    Q_PROPERTY(const QString ID READ id)

public:
    explicit GeneralSettingsPane(QWidget *parent = 0);
    ~GeneralSettingsPane();
    const QString title() const;
    const QString id() const;

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_checkBoxEnableDictation_toggled(bool checked);
    void on_checkBoxEnableDesktopControl_toggled(bool checked);

private:
    Ui::GeneralSettingsPane *ui;
};

}}

#endif // GENERAL_HPP
