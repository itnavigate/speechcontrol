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

#ifndef CONTENTSETTINGSPANE_HPP
#define CONTENTSETTINGSPANE_HPP

#include <QFrame>

namespace Ui {
class ContentSettingsPane;
}

namespace SpeechControl {
namespace Windows {

class ContentSettingsPane : public QFrame {
    Q_OBJECT
    Q_PROPERTY ( const QString Title READ title )
    Q_PROPERTY ( const QString ID READ id )

public:
    explicit ContentSettingsPane ( QWidget *parent = 0 );
    ~ContentSettingsPane();
    const QString title() const;
    const QString id() const;
    virtual void show();

protected:
    void changeEvent ( QEvent *e );
    void updateList();

private slots:
    void on_btnDelete_clicked();
    void on_btnAdd_clicked();
    void on_btnInfo_clicked();

private:
    Ui::ContentSettingsPane *ui;
};

}
}

#endif // CONTENTSETTINGSPANE_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;