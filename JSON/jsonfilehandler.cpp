#include "jsonfilehandler.h"

JsonFileHandler::JsonFileHandler(){

}

JsonFileHandler::JsonFileHandler(QString name, QString surname, int age)
{
    this->name = name;
    this->surname = surname;
    this->age = age;
}

void JsonFileHandler::read(const QJsonObject &json){

    name = json["name"].toString();
    surname = json["surname"].toString();
    age = json["age"].toInt();
}


void JsonFileHandler::write(QJsonObject &json) const{

    json["name"] = name;
    json["surname"] = surname;
    json["age"] = age;
}

void JsonFileHandler::readJSON(){

    QString val;
    QFile loadFile(QStringLiteral("hello.json"));
    if (!loadFile.open(QIODevice::ReadOnly)) {
            qWarning("Couldn't open save file.");
            return;
    }else{
        qDebug("It is opened");
    }

    QByteArray saveData = loadFile.readAll();
    loadFile.close();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    read(loadDoc.object());

}

void JsonFileHandler::writeJSON(){

    QFile saveFile(QStringLiteral("testWrite.json"));
    if (!saveFile.open(QIODevice::WriteOnly)) {
            qWarning("Couldn't open write to file.");
            return;
    }else{
        qDebug("It is opened");
    }

    QJsonObject personalDataObject;
    write(personalDataObject);

    QJsonDocument saveDoc(personalDataObject);

    saveFile.write(saveDoc.toJson());
}

void JsonFileHandler::deleteQuotationMarks(QString& str){

    int length = str.length();

    str = str.mid(1, length-2);


}
