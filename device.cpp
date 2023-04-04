#include "device.h"
#include "mainwindow.h"

Device::Device(MainWindow *_mw)
{
    mw = _mw;
    TurnOff();
    batteryLevel = 10;

    breathPacer = 10;
    challenger = "Beginner";
    SetBatteryLevel();
}

bool Device::GetIsOn(){
    return isOn;
}
void Device::TurnOn(){
    isOn = true;
    menu = 0;
    QWidget *qw = mw->findChild<QWidget*>(QString::fromStdString("MainMenu"));
    if(qw) qw->raise();
    else cout << "Cant find MainMenu" << endl;

    qw = mw->findChild<QWidget*>(QString::fromStdString("OffScreen"));
    if(qw) qw->lower();
    else cout << "Cant find OffScreen" << endl;
}

void Device::TurnOff(){
    isOn = false;
    QWidget *qw = mw->findChild<QWidget*>(QString::fromStdString("OffScreen"));
    if(qw) qw->raise();
    else cout << "Cant find OffScreen" << endl;
}
int Device::GetBatteryLevel(){
    return batteryLevel;
}
void Device::DecreaseBatteryLevel(){
    batteryLevel -= 0.1f;
    SetBatteryLevel();
}

void Device::IncreaseBatteryLevel(){
    batteryLevel += 0.1f;
    SetBatteryLevel();
}

void Device::SetBatteryLevel(){
    mw->SetBatteryLevel(batteryLevel);
    if(batteryLevel < 20.0f){
        mw->SetBatteryColor("red");
    }else if(batteryLevel <= 50.0f){
        mw->SetBatteryColor("orange");
    }else if(batteryLevel < 80.0f){
        mw->SetBatteryColor("blue");
    }else{
        mw->SetBatteryColor("green");
    }
}

void Device::DownButton(){
    if(!isOn) return;
    if(menu == 0){//Main Menu
        if(subMenu == 2){
            subMenu = 0;
            mw->SetButtonColor("#3b6282", "MM_Button1");
            mw->SetButtonColor("#94a6b5", "MM_Button3");
        }else if(subMenu == 1){
            subMenu = 2;
            mw->SetButtonColor("#3b6282", "MM_Button3");
            mw->SetButtonColor("#94a6b5", "MM_Button2");
        }else{
            subMenu = 1;
            mw->SetButtonColor("#94a6b5", "MM_Button1");
            mw->SetButtonColor("#3b6282", "MM_Button2");
        }
    }else if(menu == 1){//Settings Menu
        if(subMenu == 0){
            subMenu = 1;
            mw->SetButtonColor("#94a6b5", "S_Button1");
            mw->SetButtonColor("#3b6282", "S_Button2");
        }else{
            subMenu = 0;
            mw->SetButtonColor("#94a6b5", "S_Button2");
            mw->SetButtonColor("#3b6282", "S_Button1");
        }
    }

}

void Device::UpButton(){
    if(!isOn) return;
    if(menu == 0){
        if(subMenu == 2){
            subMenu = 1;
            mw->SetButtonColor("#3b6282", "MM_Button2");
            mw->SetButtonColor("#94a6b5", "MM_Button3");
        }else if(subMenu == 1){
            subMenu = 0;
            mw->SetButtonColor("#3b6282", "MM_Button1");
            mw->SetButtonColor("#94a6b5", "MM_Button2");
        }else{
            subMenu = 2;
            mw->SetButtonColor("#94a6b5", "MM_Button1");
            mw->SetButtonColor("#3b6282", "MM_Button3");
        }
    }else if(menu == 1){//Settings Menu
        if(subMenu == 0){
            subMenu = 1;
            mw->SetButtonColor("#94a6b5", "S_Button1");
            mw->SetButtonColor("#3b6282", "S_Button2");
        }else{
            subMenu = 0;
            mw->SetButtonColor("#94a6b5", "S_Button2");
            mw->SetButtonColor("#3b6282", "S_Button1");
        }
    }

}

