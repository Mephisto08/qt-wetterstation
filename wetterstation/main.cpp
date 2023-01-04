#include "qnetworkreply.h"
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
#include <QQuickItem>
#include "caller.h"
using namespace std;


int main(int argc, char *argv[])
{

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);


    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Wetterstation_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }
    QQmlApplicationEngine engine;


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    //QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
    //                 &app, [url](QObject *obj, const QUrl &objUrl) {
    //    if (!obj && url == objUrl)
    //        QCoreApplication::exit(-1);
    //}, Qt::QueuedConnection);
    //engine.load(url);
    QQmlComponent component(&engine, url);
    QObject *mainPage = component.create();
    Caller* caller = new Caller(mainPage);
    engine.rootContext()->setContextProperty("caller", caller);
    // Initial api call for starting values
    caller->getWeather();

    QTimer timer;
    // for schleife die alle 10 sekunden ausgeführt wird
    QObject::connect(&timer, &QTimer::timeout, [&]() {

        // aktuelles Datum holen
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        // aktuelles Datum in string und dann Qstring umwandeln
        std::ostringstream actDate;
        actDate << std::put_time(&tm, "%d. %b %Y ");
        std::string actDateString = actDate.str();
        QString actDateQString = QString::fromStdString(actDateString);
        // date qml elelment holen und text setzen
        QObject* dateItem = mainPage->findChild<QObject *>("wetterDate");
        dateItem->setProperty("text", actDateQString);

        // aktuelle Zeit in string und dann Qstring umwandeln
        std::ostringstream actTime;
        actTime << std::put_time(&tm, "%H:%M:%S Uhr");
        std::string actTimeString = actTime.str();
        QString actTimeQString = QString::fromStdString(actTimeString);
        // time qml elelment holen und text setzen
        QObject* timeItem = mainPage->findChild<QObject *>("wetterTime");
        timeItem->setProperty("text", actTimeQString);

        // sekunden holen um abzugleichen, dass api call nur alle 60 sek durchgeführt wird
        std::ostringstream seconds;
        seconds << std::put_time(&tm, "%S");
        std::string secondsStr = seconds.str();
        if (secondsStr == "00"){
            caller->getWeather();
        }


        /*else if(secondsStr == "30"){
            QObject* testItem = mainPage->findChild<QObject *>("wetterTitle");
            // testItem->setProperty("text", "temperature");
        }
        //item->setProperty("text", "Hello Ramon!");*/


    });
    timer.start(1000); // 10 Sekunden

    return app.exec();
}

