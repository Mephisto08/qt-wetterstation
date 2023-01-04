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
    bool m_ueConneted;
public:
    Caller();
    void getTextfield();

    Q_INVOKABLE void setConnected(const bool& ueConnected, const QString& abs)
    {
        std::string absCpp = abs.toStdString();
        cout << "Caller SET ABCD: " << absCpp << endl;
        this->m_ueConneted = ueConnected;
    }

    Q_INVOKABLE inline const bool& isConnected() const
    {
        cout << "Caller IS EDFG: " << m_ueConneted << endl;
        return this->m_ueConneted;
    }

    ~Caller();

};


#endif // CALLER_H
