#ifndef SESSIONHISTORY_H
#define SESSIONHISTORY_H

#define MAX_POINTS 64
using namespace std;
#include <iostream>
#include <cstdio>
#include <bits/stdc++.h>

class SessionHistory
{
public:
    SessionHistory(string cl, int clt, int cmt, int cht, float ac, int tt, int as, double hrvp[], string d, string t);
    friend ostream& operator <<(ostream& os, SessionHistory const & sh){
        return os << " challengeLevel: " << sh.challengeLevel << " coheranceLowTime: " << sh.coheranceLowTime << " coheranceMedTime: " << sh.coheranceMedTime << " coheranceHighTime: " << sh.coheranceHighTime << " averageCoherance: " << sh.averageCoherance << " toalTime: " << sh.toalTime << " achievementScore: " << sh.achievementScore << " date: " << sh.date << " time: " << sh.time << endl;
    }
    string GetChallengeLevel();
    int GetCoheranceLowTime();
    int GetCoheranceMedTime();
    int GetCoheranceHighTime();
    float GetAverageCoherance();
    int GetToalTime();
    int GetAchievementScore();
    double* GetHrvPoints();
    string GetDate();
    string GetTime();
    
private:
    string challengeLevel;
    int coheranceLowTime;
    int coheranceMedTime;
    int coheranceHighTime;
    float averageCoherance;
    int toalTime;
    int achievementScore;
    double hrvPoints[MAX_POINTS];
    string date;
    string time;

};

#endif // SESSIONHISTORY_H
