#ifndef POC_H
#define POC_H

using namespace std;
#include <iostream>
#include <fstream>
#include <string.h>
#include <QFile>
#include <QString>

#include "sessionhistory.h"

#define MAX_STR 128
#define MAX_DATE 10

struct Setting {
    float breathPacing;
    float delay;
};

class SessionManager
{
public:
    SessionManager();
    bool findSession(string fileName);
    void SaveToFile(SessionHistory* session, int index);
    SessionHistory* LoadFromFile(string fileName);
    string arrayToString(double *arr);
    bool removeSession(string fileName);

private:
    string name;
    string date;
    float coheranceLevel;
    int bpmRecorded;
    int breathingLevelOfUser;

};




#endif // SESSIONMANAGER_H
