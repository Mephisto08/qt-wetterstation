#ifndef CALLER_H
#define CALLER_H
#include <iostream>
#include <QObject>
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

#include <map>
#include <string>

using namespace std;


class Caller : public QObject {
    Q_OBJECT

private:
    QObject* mainPage;
    QString city = "Darmstadt";
    QString lastCity = "Darmstadt";
    QString temperature = "...";
    QString winddirection = "...";
    QString windspeed = "...";
    QString weatherCode = "...";
    std::map<int, std::string> wetterCodes {
        {0, "Klarer Himmel"},
        {1, "Hauptsächlich klar"},
        {2, "Teilweise bewölkt"},
        {3, "Bewölkt"},
        {45, "Nebel"},
        {48, "Ablagerung von Raureifnebel"},
        {51, "Nieselregen: Leicht"},
        {53, "Nieselregen: Mäßig"},
        {55, "Nieselregen: Dicht"},
        {56, "Gefriertrocknung: Leicht"},
        {57, "Gefriertrocknung: Dicht"},
        {61, "Regen: Leicht"},
        {63, "Regen: Mäßig"},
        {65, "Regen: Schwer"},
        {66, "Eisregen: Leicht"},
        {67, "Eisregen: Schwer"},
        {71, "Schneefall: Leicht"},
        {73, "Schneefall: Mäßig"},
        {75, "Schneefall: Schwer"},
        {77, "Schneekörner"},
        {80, "Regenschauer: Leicht"},
        {81, "Regenschauer: Mäßig"},
        {82, "Regenschauer: Heftig"},
        {85, "Schneeschauer: Leicht"},
        {86, "Schneeschauer: Schwer"},
        {95, "Gewitter: Leicht oder mäßig"},
        {96, "Gewitter: Leichter Hagel"},
        {99, "Gewitter: Schwerer Hagel"}
    };

public:
    Caller(QObject* mainPage) {
        this->mainPage = mainPage;
    }
    ~Caller() {}

    Q_INVOKABLE void triggerUpdate() {
        try {
            this->getWeather();
        } catch (...) {
        }

    }

    Q_INVOKABLE void setCity( const QString& city) {
        this->lastCity = this->city;
        this->city = city;
    }

    Q_INVOKABLE inline const QString getCity() const {
        return this->city;
    }

    Q_INVOKABLE void setWeatherCode( const QString& weatherCode) {
        this->weatherCode = weatherCode;
    }

    Q_INVOKABLE inline const QString getWeatherCode() const {
        return this->weatherCode;
    }

    Q_INVOKABLE void setTemperature( const QString& temperature) {
        this->temperature = temperature;
    }

    Q_INVOKABLE inline const QString getTemperature() const {
        return this->temperature;
    }

    Q_INVOKABLE void setWinddirection( const QString& winddirection) {
        this->winddirection = winddirection;
    }

    Q_INVOKABLE inline const QString getWinddirection() const {
        return this->winddirection;
    }

    Q_INVOKABLE void setWindspeed( const QString& windspeed) {
        this->windspeed = windspeed;
    }

    Q_INVOKABLE inline const QString getWindspeed() const {
        return this->windspeed;
    }
    void getWeather() {
        QTimer timer;
        QNetworkAccessManager* manager = new QNetworkAccessManager();
        QString geoUrl = "https://geocoding-api.open-meteo.com/v1/search?count=1&name="+this->city;

        QNetworkRequest geoRequest;
        timer.start(30000);
        timer.setSingleShot(true);

        QSslConfiguration config = QSslConfiguration::defaultConfiguration();
        config.setProtocol(QSsl::TlsV1_2);
        geoRequest.setSslConfiguration(config);
        geoRequest.setUrl(QUrl(geoUrl));

        QNetworkReply* rep = manager->get(geoRequest);

        QObject::connect(manager, &QNetworkAccessManager::finished, [this](QNetworkReply* reply) {
            QByteArray data = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            QJsonObject sett2 = doc.object();
            QJsonValue rawResults = sett2.value(QString("results"));
            QJsonArray results = rawResults.toArray();
            if (results.isEmpty()){
                QObject* wetterTitle = this->mainPage->findChild<QObject *>("wetterTitle");
                wetterTitle->setProperty("text", this->lastCity);
                QObject* optionInputCity = this->mainPage->findChild<QObject *>("inputCity");
                optionInputCity->setProperty("text", this->lastCity);
                this->city = this->lastCity;
                return;
            }
            QJsonObject result = results[0].toObject();

            QString latitude = QString::number(result["latitude"].toDouble());
            QString longitude = QString::number(result["longitude"].toDouble());

            QString correctCityName = QString(result["name"].toString());
            this->setCity(correctCityName);
            QObject* wetterTitle = this->mainPage->findChild<QObject *>("wetterTitle");
            wetterTitle->setProperty("text", correctCityName);
            QObject* optionInputCity = this->mainPage->findChild<QObject *>("inputCity");
            optionInputCity->setProperty("text", correctCityName);

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


            QObject::connect(manager, &QNetworkAccessManager::finished, [this](QNetworkReply* reply) {
                QByteArray data = reply->readAll();
                QJsonDocument doc = QJsonDocument::fromJson(data);
                QJsonObject sett2 = doc.object();
                QJsonValue rawResult = sett2.value(QString("current_weather"));
                QJsonObject weather = rawResult.toObject();


                // TODO:
                // alle Daten die wir benötigen aus Request holen und in this-> varibalen speichern (falls neue...getter und setter anlegen)
                // Danach qml elemente setzten mit ausgelesen werten der api

                QString temperature = QString::number(weather["temperature"].toDouble());
                QString windspeed = QString::number(weather["windspeed"].toDouble());
                QString winddirection = QString::number(weather["winddirection"].toDouble());
                int wetterCode = weather["weathercode"].toInt();
                QString weatherCodeQS = QString::fromStdString(this->wetterCodes[wetterCode]);

                this->setTemperature(temperature);
                this->setWinddirection(winddirection);
                this->setWindspeed(windspeed);
                this->setWeatherCode(weatherCodeQS);

                QObject* wetterTempItem = this->mainPage->findChild<QObject *>("wetterTemperatur");
                wetterTempItem->setProperty("text", temperature);
                QObject* wetterWindSpeedItem = this->mainPage->findChild<QObject *>("wetterWindgeschindigkeit");
                wetterWindSpeedItem->setProperty("text", windspeed);
                QObject* wetterWindDirItem = this->mainPage->findChild<QObject *>("wetterRichtung");
                wetterWindDirItem->setProperty("text", winddirection);
                QObject* wetterCodeItem = this->mainPage->findChild<QObject *>("wetterCode");
                wetterCodeItem->setProperty("text", weatherCodeQS);


            });
            QObject::connect(manager, &QNetworkAccessManager::finished, manager, &QNetworkAccessManager::deleteLater);
            QObject::connect(manager, &QNetworkAccessManager::finished, rep, &QNetworkReply::deleteLater);
        });
        QObject::connect(manager, &QNetworkAccessManager::finished, manager, &QNetworkAccessManager::deleteLater);
        QObject::connect(manager, &QNetworkAccessManager::finished, rep, &QNetworkReply::deleteLater);
    }
};

#endif // CALLER_H



