#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    // Initialize timer counter
    currentTimerCount = -1;

    // Set Initial skin status
    onSkin = false;

    // Create database connection
    db = new DBManager();

    // Create a user Profile
    profile = db->getProfile(1);

    maxPowerLevel = profile->getPLvl();

    // Create lists of therapies
    initializeTherapies();

    // Create menu tree
    masterMenu = new Menu("MAIN MENU", {"PROGRAMS","FREQUENCY","HISTORY"}, nullptr);
    mainMenuOG = masterMenu;
    initializeMainMenu(masterMenu);

    // Initialize the main menu view
    activeQListWidget = ui->mainMenuListView;
    activeQListWidget->addItems(masterMenu->getMenuItems());
    activeQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(masterMenu->getName());

    // Account for device being "off" on sim start
    powerStatus = false;
    changePowerStatus();
    connect(ui->powerButton, &QPushButton::released, this, &MainWindow::powerChange);

    // charge button connections
    connect(ui->chargeAdminButton, &QPushButton::released, this, &MainWindow::rechargeBattery);

    // Battery level spin box connections
    connect(ui->batteryLevelAdminSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::changeBatteryLevel);

    // device interface button connections
    connect(ui->upButton, &QPushButton::pressed, this, &MainWindow::navigateUpMenu);
    connect(ui->downButton, &QPushButton::pressed, this, &MainWindow::navigateDownMenu);
    connect(ui->okButton, &QPushButton::pressed, this, &MainWindow::navigateSubMenu);
    connect(ui->menuButton, &QPushButton::pressed, this, &MainWindow::navigateToMainMenu);
    connect(ui->backButton, &QPushButton::pressed, this, &MainWindow::navigateBack);
    connect(ui->rightButton, &QPushButton::pressed, this, &MainWindow::changePowerLevelUp);
    connect(ui->leftButton, &QPushButton::pressed, this, &MainWindow::changePowerLevelDown);
    connect(ui->applyToSkinButton, &QPushButton::clicked, this, QOverload<bool>::of(&MainWindow::applyToSkin));

    // Disable Power Buttons
    ui->rightButton->blockSignals(true);
    ui->leftButton->blockSignals(true);


    // Apply to skin QComboBox connections
    connect(ui->applyToSkinAdminBox, QOverload<int>::of(&QComboBox::activated), this, QOverload<int>::of(&MainWindow::applyToSkin));

    // Change power level from admin
    connect(ui->powerLevelAdminSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::changePowerLevel);

    // Initialize battery levels
    ui->powerLevelAdminSpinBox->setValue(profile->getPLvl());
    ui->batteryLevelAdminSpinBox->setValue(profile->getBLvl());

    ui->programViewWidget->setVisible(false);
    ui->electrodeLabel->setVisible(false);

    //Load recordings from the database
    recordings = db->getRecordings();
    for (int x = 0; x < recordings.size(); x++) {
        allRecordings += recordings[x]->toString();
    }
}


MainWindow::~MainWindow() {

    db->addProfile(profile->getId(), profile->getBLvl(), profile->getPLvl());

    delete mainMenuOG;
    delete ui;

    for (int i = 0; i < programs.size(); i++) {
        delete programs[i];
    }

    for (int i = 0; i < frequencies.size(); i++) {
        delete frequencies[i];
    }

    for (int i = 0; i < recordings.size(); i++) {
        delete recordings[i];
    }

    delete db;
    delete profile;
}


