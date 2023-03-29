#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define POWER_DRAIN 1000.0

#include <QMainWindow>
#include <QSpinBox>
#include <QListWidget>
#include <QGraphicsView>
#include <QTime>
#include <QStatusBar>
#include <QVector>
#include <QtGlobal>

#include "menu.h"
#include "therapy.h"
#include "dbmanager.h"
#include "profile.h"
#include "record.h"


/* Class Purpose: The main window of the application, acts as the control of the application
 *
 * Data Members:
 * -Menu* masterMenu: the current node of the menu tree that the user is on
 * -Menu* mainMenuOG: the original main menu node, need this in order to delete without memory leaks
 *
 * -Ui::MainWindow *ui: A ui pointer for the UI in mainwindow
 * -QListWidget *activeQListWidget: A widget for displaying the menus
 *
 * -QVector<Therapy*> programs: a vector holding the therapy objects that make up a program
 * -QVector<Therapy*> frequencies: a vector holding the therapy objects that make up a frequency
 * -QVector<Record*> recordings: a vector holding the record objects
 *
 * -QStringList allRecordings: A list of strings where each string is a string representation of a record object
 *
 * -Therapy* currentTherapy: A pointer for the current running therapy
 * -DBManager* db: A database variable
 * -Profile* profile: A profile variable
 *
 * -bool powerStatus: whether the denas is turned on or off
 * -bool onSkin: whether the devices is on someones skin or off
 *
 * -int currentTimerCount: the current count of whatever timer is running
 * -int maxPowerLevel: the max power level that a therapy has hit
 *
 * -QString timeString: a string representation of how long is left in a therapy
 *
 *
 * Class Functions:
 * void updateMenu(const QString, const QStringList)
 * void initializeMainMenu(Menu*): Dynamically allocates all the menu objects
 * void initializeTherapies(): Dynamically allocates all the therapies
 * void initializeTimer(QTimer*): Initalizes the timer
 * void beginTherapy(Therapy*): Displays the therapy page, and initalizes data to run the therapy if they click the apply to skin button
 * void changePowerStatus(): Disables UI if the device is getting turned off, enables the UI if the device is getting turned on
 * void drainBattery(): Contains an equation of how much to drain the battery by then calls another function to change it
 *
 * void updateTimer(): Updates the timer displayed on the screen, saves to the database and navigates back if the therapy finishes
 * void powerChange(): Changes the state from on to off, saves data to the database if it is turning off
 * void rechargeBattery(): Changes the currentPowerLevel to 100
 * void navigateDownMenu(): Navigates down on a menu
 * void navigateUpMenu(): Navigates up on a menu
 * void navigateSubMenu(): Connected to the OK button, goes into a submenu
 * void navigateToMainMenu(): Navigates to main menu
 * void navigateBack(): Navigate the previous menu
 * void changePowerLevelUp(): Slot to change the power level up
 * void changePowerLevelDown(): Slot to change the power level down
 * void changeBatteryLevel(double): Slot to change the battery level from the admin panel
 * void applyToSkin(bool): Starts/stops the therapy timer if they are on a therapy
 * void applyToSkin(int): Starts/stops the therapy from the admin panel
 * void changePowerLevel(int): sets the max power level if the current power is higher than max and sets the ui with the power level
 */

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Menu* masterMenu;
    Menu* mainMenuOG;

    Ui::MainWindow *ui;
    QListWidget *activeQListWidget;

    QVector<Therapy*> programs;
    QVector<Therapy*> frequencies;
    QVector<Record*> recordings;

    QStringList allRecordings;

    Therapy* currentTherapy;
    DBManager* db;
    Profile* profile;

    bool powerStatus;
    bool onSkin;

    int currentTimerCount;
    int maxPowerLevel;

    QString timeString;

    void updateMenu(const QString, const QStringList);
    void initializeMainMenu(Menu*);
    void initializeTherapies();
    void initializeTimer(QTimer*);
    void beginTherapy(Therapy*);
    void changePowerStatus();
    void drainBattery();


private slots:
    void updateTimer();
    void powerChange();
    void rechargeBattery();
    void navigateDownMenu();
    void navigateUpMenu();
    void navigateSubMenu();
    void navigateToMainMenu();
    void navigateBack();
    void changePowerLevelUp();
    void changePowerLevelDown();
    void changeBatteryLevel(double);
    void applyToSkin(bool);
    void applyToSkin(int);
    void changePowerLevel(int);

};

#endif // MAINWINDOW_H
