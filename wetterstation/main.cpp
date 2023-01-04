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

#include "caller.h"
using namespace std;


void getWeather(QObject* mainPage, QString location) {
    QTimer timer;
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QString geoUrl = "https://geocoding-api.open-meteo.com/v1/search?count=1&name="+location;

    QNetworkRequest geoRequest;
    timer.start(30000);
    timer.setSingleShot(true);

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1_2);
    geoRequest.setSslConfiguration(config);
    geoRequest.setUrl(QUrl(geoUrl));

    QNetworkReply* rep = manager->get(geoRequest);

    QObject::connect(manager, &QNetworkAccessManager::finished, [mainPage](QNetworkReply* reply) {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject sett2 = doc.object();
        QJsonValue rawResults = sett2.value(QString("results"));
        QJsonArray results = rawResults.toArray();
        QJsonObject result = results[0].toObject();
        QString latitude = QString::number(result["latitude"].toDouble());
        QString longitude = QString::number(result["longitude"].toDouble());

        QTimer timer;
        QNetworkAccessManager* manager = new QNetworkAccessManager();
        QString weatherUrl = "https://api.open-meteo.com/v1/forecast?current_weather=true&latitude="+latitude+"&longitude="+longitude;

        QNetworkRequest weatherRequest;
        timer.start(30000);
        timer.setSingleShot(true);

        QSslConfiguration config = QSslConfiguration::defaultConfiguration();
        config.setProtocol(QSsl::TlsV1_2);
        weatherRequest.setSslConfiguration(config);
        weatherRequest.setUrl(QUrl(weatherUrl));

        QNetworkReply* rep = manager->get(weatherRequest);
        QObject::connect(&timer, &QTimer::timeout, nullptr, [&rep]() { rep->abort(); });


        QObject::connect(manager, &QNetworkAccessManager::finished, [mainPage](QNetworkReply* reply) {
            QByteArray data = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            QJsonObject sett2 = doc.object();
            QJsonValue rawResult = sett2.value(QString("current_weather"));
            QJsonObject weather = rawResult.toObject();

            QString time = QString(weather["time"].toString());
            QString temperature = QString::number(weather["temperature"].toDouble());
            QString winddirection = QString::number(weather["winddirection"].toDouble());
            QString windspeed = QString::number(weather["windspeed"].toDouble());
            //cout << endl << "WEATHER" << endl;
            //cout << "Time -> " << time.toStdString() << endl;
            //cout << "Temperature -> " << temperature.toStdString() << endl;
            QObject* testItem = mainPage->findChild<QObject *>("wetterTitle");
            testItem->setProperty("text", temperature);
            //cout << "Winddirection -> " << winddirection.toStdString() << endl;
            //cout << "Windspeed -> " << windspeed.toStdString() << endl;
        });
        QObject::connect(manager, &QNetworkAccessManager::finished, manager, &QNetworkAccessManager::deleteLater);
        QObject::connect(manager, &QNetworkAccessManager::finished, rep, &QNetworkReply::deleteLater);
    });
    QObject::connect(manager, &QNetworkAccessManager::finished, manager, &QNetworkAccessManager::deleteLater);
    QObject::connect(manager, &QNetworkAccessManager::finished, rep, &QNetworkReply::deleteLater);
}

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
    engine.rootContext()->setContextProperty("caller", new Caller());
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    //QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
    //                 &app, [url](QObject *obj, const QUrl &objUrl) {
    //    if (!obj && url == objUrl)
    //        QCoreApplication::exit(-1);
    //}, Qt::QueuedConnection);
    //engine.load(url);
    QQmlComponent component(&engine, url);
    QObject *mainPage = component.create();

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
            getWeather(mainPage, "Darmstadt");
        }else if(secondsStr == "30"){
            QObject* testItem = mainPage->findChild<QObject *>("wetterTitle");
            testItem->setProperty("text", "temperature");
        }
        //item->setProperty("text", "Hello Ramon!");


    });
    timer.start(1000); // 10 Sekunden

    cout << "Test" << endl;
    return app.exec();
}