void MainWindow::initializeMainMenu(Menu* m) {

    QStringList frequenciesList;
    QStringList programsList;

    for (Therapy* f : this->frequencies) {
        frequenciesList.append(f->getName());
    }
    for (Therapy* p : this->programs) {
        programsList.append(p->getName());
    }

    Menu* programs = new Menu("PROGRAMS", programsList, m);
    Menu* frequencies = new Menu("FREQUENCIES", frequenciesList, m);
    Menu* history = new Menu("HISTORY", {"VIEW","CLEAR"}, m);

    m->addChildMenu(programs);
    m->addChildMenu(frequencies);
    m->addChildMenu(history);

    for (Therapy* f : this->frequencies) {
        frequencies->addChildMenu(new Menu(f->getName(), {}, frequencies));
    }

    for (Therapy* p : this->programs) {
        programs->addChildMenu(new Menu(p->getName(), {}, programs));
    }

    Menu* viewHistory = new Menu("VIEW",{}, history);
    Menu* clearHistory = new Menu("CLEAR", {"YES","NO"}, history);
    history->addChildMenu(viewHistory);
    history->addChildMenu(clearHistory);
}


void MainWindow::initializeTherapies() {

    QList<Therapy*>* therapies = db->therapies();

    for (Therapy* f : *therapies) {
        programs.push_back(f);
    }

    QList<QString>* frequencyStrings = db->frequencies();

    for (QString f : *frequencyStrings) {
        frequencies.push_back(new Therapy(f, 0, f));
    }

    delete frequencyStrings;
    delete therapies;
}


void MainWindow::beginTherapy(Therapy* t) {

    ui->electrodeLabel->setVisible(true);
    currentTherapy = t;

    //Update frequency label
    ui->frequencyLabel->setText(t->getFreq());
    ui->programViewWidget->setVisible(true);

    //Timer
    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->treatmentView->setScene(scene);
    currentTimerCount = t->getTime();
    timeString = QString::number(currentTimerCount/60) + ":00";
    scene->addText(timeString);
    initializeTimer(t->getTimer());

    //Set up recording object for this therapy
    //Note: The name and start time of the recording is set properly here, the power level and duration will be saved at the end.
    Record* newR = new Record(t->getName(), QDateTime::currentDateTime(), 0, 0);
    recordings.append(newR);

    //Power Display
    ui->powerLevelLabel->setText("Power: " + QString::number(profile->getPLvl()));

    //Set Max Power Level back to currentPowerLevel
    maxPowerLevel = profile->getPLvl();

    //Power Buttons Enabled
    ui->rightButton->blockSignals(false);
    ui->leftButton->blockSignals(false);
}


void MainWindow::changePowerStatus() {

    activeQListWidget->setVisible(powerStatus);
    ui->menuLabel->setVisible(powerStatus);
    ui->statusBarQFrame->setVisible(powerStatus);
    ui->treatmentView->setVisible(powerStatus);
    ui->frequencyLabel->setVisible(powerStatus);
    ui->programViewWidget->setVisible(powerStatus);

    //Remove this if we want the menu to stay in the same position when the power is off
    if (powerStatus) {
        MainWindow::navigateToMainMenu();
        ui->applyToSkinButton->setChecked(false);
        applyToSkin(false);
    }

    ui->upButton->setEnabled(powerStatus);
    ui->downButton->setEnabled(powerStatus);
    ui->leftButton->setEnabled(powerStatus);
    ui->rightButton->setEnabled(powerStatus);
    ui->menuButton->setEnabled(powerStatus);
    ui->okButton->setEnabled(powerStatus);
    ui->backButton->setEnabled(powerStatus);
    ui->applyToSkinButton->setEnabled(powerStatus);
    ui->applyToSkinAdminBox->setEnabled(powerStatus);
}


