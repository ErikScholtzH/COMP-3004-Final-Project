#include "device.h"
#include "mainwindow.h"
#include <QDebug>

Device::Device(MainWindow *_mw)
{
    mw = _mw;
    TurnOff();
    SetupVariables();
    SetBatteryLevel();
    FindHistory();
}

void Device::SetupVariables(){
    inSummary = false;
    isOn = false;
    batteryLevel = 70;
    breathPacer = 10;
    menu = 0;
    subMenu = 0;
    lastMenu = 0;
    historySize = 0;
    challenger = "Beginner";
    inSession = false;
    breathLoc = 0;
    breathIn = true;

    sessionManager = new SessionManager();
    measuringDevice = new HeartMeasuringElectrodes();

    x.resize(101);
    y.resize(101);
}

void Device::FindHistory(){
    QWidget* parent = mw->findChild<QWidget*>(QString::fromStdString("historyFrame"));
    QVBoxLayout* layout = new QVBoxLayout(parent);

    layout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    layout->setSpacing(2);
    for(int i = 0; i < MAX_HISTORY; i++){
        if(sessionManager->findSession("data" + to_string(i) + ".txt")){
            history[i] = sessionManager->LoadFromFile("data" + to_string(i) + ".txt");

            QWidget* newWig = new QWidget(mw);
            newWig->setFixedSize(361, 51);
            newWig->setStyleSheet(QString::fromStdString("background-color: #94a6b5"));
            newWig->setObjectName(QString::fromStdString("_"+to_string(i)+"Hist"));

            QLabel *newLabel = new QLabel(newWig);
            newLabel->setObjectName(QString::fromStdString("Label"));
            newLabel->setText(QString::fromStdString(history[i]->GetDate() + "   -   " + history[i]->GetTime()));
            newLabel->move(100, 10);
            newLabel->setStyleSheet("font-size: 15px; border: none;");

            newWig->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            layout->addWidget(newWig);
            historySize++;

            break;
        }
    }

    parent->move(0, 0);
    parent->show();
}

bool Device::GetIsOn(){
    return isOn;
}
void Device::TurnOn(){
    isOn = true;
    menu = 0;
    subMenu = 0;
    QWidget *qw = mw->findChild<QWidget*>(QString::fromStdString("_0MainMenu"));
    if(qw) qw->raise();
    else cout << "Cant find MainMenu" << endl;
    mw->SetButtonColor("#3b6282", "MM_Button1");
    mw->SetButtonColor("#94a6b5", "MM_Button2");
    mw->SetButtonColor("#94a6b5", "MM_Button3");


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
    batteryLevel -= 0.5f;
    SetBatteryLevel();
}

void Device::IncreaseBatteryLevel(){
    batteryLevel += 0.5f;
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
    if(!isOn || inSummary) return;
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
    }else if(menu == 3){
        if(subMenu <= 0){
            mw->SetButtonColor("#94a6b5", "_"+to_string(subMenu)+"Hist");
            subMenu = historySize-1;
            mw->SetButtonColor("#3b6282", "_"+to_string(subMenu)+"Hist");
        }
        else{
            mw->SetButtonColor("#94a6b5", "_"+to_string(subMenu)+"Hist");
            subMenu--;
            mw->SetButtonColor("#3b6282", "_"+to_string(subMenu)+"Hist");
        }
        QWidget* parent = mw->findChild<QWidget*>(QString::fromStdString("historyFrame"));
        parent->move(0, -53 * subMenu);

    }
}

void Device::UpButton(){
    if(!isOn || inSummary) return;
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
    }else if(menu == 3){
        if(subMenu >= historySize-1){
            mw->SetButtonColor("#94a6b5", "_"+to_string(subMenu)+"Hist");
            subMenu = 0;
            mw->SetButtonColor("#3b6282", "_"+to_string(subMenu)+"Hist");
        }
        else{
            mw->SetButtonColor("#94a6b5", "_"+to_string(subMenu)+"Hist");
            subMenu++;
            mw->SetButtonColor("#3b6282", "_"+to_string(subMenu)+"Hist");
        }
        QWidget* parent = mw->findChild<QWidget*>(QString::fromStdString("historyFrame"));
        parent->move(0, -53 * subMenu);
    }

}

