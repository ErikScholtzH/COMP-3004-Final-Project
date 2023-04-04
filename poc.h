#ifndef POC_H
#define POC_H

using namespace std;
#include <iostream>
#include <fstream>

#define MAX_STR 128
#define MAX_DATE 10

struct Setting {
    float breathPacing;
    float delay;
};

class POC
{
public:
    POC();
    void SaveToFile();
    void LoadFromFile();
    void CheckForFile(string name);
private:
    string name;
    string date;
    float coheranceLevel;
    int bpmRecorded;
    int breathingLevelOfUser;

};




#endif // POC_H
