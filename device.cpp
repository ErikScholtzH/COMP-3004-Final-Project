#include "device.h"
#include "mainwindow.h"

//constructor
Device::Device(MainWindow *_mw)
{
    mw = _mw;
    TurnOff();
    SetupVariables();
    SetBatteryLevel();
    FindHistory();
}

//sets up the variables
void Device::SetupVariables(){
    inSummary = false;
    isOn = false;
    batteryLevel = 5;
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

    x.resize(500);
    y.resize(500);
}

//finds the user history
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

//returns if device is on
bool Device::GetIsOn(){
    return isOn;
}

//turns on device
void Device::TurnOn(){

    if(GetBatteryLevel() != 0) {
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
}

//turns off device
void Device::TurnOff(){
    isOn = false;
    QWidget *qw = mw->findChild<QWidget*>(QString::fromStdString("OffScreen"));
    if(qw) qw->raise();
    else cout << "Cant find OffScreen" << endl;
}

//returns the battery level
int Device::GetBatteryLevel(){
    return batteryLevel;
}

//decreases the battery level by 0.5
void Device::DecreaseBatteryLevel(){
    batteryLevel -= 0.5f;
    SetBatteryLevel();
}

//increases the battery level by 0.5
void Device::IncreaseBatteryLevel(){
    batteryLevel += 0.5f;
    SetBatteryLevel();
}

void Device::fullCharge() {
    batteryLevel = 100.0f;
    SetBatteryLevel();
}

//changes the batery level colors
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

//controls the logic of when the down button is pressed
void Device::DownButton(){
    if(!isOn || inSummary) return;
    if(menu == 0){//Main Menu
        mw->SetButtonColor("#94a6b5", "MM_Button" +to_string(subMenu+1));
        subMenu++;
        if(subMenu > 3) subMenu = 0;
        mw->SetButtonColor("#3b6282", "MM_Button" +to_string(subMenu+1));
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
    }else if(menu == 4){
        if(!playingSnake)
            PlaySnake();
        dir = 2;
        return;
    }
}

//controls the logic of when the up button is pressed
void Device::UpButton(){
    if(!isOn || inSummary) return;
    if(menu == 0){
        mw->SetButtonColor("#94a6b5", "MM_Button" +to_string(subMenu+1));
        subMenu--;
        if(subMenu < 0) subMenu = 3;
        mw->SetButtonColor("#3b6282", "MM_Button" +to_string(subMenu+1));
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

    }else if(menu == 4){
        if(!playingSnake)
            PlaySnake();
        dir = 3;
        return;
    }

}

//controls the logic of when the back button is pressed
void Device::BackButton(){
    if(!isOn || inSession || playingSnake) return;

    inSummary = false;
    menu = lastMenu;
    subMenu = 0;
    QWidget *qw;

    mw->findChild<QWidget*>(QString::fromStdString("_0Hist"))->lower();
    mw->findChild<QWidget*>(QString::fromStdString("__Overview"))->lower();
    mw->findChild<QWidget*>(QString::fromStdString("_4SnakeGame"))->lower();
    if(menu==0){//Main Menu
        qw = mw->findChild<QWidget*>(QString::fromStdString("_0MainMenu"));
        mw->SetButtonColor("#3b6282", "MM_Button1");
        mw->SetButtonColor("#94a6b5", "MM_Button2");
        mw->SetButtonColor("#94a6b5", "MM_Button3");
        mw->SetButtonColor("#94a6b5", "MM_Button4");
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
    }else if (menu == 4){
        qw = mw->findChild<QWidget*>(QString::fromStdString("_4SnakeGame"));
        SetupSnake();
    }
    qw->raise();
}

//controls the logic of when the menu button is pressed
void Device::MenuButton(){
    if(!isOn || inSession || playingSnake) return;

    inSummary = false;
    menu = 0;
    subMenu = 0;

    mw->SetButtonColor("#3b6282", "MM_Button1");
    mw->SetButtonColor("#94a6b5", "MM_Button2");
    mw->SetButtonColor("#94a6b5", "MM_Button3");
    mw->SetButtonColor("#94a6b5", "MM_Button4");

    mw->findChild<QWidget*>(QString::fromStdString("_0MainMenu"))->raise();
    mw->findChild<QWidget*>(QString::fromStdString("__Overview"))->lower();
    mw->findChild<QWidget*>(QString::fromStdString("_0Hist"))->lower();
    mw->findChild<QWidget*>(QString::fromStdString("_4SnakeGame"))->lower();
}

//controls the logic of when the right button is pressed
void Device::IncreaseButton(){
    if(!isOn || inSummary) return;

    if(menu == 4){
        if(!playingSnake)
            PlaySnake();
        dir = 0;
        return;
    }


    if(menu==1){
        if(subMenu == 0){
            IncreaseDifficulty();
        }else if (subMenu == 1){
            IncreaseLevel();
        }
    }
}

//controls the logic of when the down button is pressed
void Device::DecreaseButton(){
    if(!isOn || inSummary) return;

    if(menu == 4){
        if(!playingSnake)
            PlaySnake();
        dir = 1;
        return;
    }

    if(menu==1){
        if(subMenu == 0){
            DecreaseDifficulty();
        }else if (subMenu == 1){
            DecreaseLevel();
        }
    }
}

//controls the logic of when the select button is pressed
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
            x.resize(500);
            y.resize(500);
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
        }else{
            SetupSnake();
            menu = 4;
            subMenu = 0;
            qw = mw->findChild<QWidget*>(QString::fromStdString("_4SnakeGame"));
            qw->raise();
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
    }else if (menu == 4){
        if(playingSnake) return;
        cout << "Playing Snake" << endl;
        PlaySnake();
    }
}

