#include "therapy.h"

Therapy::Therapy(QString name, int t, QString freq) {

    therapyName = name;
    treatFreq = freq;
    time = t;

    // initialize the timer
    timer = new QTimer(this);
}


Therapy::~Therapy() {
    delete timer;
}


// getters
QString Therapy::getName() { return therapyName; }
int Therapy::getTime() { return time; }
QString Therapy::getFreq() { return treatFreq; }
QTimer* Therapy:: getTimer() { return timer; }
