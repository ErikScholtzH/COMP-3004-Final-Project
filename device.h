#ifndef DEVICE_H
#define DEVICE_H

using namespace std;
#include <iostream>
#include <time.h>

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
private:
    float batteryLevel;
    string time;
    MainWindow* mw;
    int menu = 0;
    int lastMenu = 0;
    int subMenu = 0;

    bool isOn;

    int breathPacer;
    string challenger;
};

#endif // DEVICE_H
