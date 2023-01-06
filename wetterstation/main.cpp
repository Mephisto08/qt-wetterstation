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
#include <QDateTime>
#include <QTimeZone>
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

        QTimeZone targetTimeZone(caller->getTimezone().toUtf8());
        QTimeZone currentTimeZone = QTimeZone::systemTimeZone();

        int currentOffsetFromUtc = currentTimeZone.offsetFromUtc(QDateTime::currentDateTime());
        int targetOffsetFromUtc = targetTimeZone.offsetFromUtc(QDateTime::currentDateTime());

        QDateTime currentDateTime = QDateTime::currentDateTime();
        QDateTime targetDateTime = currentDateTime.addSecs(targetOffsetFromUtc - currentOffsetFromUtc);
        caller->setActualDate(targetDateTime);

        QString date = targetDateTime.toString("d. MMM yyyy");
        QString time = targetDateTime.toString("HH:mm:ss") + " Uhr";
        std::string seconds = targetDateTime.toString("ss").toStdString();

        // date qml elelment holen und text setzen
        QObject* dateItem = mainPage->findChild<QObject *>("wetterDate");
        dateItem->setProperty("text", date);

        // time qml elelment holen und text setzen
        QObject* timeItem = mainPage->findChild<QObject *>("wetterTime");
        timeItem->setProperty("text", time);

        if (seconds == "00" && !caller->getEditingOptions()){
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