void Device::BackButton(){
    if(!isOn) return;
    menu = lastMenu;
    subMenu = 0;
    QWidget *qw;
    if(menu==0){
        qw = mw->findChild<QWidget*>(QString::fromStdString("MainMenu"));
        mw->SetButtonColor("#3b6282", "MM_Button1");
        mw->SetButtonColor("#94a6b5", "MM_Button2");
        mw->SetButtonColor("#94a6b5", "MM_Button3");
     }else if(menu == 1){
        qw = mw->findChild<QWidget*>(QString::fromStdString("Settings"));
        mw->SetButtonColor("#94a6b5", "S_Button2");
        mw->SetButtonColor("#3b6282", "S_Button1");
    }else if(menu == 2){
        qw = mw->findChild<QWidget*>(QString::fromStdString("HVRScreen"));
    }
    qw->raise();
}

void Device::MenuButton(){
    if(!isOn) return;
    menu = 0;
    subMenu = 0;

    mw->SetButtonColor("#3b6282", "MM_Button1");
    mw->SetButtonColor("#94a6b5", "MM_Button2");
    mw->SetButtonColor("#94a6b5", "MM_Button3");

    QWidget *qw = mw->findChild<QWidget*>(QString::fromStdString("MainMenu"));
    qw->raise();
}


void Device::IncreaseButton(){
    if(!isOn) return;
    if(menu==1){
        if(subMenu == 0){
            IncreaseDifficulty();
        }else if (subMenu == 1){
            IncreaseLevel();
        }
    }
}

void Device::DecreaseButton(){
    if(!isOn) return;
    if(menu==1){
        if(subMenu == 0){
            DecreaseDifficulty();
        }else if (subMenu == 1){
            DecreaseLevel();
        }
    }
}

void Device::SelectButton(){
    if(!isOn) return;
    lastMenu = menu;
    if(menu==0){
        if(subMenu == 0){
            menu = 2;
            QWidget *qw = mw->findChild<QWidget*>(QString::fromStdString("HVRScreen"));
            qw->raise();
        }else if (subMenu == 1){
            menu = 1;
            QWidget *qw = mw->findChild<QWidget*>(QString::fromStdString("Settings"));
            qw->raise();
        }else if(subMenu == 2){

        }
        subMenu = 0;
    }
}

void Device::IncreaseDifficulty(){
    if(!isOn) return;
    if(challenger == "Beginner"){
        challenger = "Intermediate";
    }else if(challenger == "Intermediate"){
        challenger = "Proficient";
    }else if(challenger == "Proficient"){
        challenger = "Advanced";
    }else{
        challenger = "Beginner";
    }

    QLabel *ql = mw->findChild<QLabel*>(QString::fromStdString("CL_Text"));
    if(ql) ql->setText(QString::fromStdString(challenger));
    else cout << "Couldnt find CL_Text" << endl;
}

void Device::DecreaseDifficulty(){
    if(!isOn) return;
    if(challenger == "Beginner"){
        challenger = "Advanced";
    }else if(challenger == "Intermediate"){
        challenger = "Beginner";
    }else if(challenger == "Proficient"){
        challenger = "Intermediate";
    }else{
        challenger = "Proficient";
    }

    QLabel *ql = mw->findChild<QLabel*>(QString::fromStdString("CL_Text"));
    if(ql) ql->setText(QString::fromStdString(challenger));
    else cout << "Couldnt find CL_Text" << endl;
}

void Device::IncreaseLevel(){
    if(!isOn) return;
    if(breathPacer == 30) breathPacer = 1;
    else breathPacer++;

    QLabel *ql = mw->findChild<QLabel*>(QString::fromStdString("BP_Text"));
    if(ql) ql->setText(QString::number(breathPacer));
    else cout << "Couldnt find BP_Text" << endl;
}

void Device::DecreaseLevel(){
    if(!isOn) return;
    if(breathPacer == 1) breathPacer = 30;
    else breathPacer--;

    QLabel *ql = mw->findChild<QLabel*>(QString::fromStdString("BP_Text"));
    if(ql) ql->setText(QString::number(breathPacer));
    else cout << "Couldnt find BP_Text" << endl;
}
