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
}

void Device::FindHistory(){
    QWidget* parent = mw->findChild<QWidget*>(QString::fromStdString("historyFrame"));
    QVBoxLayout* layout = new QVBoxLayout(parent);

    layout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    layout->setSpacing(2);
    for(int i = 0; i < MAX_HISTORY; i++){
        fstream inFile;
        inFile.open("data" + to_string(i) + ".venomS3V0N", ios::in);
        if(inFile.is_open()){
            int count = 0;
            string tp;
            string params[10];

            while(getline(inFile, tp)){
                params[count] = tp;
                count++;
            }
            history[i] = new SessionHistory(params[0],       stoi(params[1]), stoi(params[2]), stoi(params[3]),
                                        stof(params[4]), stoi(params[5]), stoi(params[6]), params[7],
                                        params[8],       params[9]);
            inFile.close();

            QWidget* newWig = new QWidget(mw);
            newWig->setFixedSize(361, 51);
            newWig->setStyleSheet(QString::fromStdString("background-color: #94a6b5"));
            newWig->setObjectName(QString::fromStdString("_"+to_string(i)+"Hist"));

            QLabel *newLabel = new QLabel(newWig);
            newLabel->setObjectName(QString::fromStdString("Label"));
            newLabel->setText(QString::fromStdString(params[8] + "   -   " + params[9]));
            newLabel->move(100, 10);
            newLabel->setStyleSheet("font-size: 15px; border: none;");

            newWig->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            layout->addWidget(newWig);
            historySize++;

            inFile.close();
            continue;
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
    if(!isOn) return;

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
    if(!isOn) return;

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

        }else if (subMenu == 1){
            menu = 1;
            qw = mw->findChild<QWidget*>(QString::fromStdString("_1Settings"));
            qw->raise();
        }else if(subMenu == 2){
            menu = 3;
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

        if(!inSession) {

            inSession = true;

            string level = (mw->findChild<QComboBox*>(QString::fromStdString("comboBox"))->currentText()).toStdString();

            QWidget* redOn = mw->findChild<QWidget*>(QString::fromStdString("widget_2"));
            QWidget* blueOn = mw->findChild<QWidget*>(QString::fromStdString("widget_3"));
            QWidget* greenOn = mw->findChild<QWidget*>(QString::fromStdString("widget_4"));

            QLabel* coherenceScoreLabel = mw->findChild<QLabel*>(QString::fromStdString("label_5"));
//            QLabel* timeLabel = mw->findChild<QLabel*>(QString::fromStdString("label_5"));

            redOn->setStyleSheet("background: grey ");
            blueOn->setStyleSheet("background: grey ");
            greenOn->setStyleSheet("background: grey ");

            cout << level << endl;

            int coherentBPM[35] = {70, 75, 79, 80, 79, 75, 70, 65, 61, 60, 61, 65, 70, 75, 79, 80, 79, 75, 70, 65, 61, 60, 61, 65, 70, 75, 79, 80, 79, 75, 70, 65, 61, 60, 61};
            float coherentScores[35] = {14.081, 14.829, 16.554, 17.399, 15.845, 15.777, 17.725, 16.472934586508173, 15.935926114090117, 14.900258192099465, 15.115566295646148, 16.26759823179791, 17.081257666005877, 14.821728441425501, 15.860332733140707, 15.836389979172596, 14.074859428654752, 14.147223012408118, 17.297047764697107, 14.733928776798555, 15.712365417337232, 14.414283728327843, 16.02099466374129, 15.846370583517548, 17.335415168745836, 15.778336567062163, 15.361505580278917, 16.076289112663266, 15.457338108019695, 17.43204522015678, 15.526684559643238, 16.22119603892945, 15.264051068420355, 15.73661848910545, 16.73229392368946};

            int incohenrentBPM[35] = {65, 70, 68, 75, 82, 78, 60, 90, 95, 50, 100, 68, 72, 80, 70, 80, 72, 65, 90, 100, 68, 75, 95, 62, 78, 85, 70, 80, 72, 65, 90, 100, 68, 75, 95};
            float incoherentScores[35] = {0.676, 0.791, 0.532, 0.934, 0.537, 0.984, 0.856, 0.944141690784161, 0.6769622722665572, 0.8650419337478217, 0.5699839310445969, 0.5934419670992245, 0.6693196420040145, 0.8998924687774668, 0.6476111590086364, 0.9062242264757781, 0.9864137777818425, 0.9874011863688554, 0.6881160338002256, 0.6625688002023829, 0.8766976518777016, 0.7111006983701928, 0.8670320304461671, 0.738515875587383, 0.9252620142403182, 0.9527205579636875, 0.9643325547992137, 0.8629243979902797, 0.9607750220316112, 0.797475755921756, 0.8533784841368448, 0.7170554247063467, 0.6048571687289822, 0.7934731865971133, 0.6226393238411937};



            float sum = 0.0;

            if(level == "Low") {

                for(int i = 0; i < sizeof(incoherentScores)/sizeof(int); i++){
                    delay();
                    sum += incoherentScores[i];
                    coherenceScoreLabel->setText(QString::number(sum/(i+1)));

                    updateLEDS(sum/(i+1));

                    if(!inSession) {
                        break;
                    }
                }

            }
            else if(level == "Medium") {
                sleep(1);
                blueOn->setStyleSheet("background: blue ");
            }
            else {

                for(int i = 0; i < sizeof(coherentScores)/sizeof(int); i++){
                    delay();
                    sum += coherentScores[i];
                    coherenceScoreLabel->setText(QString::number(sum/(i+1)));

                    updateLEDS(sum/(i+1));

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

void Device::updateLEDS(float avg) {

    mw->findChild<QWidget*>(QString::fromStdString("widget_2"))->setStyleSheet("background: grey ");
    mw->findChild<QWidget*>(QString::fromStdString("widget_3"))->setStyleSheet("background: grey ");
    mw->findChild<QWidget*>(QString::fromStdString("widget_4"))->setStyleSheet("background: grey ");

    if(avg < 3) {
        mw->findChild<QWidget*>(QString::fromStdString("widget_2"))->setStyleSheet("background: red ");
    }
    else if(avg > 12) {
        mw->findChild<QWidget*>(QString::fromStdString("widget_4"))->setStyleSheet("background: green ");
    }
    else {
        mw->findChild<QWidget*>(QString::fromStdString("widget_3"))->setStyleSheet("background: blue ");
    }
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
