#include "sessionhistory.h"

SessionHistory::SessionHistory(string cl, int clt, int cmt, int cht, float ac, int tt, int as, int hrvp[], string d, string t)
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
    for(i = 0; i < tt/5; i++){
        hrvPoints[i] = hrvp[i];
    }
    for(int j = i; j < MAX_POINTS; j++){
        hrvPoints[j] = -1;
    }
    //new code


    date = d;
    time = t;
}

string SessionHistory::GetChallengeLevel() const
{
    return challengeLevel;
}

int SessionHistory::GetCoheranceLowTime() const
{
    return coheranceLowTime;
}

int SessionHistory::GetCoheranceMedTime() const
{
    return coheranceMedTime;
}

int SessionHistory::GetCoheranceHighTime() const
{
    return coheranceHighTime;
}

float SessionHistory::GetAverageCoherance() const
{
    return averageCoherance;
}

int SessionHistory::GetToalTime() const
{
    return toalTime;
}

int SessionHistory::GetAchievementScore() const
{
    return achievementScore;
}

int* SessionHistory::GetHrvPoints()
{
    return hrvPoints;
}

string SessionHistory::GetDate() const
{
    return date;
}

string SessionHistory::GetTime() const
{
    return time;
}