void MainWindow::powerChange() {

    if (profile->getBLvl() > 0) {
        powerStatus  = !powerStatus;
        changePowerStatus();
    }

    if (currentTimerCount != -1) {
        //Save Record
        if (masterMenu->getParent()->getName() == "PROGRAMS") {
            recordings.last()->setDuration((currentTherapy->getTime())-currentTimerCount);
            recordings.last()->setPowerLevel(maxPowerLevel);
            db->addTherapyRecord(recordings.last()->getTreatment(),recordings.last()->getStartTime(),recordings.last()->getPowerLevel(),recordings.last()->getDuration());
        }
        else {
            recordings.last()->setDuration(currentTimerCount);
            recordings.last()->setPowerLevel(maxPowerLevel);
            db->addFrequencyRecord(recordings.last()->getTreatment(),recordings.last()->getStartTime(),recordings.last()->getPowerLevel(),recordings.last()->getDuration());
        }

        allRecordings += recordings.last()->toString();
        //Stop therapy
        currentTimerCount = -1;
        currentTherapy->getTimer()->stop();
        currentTherapy->getTimer()->disconnect();
        applyToSkin(false);
    }
}


void MainWindow::rechargeBattery() {

    int batteryLevel = 100.0;
    changeBatteryLevel(batteryLevel);
}


void MainWindow::navigateUpMenu() {

    int nextIndex = activeQListWidget->currentRow() - 1;

    if (nextIndex < 0) {
        nextIndex = activeQListWidget->count() - 1;
    }

    activeQListWidget->setCurrentRow(nextIndex);
}


void MainWindow::navigateDownMenu() {

    int nextIndex = activeQListWidget->currentRow() + 1;

    if (nextIndex > activeQListWidget->count() - 1) {
        nextIndex = 0;
    }

    activeQListWidget->setCurrentRow(nextIndex);
}


void MainWindow::navigateSubMenu() {

    int index = activeQListWidget->currentRow();
    if (index < 0) return;

    // Prevent crash if ok button is selected in view
    if (masterMenu->getName() == "VIEW") {
        return;
    }

    //Logic for when the menu is the delete menu.
    if (masterMenu->getName() == "CLEAR") {
        if (masterMenu->getMenuItems()[index] == "YES") {
            db->deleteRecords();
            allRecordings.clear();

            for (int x = 0; x < recordings.size(); x++) {
                delete recordings[x];
            }

            recordings.clear();
            navigateBack();
            return;
        }
        else {
            navigateBack();
            return;
        }
    }

    //If the menu is a parent and clicking on it should display more menus.
    if (masterMenu->get(index)->getMenuItems().length() > 0) {
        masterMenu = masterMenu->get(index);
        MainWindow::updateMenu(masterMenu->getName(), masterMenu->getMenuItems());


    }
    //If the menu is not a parent and clicking on it should start a therapy
    else if (masterMenu->get(index)->getMenuItems().length() == 0 && (masterMenu->getName() == "FREQUENCIES" || masterMenu->getName() == "PROGRAMS")) {
        if (masterMenu->getName() == "PROGRAMS") {
            //Update new menu info
            masterMenu = masterMenu->get(index);
            MainWindow::updateMenu(programs[index]->getName(), {});
            MainWindow::beginTherapy(programs[index]);
        }
        else if (masterMenu->getName() == "FREQUENCIES") {
            masterMenu = masterMenu->get(index);
            MainWindow::updateMenu(frequencies[index]->getName(), {});
            MainWindow::beginTherapy(frequencies[index]);
        }
    }
    //If the button pressed should display the device's recordings.
    else if (masterMenu->get(index)->getName() == "VIEW") {
        masterMenu = masterMenu->get(index);
        MainWindow::updateMenu("RECORDINGS", allRecordings);
    }
}


void MainWindow::initializeTimer(QTimer* t) {

    connect(t, &QTimer::timeout, this, &MainWindow::updateTimer);

    if (onSkin == true) {
        t->start(1000);
    }

}


