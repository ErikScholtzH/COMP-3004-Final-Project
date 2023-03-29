#ifndef THERAPY_H
#define THERAPY_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QDateTime>


/* Purpose of class: Therapy object to hold information about a therapy
 *
 * Data Members:
 * -int time: A integer representation of how long a therapy lasts
 * - QString therapyName: a QString representation of the therapies name
 * - QTimer* timer: A QTimer object to keep track of how long a therapy should last
 * - QString treatFreq: A string representation of what frequency a therapy needs to be at
 *
 * Class functions:
 * -getters for data members
 */

class Therapy: public QObject {

    Q_OBJECT

    public:
        Therapy(QString therapyName, int t, QString treatFreq);
        ~Therapy();
        QString getName();
        int getTime();
        QString getFreq();
        QTimer* getTimer();   

    private:
        int time;
        QString therapyName;
        QTimer* timer;
        QString treatFreq;

};

#endif // THERAPY_H
