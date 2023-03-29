#ifndef PROFILE_H
#define PROFILE_H

class DBManager;


/* Class Purpose: Saves battery level and power level inbetween uses of the app
 *
 * Data Members:
 * - double batteryLevel: a double representation of the battery level
 * - int powerLvl: a integer representation of the power level
 * - int id: a unique identifier
 *
 * Class Functions:
 * - Getters and Setters
 */

class Profile {

public:
    Profile(int, double, int);
    int getId();
    double getBLvl();
    int getPLvl();
    void setBLvl(double);
    void setPLvl(int);

private:
    int id;
    double batteryLvl;
    int powerLvl;

};

#endif // PROFILE_H
