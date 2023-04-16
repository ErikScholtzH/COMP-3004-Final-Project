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
    cout << "Loading sessions..." << endl;
    //setup initializer
    historySize = 0;
    QWidget* parent = mw->findChild<QWidget*>(QString::fromStdString("historyFrame"));
    QVBoxLayout* layout = parent->findChild<QVBoxLayout*>(QString::fromStdString("QV"));
    //if layout already exists, set it

    if(layout != NULL){
        QLayoutItem* item;
        while( (item = layout->takeAt(0) ) != NULL){
            delete item->widget();
            delete item;
        }
    }else{
        layout = new QVBoxLayout(parent);
        layout->setObjectName(QString::fromStdString("QV"));
        layout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
        layout->setSpacing(2);
    }

    for(int i = 0; i < MAX_HISTORY; i++){
        if(sessionManager->findSession("data" + to_string(i) + ".txt")){
            history[i] = sessionManager->LoadFromFile("data" + to_string(i) + ".txt");

            QWidget* newWig = new QWidget(mw);
            newWig->setFixedSize(361, 51);
            newWig->setStyleSheet(QString::fromStdString("background-color: #94a6b5"));
            newWig->setObjectName(QString::fromStdString("_"+to_string(i+1)+"Hist"));

            QLabel *newLabel = new QLabel(newWig);
            newLabel->setObjectName(QString::fromStdString("Label"));
            newLabel->setText(QString::fromStdString(history[i]->GetDate() + "   -   " + history[i]->GetTime()));
            newLabel->move(100, 10);
            newLabel->setStyleSheet("font-size: 15px; border: none;");

            newWig->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            layout->addWidget(newWig);
            historySize++;

            continue;
        }
        if(historySize > 0){
            cout << "Loaded " << historySize << " sessions." << endl;
        }
        else{
            cout << "No sessions found." << endl;
        }
        break;
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
        if(historySize == 0) return;

        mw->SetButtonColor("#94a6b5", "_"+to_string(subMenu)+"Hist");
        subMenu++;
        if(subMenu > historySize) subMenu = 0;
        mw->SetButtonColor("#3b6282", "_"+to_string(subMenu)+"Hist");

        QWidget* parent = mw->findChild<QWidget*>(QString::fromStdString("historyFrame"));
        if(subMenu <= 1)
            parent->move(0, 0);
        else
            parent->move(0, 53 + (-53 * subMenu));
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
        if(historySize == 0) return;

        mw->SetButtonColor("#94a6b5", "_"+to_string(subMenu)+"Hist");
        subMenu--;
        if(subMenu < 0) subMenu = historySize;
        mw->SetButtonColor("#3b6282", "_"+to_string(subMenu)+"Hist");

        QWidget* parent = mw->findChild<QWidget*>(QString::fromStdString("historyFrame"));
        if(subMenu <= 1)
            parent->move(0, 0);
        else
            parent->move(0, 53 + (-53 * subMenu));

    }

}

