#ifndef DEVICE_H
#define DEVICE_H

#define MAX_HISTORY 64
#define MAX_LENGTH 512

using namespace std;
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <unistd.h>
#include <QCoreApplication>
#include "SessionManager.h"
#include "heartmeasuringelectrodes.h"
#include "QScrollArea"
#include "QVBoxLayout"
#include "QLabel"
#include "QDateTime"
#include "QPushButton"
#include "QComboBox"
#include "QProgressBar"
#include "qcustomplot.h"

class MainWindow;

class Device
{
public:
    Device(MainWindow *_mw);
    void SetBatteryLevel();
    int GetBatteryLevel();
    void DecreaseBatteryLevel();
    void IncreaseBatteryLevel();
    void fullCharge();
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
    void updateBreathPacerBar();
    void delay(float val);
    void runSession();
    int* stringToArray(char* str, int size);
    void resetData();
    void SetupSnake();
    void PlaySnake();
    void MoveFood();
    void AddToBody();

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
    int breathLoc;
    bool breathIn;

    string challenger;
    bool inSession;


    int historySize;
    SessionHistory *history[MAX_HISTORY];

    SessionManager* sessionManager;

    HeartMeasuringElectrodes* measuringDevice;

    QVector<double> x, y;

    int snakePosx;
    int snakePosy;
    int foodPosx;
    int foodPosy;

    int snakeScore;
    bool playingSnake;
    bool snakeAlive;
    int dir; //0 - right, 1 - left, 2 - up, 3 - down
    QWidget *snakeBody[MAX_LENGTH];
};

#endif // DEVICE_H