void Device::BackButton(){
    if(!isOn || inSession) return;

    inSummary = false;
    menu = lastMenu;
    subMenu = 0;
    QWidget *qw;

    mw->findChild<QWidget*>(QString::fromStdString("__Overview"))->lower();

    if(menu==0){//Main Menu
        qw = mw->findChild<QWidget*>(QString::fromStdString("_0MainMenu"));
        mw->SetButtonColor("#3b6282", "MM_Button1");
        mw->SetButtonColor("#94a6b5", "MM_Button2");
        mw->SetButtonColor("#94a6b5", "MM_Button3");
     }else if(menu == 1){//Settings
        qw = mw->findChild<QWidget*>(QString::fromStdString("_1Settings"));
        mw->SetButtonColor("#94a6b5", "S_Button2");
        mw->SetButtonColor("#3b6282", "S_Button1");
    }else if(menu == 2){//HVR Screen
        qw = mw->findChild<QWidget*>(QString::fromStdString("_2HVRScreen"));
    }else if(menu == 3){//Log History
        qw = mw->findChild<QWidget*>(QString::fromStdString("_3LogHistory"));
        if(historySize > 0){
            for(int i = 0; i < historySize; i++)
                mw->SetButtonColor("#94a6b5", "_"+to_string(i)+"Hist");

            mw->SetButtonColor("#3b6282", "_"+to_string(0)+"Hist");

            QWidget* parent = mw->findChild<QWidget*>(QString::fromStdString("historyFrame"));
            parent->move(0, 0);
        }
        qw->raise();
    }
    qw->raise();
}

void Device::MenuButton(){
    if(!isOn || inSession) return;

    inSummary = false;
    menu = 0;
    subMenu = 0;

    mw->SetButtonColor("#3b6282", "MM_Button1");
    mw->SetButtonColor("#94a6b5", "MM_Button2");
    mw->SetButtonColor("#94a6b5", "MM_Button3");

    QWidget *qw = mw->findChild<QWidget*>(QString::fromStdString("_0MainMenu"));

    qw->raise();

    mw->findChild<QWidget*>(QString::fromStdString("__Overview"))->lower();
}


void Device::IncreaseButton(){
    if(!isOn || inSummary) return;
    if(menu==1){
        if(subMenu == 0){
            IncreaseDifficulty();
        }else if (subMenu == 1){
            IncreaseLevel();
        }
    }
}

void Device::DecreaseButton(){
    if(!isOn || inSummary) return;
    if(menu==1){
        if(subMenu == 0){
            DecreaseDifficulty();
        }else if (subMenu == 1){
            DecreaseLevel();
        }
    }
}

void Device::SelectButton(){
    if(!isOn || inSummary) return;
    if(menu==0){
        lastMenu = menu;
        QWidget *qw;
        if(subMenu == 0){
            menu = 2;
            qw = mw->findChild<QWidget*>(QString::fromStdString("_2HVRScreen"));
            qw->raise();
            mw->findChild<QWidget*>(QString::fromStdString("widget_2"))->setStyleSheet("background: grey ");
            mw->findChild<QWidget*>(QString::fromStdString("widget_3"))->setStyleSheet("background: grey ");
            mw->findChild<QWidget*>(QString::fromStdString("widget_4"))->setStyleSheet("background: grey ");

            mw->findChild<QLabel*>(QString::fromStdString("label_5"))->setText("000.000");
            mw->findChild<QLabel*>(QString::fromStdString("label_6"))->setText("000.000");
            mw->findChild<QLabel*>(QString::fromStdString("label_7"))->setText("000.000");

            mw->findChild<QCustomPlot*>(QString::fromStdString("plot"))->xAxis->setRange(0, 64);
            mw->findChild<QCustomPlot*>(QString::fromStdString("plot"))->yAxis->setRange(40, 100);

            mw->findChild<QProgressBar*>(QString::fromStdString("progressBar"))->setValue(0);
            x.clear();
            y.clear();
            x.resize(101);
            y.resize(101);
            mw->findChild<QCustomPlot*>(QString::fromStdString("plot"))->addGraph();
            mw->findChild<QCustomPlot*>(QString::fromStdString("plot"))->graph(0)->setData(x, y);
            mw->findChild<QCustomPlot*>(QString::fromStdString("plot"))->replot();

            measuringDevice->reset();

            breathLoc = 0;

        }else if (subMenu == 1){
            menu = 1;
            qw = mw->findChild<QWidget*>(QString::fromStdString("_1Settings"));
            qw->raise();
        }else if(subMenu == 2){
            menu = 3;

            FindHistory();
            //new code
            int totalTime = 30;
            int arr[totalTime/5];
            for(int i = 0; i<totalTime/5;i++) arr[i]=i;
            history[0] = new SessionHistory("beginner", 0,0,0,0,totalTime,0, arr, "2023", "2:69");
            //new code


            historySize++;
            sessionManager->SaveToFile(history[0], 24);


            qw = mw->findChild<QWidget*>(QString::fromStdString("_3LogHistory"));
            if(historySize > 0){

                for(int i = 0; i < historySize; i++)
                    mw->SetButtonColor("#94a6b5", "_"+to_string(i)+"Hist");

                mw->SetButtonColor("#3b6282", "_"+to_string(0)+"Hist");

                QWidget* parent = mw->findChild<QWidget*>(QString::fromStdString("historyFrame"));
                parent->move(0, 0);
            }
            qw->raise();
        }
        subMenu = 0;
    }
    else if(menu == 2) {

        runSession();


    }else if(menu == 3){
        if(historySize > 0){
            ShowSummary(subMenu);
        }
    }
}

