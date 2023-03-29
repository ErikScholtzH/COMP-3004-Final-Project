#include "record.h"

Record::Record(const QString& treatment, const QDateTime& startTime, const int powerLevel, const int duration) {

    if (startTime.isValid()) {
        this->treatment = treatment;
        this->startTime = startTime;
        this->powerLevel = powerLevel;
        this->duration = duration;
    }
}


QString Record::toString() {
    QString newString =
            startTime.toString("ddd h:mm ap") + "\n"
            + "   Therapy: " + treatment + "\n"
            + "   Power Level: " + QString::number(powerLevel) + "\n"
            + "   Duration: " + QString::number(duration/60) + ((duration%60 < 10) ? + ":0" + QString::number(duration%60) : + ":" + QString::number(duration%60));

    return newString;
}


// getters
QString Record::getTreatment() { return treatment; }
QDateTime Record::getStartTime() { return startTime; }
int Record::getPowerLevel() { return powerLevel; }
int Record::getDuration() { return duration; }


// setters
void Record::setPowerLevel(int p) { powerLevel = p; }
void Record::setDuration(int d) { duration = d; }
