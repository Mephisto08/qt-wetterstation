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
using namespace std;

void getWeather(QString location) {
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

    QObject::connect(manager, &QNetworkAccessManager::finished, [](QNetworkReply* reply) {
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

        QObject::connect(manager, &QNetworkAccessManager::finished, [](QNetworkReply* reply) {
            QByteArray data = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            QJsonObject sett2 = doc.object();
            QJsonValue rawResult = sett2.value(QString("current_weather"));
            QJsonObject weather = rawResult.toObject();

            QString time = QString(weather["time"].toString());
            QString temperature = QString::number(weather["temperature"].toDouble());
            QString winddirection = QString::number(weather["winddirection"].toDouble());
            QString windspeed = QString::number(weather["windspeed"].toDouble());

            cout << endl << "WEATHER" << endl;
            cout << "Time -> " << time.toStdString() << endl;
            cout << "Temperature -> " << temperature.toStdString() << endl;
            cout << "Winddirection -> " << winddirection.toStdString() << endl;
            cout << "Windspeed -> " << windspeed.toStdString() << endl;
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
    getWeather("Darmstadt");


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
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