//increase the difficulty
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

//decrease the difficulty
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

//increase the level
void Device::IncreaseLevel(){
    if(!isOn || inSummary) return;

    if(breathPacer == 30) breathPacer = 1;
    else breathPacer++;

    QLabel *ql = mw->findChild<QLabel*>(QString::fromStdString("BP_Text"));
    if(ql) ql->setText(QString::number(breathPacer));
    else cout << "Couldnt find BP_Text" << endl;
}

//decrease the level
void Device::DecreaseLevel(){
    if(!isOn || inSummary) return;

    if(breathPacer == 1) breathPacer = 30;
    else breathPacer--;

    QLabel *ql = mw->findChild<QLabel*>(QString::fromStdString("BP_Text"));
    if(ql) ql->setText(QString::number(breathPacer));
    else cout << "Couldnt find BP_Text" << endl;
}


//adds a delay of time in seconds
void Device::delay(float time) {
    QTime dieTime = QTime::currentTime().addMSecs(time * 1000.0);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

//Runs the simulation
void Device::runSession() {
    cout << "Starting session..." << endl;
    int PercentInGoodC = 0;
    int PercentInMedC = 0;
    int PercentInBadC = 0;
    if(!inSession) {

        inSession = true;
        string level = (mw->findChild<QComboBox*>(QString::fromStdString("comboBox"))->currentText()).toStdString();

//        QWidget* redOn = mw->findChild<QWidget*>(QString::fromStdString("widget_2"));
//        QWidget* blueOn = mw->findChild<QWidget*>(QString::fromStdString("widget_3"));
//        QWidget* greenOn = mw->findChild<QWidget*>(QString::fromStdString("widget_4"));

        QLabel* coherenceScoreLabel = mw->findChild<QLabel*>(QString::fromStdString("label_5"));
        QLabel* timeLabel = mw->findChild<QLabel*>(QString::fromStdString("label_6"));
        QLabel* achievementScoreLabel = mw->findChild<QLabel*>(QString::fromStdString("label_7"));

//        QLabel* batteryLevel = mw->findChild<QLabel*>(QString::fromStdString("batteryPercent"));

        float val;
        int j = 0;
        float sum = 0.0;

        QCustomPlot* myPlot = mw->findChild<QCustomPlot*>(QString::fromStdString("plot"));

        if(level == "Low") {
            for(int i = 0; i < 84; i++){
                delay(1);
                DecreaseBatteryLevel();

                if(GetBatteryLevel() == 0) {
                    inSession = false;
                    TurnOff();
                }

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
                    x.resize(500);
                    y.resize(500);
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
                delay(1);
                DecreaseBatteryLevel();

                if(GetBatteryLevel() == 0) {
                    inSession = false;
                    TurnOff();
                }

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
                    x.resize(500);
                    y.resize(500);
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
                delay(1);
                DecreaseBatteryLevel();

                if(GetBatteryLevel() == 0) {
                    inSession = false;
                    TurnOff();
                }

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
                    x.resize(500);
                    y.resize(500);
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
        ShowSummary(historySize);
        historySize++;
        inSession = false;
    }
    cout << "Session finished." << endl;

}

//updates the LEDs
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

//updates the breath pacer bar
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

//show a summary of the stats of user
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
    xGraph.resize(500);
    yGraph.resize(500);
    double* points = history[num]->GetHrvPoints();
    for(int i = 0; i < MAX_POINTS; i++){
        if(i < history[num]->GetToalTime()%MAX_POINTS){
            xGraph[i] = i;
            yGraph[i] = points[i];

            continue;
        }
        break;
    }

    for(int i = 0; i < sizeof(points); i++) {
        cout << points[i] << endl;
    }

    myPlot->addGraph();
    myPlot->graph(0)->setData(xGraph, yGraph);
    myPlot->replot();


    mw->findChild<QWidget*>(QString::fromStdString("__Overview"))->raise();
}

//when reset but is hit, reset the user data and delte the files
void Device::resetData(){
    for(int i = 0; i < historySize; i++){
        string fileName = "./data" + to_string(i) + ".txt";
        sessionManager->removeSession(fileName);
    }
    historySize = 0;
}

//setup/cleanup the snake game
void Device::SetupSnake(){
    for(int i = 0; i < snakeScore; i++){
        delete snakeBody[i];
        snakeBody[i] = NULL;
    }
    mw->findChild<QWidget*>(QString::fromStdString("Snake"))->move(190, 120);
    snakeAlive = true;
    snakePosx = 190;
    snakePosy = 120;
    snakeScore = 0;

    MoveFood();
    dir = 0;
    mw->findChild<QLabel*>(QString::fromStdString("StartText"))->setVisible(true);
    mw->findChild<QLabel*>(QString::fromStdString("StartText"))->setText("Press any Key to start");
}

//move snake food to randome place inside play area
void Device::MoveFood(){
    foodPosx = ((rand() % 341) + 20) + 10 / 2;
    foodPosx -= foodPosx % 10;

    foodPosy = ((rand() % 201) + 20) + 10 / 2;
    foodPosy -= foodPosy % 10;

    mw->findChild<QWidget*>(QString::fromStdString("Food"))->move(foodPosx, foodPosy);
}

//Function to control snake game logic and control imputs
void Device::PlaySnake(){
    playingSnake = true;
    mw->findChild<QLabel*>(QString::fromStdString("Score"))->setVisible(true);
    mw->findChild<QLabel*>(QString::fromStdString("StartText"))->setVisible(false);
    while(snakeAlive){

        int lastPosX = snakePosx;
        int lastPosY = snakePosy;

        if(dir <= 1)
            snakePosx += 10 * (dir == 1 ? - 1 : 1);
        else
            snakePosy += 10 * (dir == 3 ? - 1 : 1);

        mw->findChild<QWidget*>(QString::fromStdString("Snake"))->move(snakePosx, snakePosy);

        for(int i = snakeScore-1; i > 0; --i){
           snakeBody[i]->move(snakeBody[i-1]->pos());
        }
        if(snakeBody[0] != NULL) snakeBody[0]->move(lastPosX, lastPosY);

        for(int i = 0; i < snakeScore; i++){
            if(snakeBody[i]->pos() == mw->findChild<QWidget*>(QString::fromStdString("Snake"))->pos()){
                snakeAlive = false;
                break;
            }
        }

        if(snakePosx > 370 || snakePosx < 0 || snakePosy > 230 || snakePosy < 0)
            snakeAlive = false;
        else if(foodPosx == snakePosx && foodPosy == snakePosy){
            MoveFood();
            AddToBody();
            mw->findChild<QLabel*>(QString::fromStdString("Score"))->setText(QString::fromStdString("Score : " + to_string(snakeScore)));
        }

        delay(0.25 - (snakeScore * 0.005));
    }
    mw->findChild<QLabel*>(QString::fromStdString("Score"))->setVisible(false);
    mw->findChild<QLabel*>(QString::fromStdString("StartText"))->setVisible(true);
    mw->findChild<QLabel*>(QString::fromStdString("StartText"))->setText(QString::fromStdString("Game Over\n Score : " + to_string(snakeScore)));
    playingSnake = false;
}

//add 1 block to the snake body
void Device::AddToBody(){
    QWidget* snakeHead = mw->findChild<QWidget*>(QString::fromStdString("Snake"));
    QWidget* newBody = new QWidget(mw->findChild<QWidget*>(QString::fromStdString("_4SnakeGame")));
    newBody->setFixedSize(10, 10);
    newBody->setStyleSheet(QString::fromStdString("border:none;background:red"));
    newBody->setObjectName(QString::fromStdString("_SnakeBody"+to_string(snakeScore)));
    if(snakeScore == 0){//just the head
        if(dir <= 1)
            newBody->move(snakeHead->pos().x() - (10 * (dir == 1 ? -1 : 1)), snakeHead->pos().y());
        else
            newBody->move(snakeHead->pos().x(), snakeHead->pos().y() - 10 * (dir == 3 ? -1 : 1));
    }
    else if(snakeScore == 1){//just head and 1 tail at position 0
        if(snakeHead->pos().x() > snakeBody[0]->pos().x())
            newBody->move(snakeBody[0]->pos().x() - 10, snakeBody[0]->pos().y());
        else if(snakeHead->pos().x() < snakeBody[0]->pos().x())
            newBody->move(snakeBody[0]->pos().x() + 10, snakeBody[0]->pos().y());
        else if(snakeHead->pos().y() > snakeBody[0]->pos().y())
            newBody->move(snakeBody[0]->pos().x(), snakeBody[0]->pos().y() - 10);
        else if(snakeHead->pos().y() > snakeBody[0]->pos().y())
            newBody->move(snakeBody[0]->pos().x(), snakeBody[0]->pos().y() + 10);
    }else{//at least body at 0 and 1, score is at least 2
        if(snakeBody[snakeScore-1]->pos().x() > snakeBody[snakeScore-2]->pos().x())
            newBody->move(snakeBody[snakeScore-1]->pos().x() - 10, snakeBody[snakeScore-1]->pos().y());
        else if(snakeBody[snakeScore-1]->pos().x() < snakeBody[snakeScore-2]->pos().x())
            newBody->move(snakeBody[snakeScore-1]->pos().x() + 10, snakeBody[snakeScore-1]->pos().y());
        else if(snakeBody[snakeScore-1]->pos().y() > snakeBody[snakeScore-2]->pos().y())
            newBody->move(snakeBody[snakeScore-1]->pos().x(), snakeBody[snakeScore-1]->pos().y() + 10);
        else if(snakeBody[snakeScore-1]->pos().y() < snakeBody[snakeScore-2]->pos().y())
            newBody->move(snakeBody[snakeScore-1]->pos().x(), snakeBody[snakeScore-1]->pos().y() - 10);
    }

    snakeBody[snakeScore] = newBody;
    snakeScore++;

    newBody->show();
}
