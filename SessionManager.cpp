#include "SessionManager.h"

SessionManager::SessionManager()
{
//    SaveToFile();
//    LoadFromFile();
}

bool SessionManager::findSession(string fileName){
    if (FILE *file = fopen(fileName.c_str(), "r")){
        fclose(file);
        return true;
    }
    return false;
}

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
        int arr[60];
        count = 0;
        for(int i = 0; i < sizeof(arr)/sizeof(int); i++){
            string buffer = "";
//            cout << params[7] << endl; I EDITD THIS OUT
            for(int j = 0; j < strlen(params[7].c_str()); j++){
                if(params[7][j] ==  ' '){
                    arr[i] = stoi(buffer);
//                    cout << arr[i] << endl;
                    break;
                }
                else{
                    buffer += params[7][j];
                }
            }
//            cout << count <<endl;
            count++;
        }
        return new SessionHistory(params[0], stoi(params[1]), stoi(params[2]), stoi(params[3]), stof(params[4]), stoi(params[5]), stoi(params[6]), arr, params[8], params[9]);
    }
}

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


string SessionManager::arrayToString(int *arr) {
    char str[256] = "";
    for(int i = 0; i < MAX_POINTS; i++){
//        cout << i << " : " << arr[i] << endl;
        strcat(str, to_string(arr[i]).c_str());
        strcat(str, " ");
    }
    return str;
}