void MainWindow::updateTimer() {

    drainBattery();

    if (currentTherapy->getTime() == 0) {
        currentTimerCount += 1;
    }
    else {
        currentTimerCount -= 1;
    }

    timeString = QString::number(currentTimerCount/60) + ((currentTimerCount%60 < 10) ? + ":0" + QString::number(currentTimerCount%60) : + ":" + QString::number(currentTimerCount%60));

    ui->treatmentView->scene()->clear();
    ui->treatmentView->scene()->addText(timeString);

    if (currentTimerCount == 0 && currentTherapy->getTime() != 0) {
        //Save record
        recordings.last()->setDuration(currentTherapy->getTime());
        recordings.last()->setPowerLevel(maxPowerLevel);

        db->addTherapyRecord(recordings.last()->getTreatment(),recordings.last()->getStartTime(),recordings.last()->getPowerLevel(),recordings.last()->getDuration());
        allRecordings += recordings.last()->toString();

        currentTimerCount = -1;
        currentTherapy->getTimer()->stop();
        currentTherapy->getTimer()->disconnect();
        currentTherapy = nullptr;
        applyToSkin(false);
        navigateBack();
    }
}


void MainWindow::updateMenu(const QString selectedMenuItem, const QStringList menuItems) {

    activeQListWidget->clear();
    activeQListWidget->addItems(menuItems);
    activeQListWidget->setCurrentRow(0);

    ui->menuLabel->setText(selectedMenuItem);
}


void MainWindow::navigateToMainMenu() {

    if (currentTimerCount != -1) {
        //Save recording
        if (masterMenu->getParent()->getName() == "PROGRAMS") {
            recordings.last()->setDuration((currentTherapy->getTime())-currentTimerCount);
            recordings.last()->setPowerLevel(maxPowerLevel);
            db->addTherapyRecord(recordings.last()->getTreatment(),recordings.last()->getStartTime(),recordings.last()->getPowerLevel(),recordings.last()->getDuration());
        }
        else {
            recordings.last()->setDuration(currentTimerCount);
            recordings.last()->setPowerLevel(maxPowerLevel);
            db->addFrequencyRecord(recordings.last()->getTreatment(),recordings.last()->getStartTime(),recordings.last()->getPowerLevel(),recordings.last()->getDuration());
        }

        allRecordings += recordings.last()->toString();

        //End therapy
        currentTimerCount = -1;
        currentTherapy->getTimer()->stop();
        currentTherapy->getTimer()->disconnect();
        currentTherapy = nullptr;
    }

    while (masterMenu->getName() != "MAIN MENU") {
        masterMenu = masterMenu->getParent();
    }

    updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
    ui->programViewWidget->setVisible(false);
    ui->electrodeLabel->setVisible(false);
}


void MainWindow::navigateBack() {

    ui->rightButton->blockSignals(true);
    ui->leftButton->blockSignals(true);

    if (currentTimerCount != -1) {
        //Save recording
        if (masterMenu->getParent()->getName() == "PROGRAMS") {
            recordings.last()->setDuration((currentTherapy->getTime())-currentTimerCount);
            recordings.last()->setPowerLevel(maxPowerLevel);
            db->addTherapyRecord(recordings.last()->getTreatment(),recordings.last()->getStartTime(),recordings.last()->getPowerLevel(),recordings.last()->getDuration());
        }
        else {
            recordings.last()->setDuration(currentTimerCount);
            recordings.last()->setPowerLevel(maxPowerLevel);
            db->addFrequencyRecord(recordings.last()->getTreatment(),recordings.last()->getStartTime(),recordings.last()->getPowerLevel(),recordings.last()->getDuration());
        }

        allRecordings += recordings.last()->toString();

        //Stop therapy
        currentTimerCount = -1;
        currentTherapy->getTimer()->stop();
        currentTherapy->getTimer()->disconnect();
        currentTherapy = nullptr;
    }

    if (masterMenu->getName() == "MAIN MENU") {
        activeQListWidget->setCurrentRow(0);
    }
    else {
        masterMenu = masterMenu->getParent();
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
    }

    ui->programViewWidget->setVisible(false);
    ui->electrodeLabel->setVisible(false);
}


