#ifndef JSONFILEHANDLER_H
#define JSONFILEHANDLER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QChar>
#include <string>
#include <iostream>

class JsonFileHandler
{
    QString name;
    QString surname;
    int age;
public:
    JsonFileHandler();
    JsonFileHandler(QString name, QString surname, int age);
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void readJSON();
    void writeJSON();
    void deleteQuotationMarks(QString& str);
};

#endif // JSONFILEHANDLER_H
