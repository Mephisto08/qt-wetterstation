#ifndef CALLER_H
#define CALLER_H
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QLocale>
#include <QTranslator>

#include <iostream>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCoreApplication>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QQmlComponent>

#include <QTimer>
#include <QDebug>
#include <QString>
#include <QQmlContext>
using namespace std;

class Caller : public QObject
{
    Q_OBJECT
private:
    QString city;
public:
    Caller();
    void getTextfield();

    Q_INVOKABLE void setCity( const QString& abs)
    {
        this->city = abs;
    }

    Q_INVOKABLE inline const QString& getCity() const
    {
        std::string stdCityString = this->city.toStdString();
        cout << "Caller get City: " << stdCityString << endl;
        return this->city;
    }

    ~Caller();

};


#endif // CALLER_H