void Device::IncreaseDifficulty(){
    if(!isOn || inSummary) return;
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
    if(!isOn || inSummary) return;
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
    if(!isOn || inSummary) return;
    if(breathPacer == 30) breathPacer = 1;
    else breathPacer++;

    QLabel *ql = mw->findChild<QLabel*>(QString::fromStdString("BP_Text"));
    if(ql) ql->setText(QString::number(breathPacer));
    else cout << "Couldnt find BP_Text" << endl;
}

void Device::DecreaseLevel(){
    if(!isOn || inSummary) return;
    if(breathPacer == 1) breathPacer = 30;
    else breathPacer--;

    QLabel *ql = mw->findChild<QLabel*>(QString::fromStdString("BP_Text"));
    if(ql) ql->setText(QString::number(breathPacer));
    else cout << "Couldnt find BP_Text" << endl;
}


void Device::delay() {
    QTime dieTime = QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Device::runSession() {

    if(!inSession) {

        inSession = true;

        string level = (mw->findChild<QComboBox*>(QString::fromStdString("comboBox"))->currentText()).toStdString();

        QWidget* redOn = mw->findChild<QWidget*>(QString::fromStdString("widget_2"));
        QWidget* blueOn = mw->findChild<QWidget*>(QString::fromStdString("widget_3"));
        QWidget* greenOn = mw->findChild<QWidget*>(QString::fromStdString("widget_4"));

        QLabel* coherenceScoreLabel = mw->findChild<QLabel*>(QString::fromStdString("label_5"));
        QLabel* timeLabel = mw->findChild<QLabel*>(QString::fromStdString("label_6"));
        QLabel* achievementScoreLabel = mw->findChild<QLabel*>(QString::fromStdString("label_7"));

        QLabel* batteryLevel = mw->findChild<QLabel*>(QString::fromStdString("batteryPercent"));

        cout << "here" << endl;



        float val;
        int j = 0;
        float sum = 0.0;

        QCustomPlot* myPlot = mw->findChild<QCustomPlot*>(QString::fromStdString("plot"));



        if(level == "Low") {

            for(int i = 0; i < 84; i++){
                delay();
                DecreaseBatteryLevel();

                val = measuringDevice->getNextIncoherentScore();
                sum += val;
                coherenceScoreLabel->setText(QString::number(sum/(i+1)));
                timeLabel->setText(QString::number(i+1));


                if(i%5 == 4) {
                    achievementScoreLabel->setText(QString::number(sum));
                }

                updateLEDS(sum/(i+1));
                updateBreathPacerBar();

                x[i] = j;
                y[i] = measuringDevice->getNextIncoherentBPM();

                myPlot->addGraph();
                myPlot->graph(0)->setData(x, y);
                myPlot->replot();

                if(i % 63 == 0) {
                    x.clear();
                    y.clear();
                    x.resize(101);
                    y.resize(101);
                    j = 0;
                }
                j += 1;

                if(!inSession) {
                    break;
                }
            }

        }
        else if(level == "Medium") {
            for(int i = 0; i < 84; i++){
                delay();
                DecreaseBatteryLevel();

                val = measuringDevice->getNextMidcoherentScore();
                sum += val;
                coherenceScoreLabel->setText(QString::number(sum/(i+1)));
                timeLabel->setText(QString::number(i+1));


                if(i%5 == 4) {
                    achievementScoreLabel->setText(QString::number(sum));
                }

                updateLEDS(sum/(i+1));
                updateBreathPacerBar();

                x[i] = j;
                y[i] = measuringDevice->getNextMidcoherentBPM();

                myPlot->addGraph();
                myPlot->graph(0)->setData(x, y);
                myPlot->replot();

                if(i % 63 == 0) {
                    x.clear();
                    y.clear();
                    x.resize(101);
                    y.resize(101);
                    j = 0;
                }
                j += 1;

                if(!inSession) {
                    break;
                }
            }

        }
        else {

            for(int i = 0; i < 84; i++){
                delay();
                DecreaseBatteryLevel();

                val = measuringDevice->getNextCoherentScore();
                sum += val;
                coherenceScoreLabel->setText(QString::number(sum/(i+1)));
                timeLabel->setText(QString::number(i+1));


                if(i%5 == 4) {
                    achievementScoreLabel->setText(QString::number(sum));
                }

                updateLEDS(sum/(i+1));
                updateBreathPacerBar();

                x[i] = j;
                y[i] = measuringDevice->getNextCoherentBPM();

                myPlot->addGraph();
                myPlot->graph(0)->setData(x, y);
                myPlot->replot();

                if(i % 63 == 0) {
                    x.clear();
                    y.clear();
                    x.resize(101);
                    y.resize(101);
                    j = 0;
                }
                j += 1;

                if(!inSession) {
                    break;
                }
            }

        }
    }
    else {
        inSession = false;
    }




    //drawresults
}


void Device::updateLEDS(float avg) {

    mw->findChild<QWidget*>(QString::fromStdString("widget_2"))->setStyleSheet("background: grey ");
    mw->findChild<QWidget*>(QString::fromStdString("widget_3"))->setStyleSheet("background: grey ");
    mw->findChild<QWidget*>(QString::fromStdString("widget_4"))->setStyleSheet("background: grey ");

    float low;
    float high;

    if(challenger == "Beginner") {
        low = 0.5;
        high = 0.9;
    }
    else if(challenger == "Intermediate") {
        low = 0.6;
        high = 2.1;
    }
    else if(challenger == "Proficient") {
        low = 1.8;
        high = 4.0;
    }
    else {
        low = 4.0;
        high = 6.0;
    }


    if(avg < low) {
        mw->findChild<QWidget*>(QString::fromStdString("widget_2"))->setStyleSheet("background: red ");
    }
    else if(avg > high) {
        mw->findChild<QWidget*>(QString::fromStdString("widget_4"))->setStyleSheet("background: green ");
    }
    else {
        mw->findChild<QWidget*>(QString::fromStdString("widget_3"))->setStyleSheet("background: blue ");
    }
}

void Device::updateBreathPacerBar() {

    if(breathLoc == breathPacer) {
        breathIn = false;
    }
    else if(breathLoc == 0) {
        breathIn = true;
    }

    if(breathIn) {
        breathLoc += 1;
    }
    else {
        breathLoc -= 1;
    }

    mw->findChild<QProgressBar*>(QString::fromStdString("progressBar"))->setValue((float(breathLoc)/float(breathPacer))*100);
}

void Device::ShowSummary(int num){
    inSummary = true;
    if(historySize < num) cout << "Dumb dumb alert, somebody is begging for a stack overflow";
    SessionHistory * ses = history[num];
    mw->findChild<QLabel*>(QString::fromStdString("CL"))->setText(QString::fromStdString(ses->GetChallengeLevel()));
    mw->findChild<QLabel*>(QString::fromStdString("TIL"))->setText(QString::fromStdString(to_string(ses->GetCoheranceLowTime())));
    mw->findChild<QLabel*>(QString::fromStdString("TIM"))->setText(QString::fromStdString(to_string(ses->GetCoheranceMedTime())));
    mw->findChild<QLabel*>(QString::fromStdString("TIH"))->setText(QString::fromStdString(to_string(ses->GetCoheranceHighTime())));
    mw->findChild<QLabel*>(QString::fromStdString("AC"))->setText(QString::fromStdString(to_string(ses->GetAverageCoherance())));
    mw->findChild<QLabel*>(QString::fromStdString("LS"))->setText(QString::fromStdString(to_string(ses->GetToalTime())));
    mw->findChild<QLabel*>(QString::fromStdString("AS"))->setText(QString::fromStdString(to_string(ses->GetAchievementScore())));
    mw->findChild<QWidget*>(QString::fromStdString("__Overview"))->raise();
}