void Device::BackButton(){
    if(!isOn || inSession) return;

    inSummary = false;
    menu = lastMenu;
    subMenu = 0;
    QWidget *qw;

    mw->findChild<QWidget*>(QString::fromStdString("_0Hist"))->lower();
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

            subMenu = 1;
            mw->SetButtonColor("#3b6282", "_1Hist");
            mw->findChild<QWidget*>(QString::fromStdString("historyFrame"))->move(0, 0);
            mw->findChild<QWidget*>(QString::fromStdString("_0Hist"))->raise();
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


    mw->findChild<QWidget*>(QString::fromStdString("_0MainMenu"))->raise();
    mw->findChild<QWidget*>(QString::fromStdString("__Overview"))->lower();
    mw->findChild<QWidget*>(QString::fromStdString("_0Hist"))->lower();
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
            subMenu = 0;
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
            subMenu = 0;
            qw = mw->findChild<QWidget*>(QString::fromStdString("_1Settings"));
            qw->raise();

        }else if(subMenu == 2){
            menu = 3;

            FindHistory();

            qw = mw->findChild<QWidget*>(QString::fromStdString("_3LogHistory"));
            if(historySize > 0){
                mw->findChild<QWidget*>(QString::fromStdString("_0Hist"))->raise();
                for(int i = 0; i < historySize; i++)
                    mw->SetButtonColor("#94a6b5", "_"+to_string(i)+"Hist");

                mw->SetButtonColor("#3b6282", "_1Hist");

                QWidget* parent = mw->findChild<QWidget*>(QString::fromStdString("historyFrame"));
                parent->move(0, 0);
            }
            qw->raise();
            subMenu = 1;
        }
    }
    else if(menu == 2) {
        runSession();
    }
    else if(menu == 3){
        if(subMenu == 0){
            //reset the data
            resetData();
        }else{
            if(historySize > 0){
                ShowSummary(subMenu-1);
            }
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
    cout << "Starting session..." << endl;
    int PercentInGoodC = 0;
    int PercentInMedC = 0;
    int PercentInBadC = 0;
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
        int totalTime = stoi((mw->findChild<QLabel*>(QString::fromStdString("label_6"))->text()).toStdString());
        int achievementScore = stoi((mw->findChild<QLabel*>(QString::fromStdString("label_7"))->text()).toStdString());
        int coheranceScore = stoi((mw->findChild<QLabel*>(QString::fromStdString("label_5"))->text()).toStdString());

        string level = (mw->findChild<QComboBox*>(QString::fromStdString("comboBox"))->currentText()).toStdString();
        if (level == "Low"){
            PercentInGoodC = 0;
            PercentInMedC = 3;
            PercentInBadC = 97;
        }
        else if (level == "Medium"){
            PercentInGoodC = 3;
            PercentInMedC = 7;
            PercentInBadC = 70;
        }
        else{
            PercentInGoodC = 99;
            PercentInMedC = 1;
            PercentInBadC = 0;
        }

        auto currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
        struct tm* timeinfo = localtime(&currentTime);

        char timeBuffer[6];
        strftime(timeBuffer, sizeof(timeBuffer), "%H:%M", timeinfo);
        string time = timeBuffer;
        char dateBuffer[11];
        strftime(dateBuffer, sizeof(dateBuffer), "%Y/%m/%d", timeinfo);
        string date = dateBuffer;

        cout << "Low : " << PercentInBadC << endl;
        history[historySize] = new SessionHistory(challenger, PercentInBadC, PercentInMedC, PercentInGoodC, coheranceScore, totalTime, achievementScore, y.data(), date, time);
        sessionManager->SaveToFile(history[historySize], historySize);
        historySize++;
        inSession = false;
    }
    cout << "Session finished." << endl;

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
    mw->findChild<QLabel*>(QString::fromStdString("TIL"))->setText(QString::fromStdString(to_string(ses->GetCoheranceLowTime()) + "%"));
    mw->findChild<QLabel*>(QString::fromStdString("TIM"))->setText(QString::fromStdString(to_string(ses->GetCoheranceMedTime()) + "%"));
    mw->findChild<QLabel*>(QString::fromStdString("TIH"))->setText(QString::fromStdString(to_string(ses->GetCoheranceHighTime()) + "%"));
    mw->findChild<QLabel*>(QString::fromStdString("AC"))->setText(QString::fromStdString(to_string(ses->GetAverageCoherance())));
    mw->findChild<QLabel*>(QString::fromStdString("LS"))->setText(QString::fromStdString(to_string(ses->GetToalTime())));
    mw->findChild<QLabel*>(QString::fromStdString("AS"))->setText(QString::fromStdString(to_string(ses->GetAchievementScore())));

    QCustomPlot* myPlot = mw->findChild<QCustomPlot*>(QString::fromStdString("widget_6"));

    myPlot->xAxis->setRange(0, 64);
    myPlot->yAxis->setRange(40, 100);
    QVector<double> xGraph, yGraph;
    xGraph.resize(101);
    yGraph.resize(101);
    double* points = history[num]->GetHrvPoints();
    for(int i = 0; i < MAX_POINTS; i++){
        if(i < history[num]->GetToalTime()){
            xGraph[i] = i;
            yGraph[i] = points[i];
            continue;
        }
        break;
    }

    myPlot->addGraph();
    myPlot->graph(0)->setData(xGraph, yGraph);
    myPlot->replot();


    mw->findChild<QWidget*>(QString::fromStdString("__Overview"))->raise();
}

void Device::resetData(){
    for(int i = 0; i < historySize; i++){
        string fileName = "./data" + to_string(i) + ".txt";
        sessionManager->removeSession(fileName);
    }
    historySize = 0;
}
