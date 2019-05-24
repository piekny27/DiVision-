#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QtGlobal>
#include <QtDebug>
#include <QTextStream>
#include "jsonfilehandler.h"


void write(){

    QFile file("work.txt");

    if(!file.open(QFile::WriteOnly | QFile::Text)){

        qDebug()<<"Could not open !";
        return;
    }else{
        qDebug() <<"Opened !";
    }

    QTextStream out(&file);
    out<<"Hello, do u see me?";

    file.flush();
    file.close();
}

void read()
{

    QFile file("work.txt");

    if(!file.open(QFile::ReadOnly | QFile::Text)){

        qDebug()<<"Could not open for reading!";
        return;
    }else{
        qDebug() <<"Opened for reading !";
    }

    QTextStream in(&file);
    QString mText = in.readAll();

    qDebug()<<"Time for reading: ";
    qDebug()<<mText;

    file.close();


}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::cout<<"Hello ?"<<std::endl;

    JsonFileHandler myJs;
    myJs.readJSON();
    myJs.writeJSON();
    return a.exec();
}
