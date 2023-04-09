#include "poc.h"

POC::POC()
{
//    SaveToFile();
    LoadFromFile();
}

void POC::LoadFromFile(){

    ifstream inFile("data.venomS3V0N");

    char data[100];
    inFile >> data;
    cout << "Data : \n" << data << endl;

    inFile.close();
}

void POC::SaveToFile(){
    cout << "saving file"<<endl;
    ofstream outFile("data.venomS3V0N");
    outFile << "Haxk."<<endl;
    outFile.close();
}

void POC::CheckForFile(string name){

}
