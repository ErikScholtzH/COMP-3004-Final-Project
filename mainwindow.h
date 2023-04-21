#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "device.h"
#include "iostream"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void SetBatteryColor(string color);
    void SetBatteryLevel(int batteryLevel);
    void SetButtonColor(string color, string object);
    void StartSim();
    void GetUI();
    void SetupButtons();

private slots:
    void on_UpButton_released();

    void on_DownButton_released();

    void on_SelectButton_released();

    void on_LeftButton_released();

    void on_RightButton_released();

    void on_PowerButton_released();

    void on_BackButton_released();

    void on_MenuButton_released();

    void on_pushButton_released();

private:
    Ui::MainWindow *ui;
    Device *device;
    bool setup;
};
#endif // MAINWINDOW_H
