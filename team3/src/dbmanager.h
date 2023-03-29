#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>
#include <QList>
#include <QApplication>

#include "profile.h"
#include "record.h"
#include "therapy.h"


/* Class Purpose: Manages interactions between application and database
 *
 * Data Members:
 * -QSqlDatabase denasDB: the database object
 * -QString DATE_FORMAT: formats of dates going in/out the database
 * -QString DATABASE_PATH: path of the database
 *
 * Class Functions:
 * Getters for frequencies, profile, recordings, therapies
 *
 * bool addTherapyRecord(const QString& therapy, const QDateTime& time, int powerLevel, int duration): adds a therapy record to the database, returns true if it was added properly, false otherwise
 * bool addFrequencyRecord(const QString& frequency, const QDateTime& time, int powerLevel, int duration): adds a frequency record to the database, returns true if it was added properly, false otherwise
 * bool addProfile(int id, double batterLvl, int powerLvl): adds a profile to the database, returns true if it was added properly, false otherwise
 * bool deleteRecords(): deletes all the records
 *
 * bool isValidRecord(const QString& recordType, const QDateTime& time, int powerLevel, int duration): validates the record, returns true if its valid, false otherwise
 * bool addRecord(const QString& tableName, const QString& name, const QDateTime& time, int powerLevel, int duration): adds a record to the database, returns true if it was added properly, false otherwise
 * bool DBInit(): Initalizes the database
 */

class DBManager {

public:
    const QString DATE_FORMAT = "yyyy-MM-dd hh:mm";
    static const QString DATABASE_PATH;

    DBManager();
    bool addTherapyRecord(const QString& therapy, const QDateTime& time, int powerLevel, int duration);
    bool addFrequencyRecord(const QString& frequency, const QDateTime& time, int powerLevel, int duration);
    bool addProfile(int id, double batterLvl, int powerLvl);
    bool deleteRecords();
    QList<QString>* frequencies();
    QList<Therapy*>* therapies();

    Profile* getProfile(int id);
    QVector<Record*> getRecordings();


private:
    QSqlDatabase denasDB;

    bool isValidRecord(const QString& recordType, const QDateTime& time, int powerLevel, int duration);
    bool addRecord(const QString& tableName, const QString& name, const QDateTime& time, int powerLevel, int duration);
    bool DBInit();

};

#endif // DBMANAGER_H
