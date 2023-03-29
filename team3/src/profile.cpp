#include "profile.h"

Profile::Profile(int id, double batteryLvl, int powerLvl) {

    this->id = id;
    this->batteryLvl = batteryLvl;
    this->powerLvl = powerLvl;
}


// getters
int Profile::getId() { return id; }
double Profile::getBLvl() { return batteryLvl; }
int Profile::getPLvl() { return powerLvl; }


// setters
void Profile::setBLvl(double newLvl) { batteryLvl = newLvl; }
void Profile::setPLvl(int newLvl) { powerLvl = newLvl; }
