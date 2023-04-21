#include "SessionManager.h"

//constructor
SessionManager::SessionManager()
{
//    SaveToFile();
//    LoadFromFile();
}

//find a session, return true if found
bool SessionManager::findSession(string fileName){
    if (FILE *file = fopen(fileName.c_str(), "r")){
        fclose(file);
        return true;
    }
    return false;
}

//load filename from folder
SessionHistory* SessionManager::LoadFromFile(string fileName){
    fstream inFile;
    inFile.open(fileName, ios::in);
    if(inFile.is_open()){
        int count = 0;
        string tp;
        string params[10];

        while(getline(inFile, tp)){
            params[count] = tp;
            count++;
        }
        inFile.close();
        double arr[64];
        count = 0;
        string buffer = "";
        for(int i = 0; i < strlen(params[7].c_str()); i++){
            if(params[7][i] ==  ' '){
                arr[count] = atof(buffer.c_str());
                count++;
                buffer = "";
            }
            else{
                buffer += string(1, params[7][i]);
            }
        }
        return new SessionHistory(params[0], stoi(params[1]), stoi(params[2]), stoi(params[3]), stof(params[4]), stoi(params[5]), stoi(params[6]), arr, params[8], params[9]);
    }
}

//save given data to a file
void SessionManager::SaveToFile(SessionHistory* session, int index){
    cout << "Saving session..." << endl;
    ofstream outFile("data" + to_string(index) + ".txt");

    outFile << session->GetChallengeLevel() << endl;
    outFile << session->GetCoheranceHighTime() <<endl;
    outFile << session->GetCoheranceMedTime() <<endl;
    outFile << session->GetCoheranceLowTime() <<endl;
    outFile << session->GetAverageCoherance() << endl;
    outFile << session->GetToalTime() <<endl;
    outFile << session->GetAchievementScore() << endl;
    string hrvPoints = arrayToString(session->GetHrvPoints());
    outFile << hrvPoints << endl;
    outFile << session->GetDate() <<  endl;
    outFile << session->GetTime() <<  endl;
    cout << "Session saved."<< endl;
    outFile.close();
}

//converts array to a string
string SessionManager::arrayToString(double *arr) {
    std::string str;
    for(int i = 0; i < MAX_POINTS; i++){
        str += std::to_string(arr[i]);
        str += " ";
    }
    return str;
}

//converts a string to an array
bool SessionManager::removeSession(string fileName){
    QString filePath(QString::fromStdString(fileName));
    QFile file(filePath);
    if(file.exists()){
        file.remove();
        return true;
    }
    return false;
}
