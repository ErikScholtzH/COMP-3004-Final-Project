#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetupButtons();
    srand(time(NULL));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete device;
}

//setup ui buttons
void MainWindow::SetupButtons(){
    QPushButton *button = findChild<QPushButton*>(QString::fromStdString("PowerButton"));
    button->setIcon(QIcon(":/new/prefix1/Power.png"));
    button->setIconSize(QSize(50, 50));

    int rot;
    vector<string> names = {"Up", "Right", "Down", "Left"};
    for (auto name : names) {
        button = findChild<QPushButton*>(QString::fromStdString(name+"Button"));
        button->setIcon(QIcon(QString::fromStdString(":/new/prefix1/arrow" + name + ".png")));
        button->setIconSize(QSize(25, 25));
    }

    button = findChild<QPushButton*>(QString::fromStdString("BackButton"));
    button->setIcon(QIcon(":/new/prefix1/undo.png"));
    button->setIconSize(QSize(25, 25));

    button = findChild<QPushButton*>(QString::fromStdString("MenuButton"));
    button->setIcon(QIcon(":/new/prefix1/home.png"));
    button->setIconSize(QSize(25, 25));

}

//starts the simulation
void MainWindow::StartSim(){
    device = new Device(this);
}

//sets battery ui color to color
void MainWindow::SetBatteryColor(string color){
    setStyleSheet("QProgressBar#BatteryVisual::chunk{background-color: " + QString::fromStdString(color) + "}");
}

//set ui battery level to batterylevel
void MainWindow::SetBatteryLevel(int batteryLevel){
    QProgressBar *qpb = this->findChild<QProgressBar*>("BatteryVisual");
    if(qpb) qpb->setValue(batteryLevel);
    else cout << "Could not find BatteryVisual" << endl;

    QLabel *ql = this->findChild<QLabel*>("batteryPercent");
    if(ql) ql->setText(QString::number(batteryLevel) + QString::fromStdString("%"));
    else cout << "Could not find batteryPercent" << endl;
}

//sets a specified button to a specified color
void MainWindow::SetButtonColor(string color, string object){
    QWidget *qw = this->findChild<QWidget*>(QString::fromStdString(object));
    if(qw) qw->setStyleSheet("background-color: " + QString::fromStdString(color));
    else cout << "Could not find " << object << endl;
}

//All button release logic connected to device
void MainWindow::on_UpButton_released()
{
    if(!setup) return;
    device->UpButton();
}

void MainWindow::on_DownButton_released()
{
    if(!setup) return;
    device->DownButton();
}

void MainWindow::on_SelectButton_released()
{
    if(!setup) return;
    device->SelectButton();
}

void MainWindow::on_LeftButton_released()
{
    if(!setup) return;
    device->DecreaseButton();
}

void MainWindow::on_RightButton_released()
{
    if(!setup) return;
    device->IncreaseButton();
}

void MainWindow::on_PowerButton_released()
{
    if(!setup){
        StartSim();
        setup = true;
    }
    if(device->GetIsOn()) device->TurnOff();
    else device->TurnOn();
}

void MainWindow::on_BackButton_released()
{
    if(!setup) return;
    device->BackButton();
}

void MainWindow::on_MenuButton_released()
{
    if(!setup) return;
    device->MenuButton();
}

void MainWindow::on_pushButton_released()
{
    device->fullCharge();
}
