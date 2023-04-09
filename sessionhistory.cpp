#include "sessionhistory.h"

SessionHistory::SessionHistory(string cl, int clt, int cmt, int cht, float ac, int tt, int as, string hrvp, string d, string t)
{
    challengeLevel = cl;
    coheranceLowTime = clt;
    coheranceMedTime = cmt;
    coheranceHighTime = cht;
    averageCoherance = ac;
    toalTime = tt;
    achievementScore = as;

    int count = 0;
    char * val = strtok(strdup(hrvp.data()), " ");
    while(val != NULL){
        hrvPoints[count] = stoi(val);
        count++;
        val = strtok(NULL, "");
    }

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

