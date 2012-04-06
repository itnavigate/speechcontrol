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


#ifndef MAIN_HPP
#define MAIN_HPP

#include <QThread>
#include <QVariant>
#include <QVariantMap>
#include <QMainWindow>

#include <sessions/accuracymeter.hpp>

class QProgressBar;
class QListWidgetItem;

namespace Ui
{
class MainWindow;
}

namespace SpeechControl
{

class AccuracyUpdaterThread : public QThread
{
    Q_OBJECT
    AccuracyMeter* m_meter;

signals:
    void foundError();
    void foundSuccess();
    void foundNoData();

public:
    explicit AccuracyUpdaterThread (QObject* p_parent = 0) ;
    virtual ~AccuracyUpdaterThread();
    QVariantMap data();
    AccuracyMeter::Status status();

private:
    virtual void run();
    Session* session() const;

private slots:
    void assessmentCompleted();
};

class Indicator;
namespace Plugins
{

class AbstractPlugin;
}

class Core;
/**
 * @brief A namespace containing all of the classes dedicated to the user interface.
 **/
namespace Windows
{

class DictationSettingsPane;
class DesktopControlSettingsPane;

/**
 * @brief Represents the Main window of SpeechControl.
 *
 * Being the first thing (and perhaps the last thing) that users see when
 * they start SpeechControl,
 **/
class Main : public QMainWindow
{
    Q_OBJECT
    friend class SpeechControl::Core;
    friend class SpeechControl::Indicator;
    friend class Plugins::AbstractPlugin;
    friend class SpeechControl::AccuracyUpdaterThread;

public:
    /**
     * @brief Destructor.
     **/
    virtual ~Main();

    /**
     * @brief Updates the window's data.
     **/
    void updateUi();

    /**
     * @brief Shows a brief message in the status bar of the main window.
     *
     * @param p_message The message to display.
     * @param p_timeout How long it should be displayed without disturbance (in milliseconds) .Defaults to 8000.
     **/
    void setStatusMessage (const QString& p_message, const int p_timeout = 8000);

private:
    /**
     * @brief Constructor.
     **/
    explicit Main();

public slots:
    /**
     * @brief Shows the window.
     **/
    virtual void open();

    /**
     * @brief Shows progress in the task bar.
     *
     * @note At 1.0, the progress bar is hidden.
     * @param p_value The value of the progress on a scale of 0.0 to 1.0
     **/
    void setProgress (const double p_value);
    void on_acrcyThrd_foundNoData();
private slots:
    // desktop control
    void on_actionDesktopControlOptions_triggered();
    void on_actionDesktopControlActive_triggered (bool p_checked);
    void desktopControlStateChanged();

    // dictation
    void on_actionDictationOptions_triggered();
    void on_actionDictationActive_triggered (const bool p_checked);
    void dictationStateChanged();

    // training
    void on_actionStartTraining_triggered();
    void on_actionAdaptModels_triggered();
    void on_actionTrainingOptions_triggered();
    void on_actionCreateSession_triggered();

    // wizards
    void on_actionWizardMicrophone_triggered();
    void on_actionWizardContent_triggered();
    void on_actionWizardVoxForge_triggered();
    void on_actionWizardQuickStart_triggered();
    void on_actionWizardAdaption_triggered();

    // session pane
    void on_btnSessionTrain_clicked();
    void on_btnSessionAdapt_clicked();
    void on_btnSessionInfo_clicked();
    void on_listWidgetSessions_itemSelectionChanged();

    // services pane
    void on_listWidgetService_itemClicked (QListWidgetItem* p_item);
    void on_btnAllServices_clicked();

    // recognition pane
    void on_acrcyThrd_foundError();
    void on_acrcyThrd_foundSuccess();
    void on_acrcyThrd_finished();
    void doAccuracyCheck();

    // misc.
    void on_actionPluginOptions_triggered();
    void on_actionHelp_triggered();
    void on_actionOptions_triggered();
    void on_actionAboutQt_triggered();
    void on_actionReportBug_triggered();
    void on_actionAboutSpeechControl_triggered();

private:
    virtual void closeEvent (QCloseEvent* p_closeEvent);
    void updateSessionListing();
    void updateServiceListing();
    Ui::MainWindow* m_ui;
    QProgressBar* m_prgStatusbar;
    AccuracyUpdaterThread* m_acrcyThrd;
};
}
}

#endif // MAIN_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
