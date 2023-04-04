#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    device = new Device(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete device;
}

void MainWindow::SetBatteryColor(string color){
    setStyleSheet("QProgressBar#BatteryVisual::chunk{background-color: " + QString::fromStdString(color) + "}");
}

void MainWindow::SetBatteryLevel(int batteryLevel){
    QProgressBar *qpb = this->findChild<QProgressBar*>("BatteryVisual");
    if(qpb) qpb->setValue(batteryLevel);
    else cout << "Could not find BatteryVisual" << endl;

    QLabel *ql = this->findChild<QLabel*>("batteryPercent");
    if(ql) ql->setText(QString::number(batteryLevel) + QString::fromStdString("%"));
    else cout << "Could not find batteryPercent" << endl;
}

void MainWindow::SetButtonColor(string color, string object){
    QWidget *qw = this->findChild<QWidget*>(QString::fromStdString(object));
    if(qw) qw->setStyleSheet("background-color: " + QString::fromStdString(color));
    else cout << "Could not find " << object << endl;
}


void MainWindow::on_UpButton_released()
{
    device->UpButton();
}

void MainWindow::on_DownButton_released()
{
    device->DownButton();
}

void MainWindow::on_SelectButton_released()
{
    device->SelectButton();
}

void MainWindow::on_LeftButton_released()
{
    device->DecreaseButton();
}

void MainWindow::on_RightButton_released()
{
    device->IncreaseButton();
}

void MainWindow::on_PowerButton_released()
{
    if(device->GetIsOn()) device->TurnOff();
    else device->TurnOn();
}

void MainWindow::on_BackButton_released()
{
    device->BackButton();
}

void MainWindow::on_MenuButton_released()
{
    device->MenuButton();
}