void MainWindow::changePowerLevelUp() {

    int currentPowerLevel = profile->getPLvl();
    currentPowerLevel = (currentPowerLevel + 1 < 100) ? currentPowerLevel + 1 : 100;

    maxPowerLevel = (currentPowerLevel > maxPowerLevel) ? currentPowerLevel : maxPowerLevel;

    ui->powerLevelLabel->setText("Power: " + QString::number(currentPowerLevel));
    ui->powerLevelAdminSpinBox->setValue(currentPowerLevel);

    profile->setPLvl(currentPowerLevel);
}


void MainWindow::changePowerLevelDown() {

    int currentPowerLevel = profile->getPLvl();
    currentPowerLevel = (currentPowerLevel - 1 > 0) ? currentPowerLevel - 1 : 0;

    maxPowerLevel = (currentPowerLevel > maxPowerLevel) ? currentPowerLevel : maxPowerLevel;

    ui->powerLevelLabel->setText("Power: " + QString::number(currentPowerLevel));
    ui->powerLevelAdminSpinBox->setValue(currentPowerLevel);

    profile->setPLvl(currentPowerLevel);
}


void MainWindow::changeBatteryLevel(double newLevel) {

    if (newLevel >= 0.0 && newLevel <= 100.0) {
        if (newLevel == 0.0 && powerStatus == true) {
            powerChange();
            profile->setBLvl(0);
        }else{
            profile->setBLvl(newLevel);
        }

        ui->batteryLevelAdminSpinBox->setValue(newLevel);
        int newLevelInt = int(newLevel);
        ui->batteryLevelBar->setValue(newLevelInt);

        QString highBatteryHealth = "QProgressBar { selection-background-color: rgb(78, 154, 6); background-color: rgb(0, 0, 0); }";
        QString mediumBatteryHealth = "QProgressBar { selection-background-color: rgb(196, 160, 0); background-color: rgb(0, 0, 0); }";
        QString lowBatteryHealth = "QProgressBar { selection-background-color: rgb(164, 0, 0); background-color: rgb(0, 0, 0); }";

        if (newLevelInt >= 50) {
            ui->batteryLevelBar->setStyleSheet(highBatteryHealth);
        }
        else if (newLevelInt >= 20) {
            ui->batteryLevelBar->setStyleSheet(mediumBatteryHealth);
        }
        else {
            ui->batteryLevelBar->setStyleSheet(lowBatteryHealth);
        }
    }
}


void MainWindow::applyToSkin(bool checked) {

    ui->electrodeLabel->setPixmap(QPixmap(checked ? ":/icons/electrodeOn.svg" : ":/icons/electrodeOff.svg"));
    ui->applyToSkinAdminBox->setCurrentIndex(checked ? 1 : 0);
    onSkin = checked;

    if (currentTimerCount != -1) {
        if (!onSkin) {
            currentTherapy->getTimer()->stop();
        }
        else {
            currentTherapy->getTimer()->start(1000);
        }
    }
}


void MainWindow::applyToSkin(int indexVal) {

    ui->applyToSkinButton->setChecked(indexVal == 1);
    applyToSkin(indexVal == 1);
    //onSkin = !onSkin;
}


void MainWindow::drainBattery() {

    //1000 constant because 15 minutes is the longest therapy and we feel as it should last at least 15 minutes at full power
    double batteryLevel = (profile->getPLvl() == 0) ? profile->getBLvl() - 0.05 : qMax(profile->getBLvl() - profile->getPLvl()/POWER_DRAIN, 0.0);

    changeBatteryLevel(batteryLevel);
}


void MainWindow::changePowerLevel(int powerLvl) {

    int currentPowerLevel = powerLvl;
    maxPowerLevel = (currentPowerLevel > maxPowerLevel) ? currentPowerLevel : maxPowerLevel;
    ui->powerLevelLabel->setText("Power: " + QString::number(currentPowerLevel));
    profile->setPLvl(currentPowerLevel);
}
