#ifndef DEVICE_H
#define DEVICE_H

#define MAX_HISTORY 64

using namespace std;
#include <iostream>
#include <fstream>
#include <time.h>
#include <unistd.h>
#include <QCoreApplication>
#include "sessionhistory.h"
#include "QScrollArea"
#include "QVBoxLayout"
#include "QLabel"
#include "QDateTime"
#include "QPushButton"
#include "QComboBox"
class MainWindow;

class Device
{
public:
    Device(MainWindow *_mw);
    void SetBatteryLevel();
    int GetBatteryLevel();
    void DecreaseBatteryLevel();
    void IncreaseBatteryLevel();
    void UpButton();
    void DownButton();
    void IncreaseButton();
    void DecreaseButton();
    void SelectButton();
    void IncreaseDifficulty();
    void DecreaseDifficulty();
    void IncreaseLevel();
    void DecreaseLevel();
    void TurnOff();
    void TurnOn();
    bool GetIsOn();
    void BackButton();
    void MenuButton();
    void SetupVariables();
    void FindHistory();
    void ShowSummary(int);
    void SetupButtons();
    void updateLEDS(float);
    void delay();

private:
    float batteryLevel;
    string time;
    MainWindow* mw;
    int menu = 0;
    int lastMenu = 0;
    int subMenu = 0;


    bool inSummary;
    bool isOn;

    int breathPacer;
    string challenger;
    bool inSession;


    int historySize;
    SessionHistory *history[MAX_HISTORY];
};

#endif // DEVICE_H
