#include "sessionhistory.h"

//constructor
SessionHistory::SessionHistory(string cl, int clt, int cmt, int cht, float ac, int tt, int as, double hrvp[], string d, string t)
{
//    cout << "3: " <<hrvp[3] << "4: " <<hrvp[4] << "5: " <<hrvp[5] <<endl;
    challengeLevel = cl;
    coheranceLowTime = clt;
    coheranceMedTime = cmt;
    coheranceHighTime = cht;
    averageCoherance = ac;
    toalTime = tt;
    achievementScore = as;


    //new code
    int i;
    int counter = tt / 64 * 64;
    for(i = 0; i < tt % 64; i++){
        hrvPoints[i] = hrvp[counter];
        counter++;
    }
    for(int j = i; j < MAX_POINTS; j++){
        hrvPoints[j] = -1;
    }
    //new code


    date = d;
    time = t;
}

//getters and setters
string SessionHistory::GetChallengeLevel()
{
    return challengeLevel;
}

int SessionHistory::GetCoheranceLowTime()
{
    return coheranceLowTime;
}

int SessionHistory::GetCoheranceMedTime()
{
    return coheranceMedTime;
}

int SessionHistory::GetCoheranceHighTime()
{
    return coheranceHighTime;
}

float SessionHistory::GetAverageCoherance()
{
    return averageCoherance;
}

int SessionHistory::GetToalTime()
{
    return toalTime;
}

int SessionHistory::GetAchievementScore()
{
    return achievementScore;
}

double* SessionHistory::GetHrvPoints()
{
    return hrvPoints;
}

string SessionHistory::GetDate()
{
    return date;
}

string SessionHistory::GetTime()
{
    return time;
}

