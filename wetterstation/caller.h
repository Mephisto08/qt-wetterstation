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
using namespace std;

class Caller : public QObject {
    Q_OBJECT

private:
    QObject* mainPage;
    QString city = "Darmstadt";
    QString temperature = "...";
    QString winddirection = "...";
    QString windspeed = "...";

public:
    Caller(QObject* mainPage) {
        this->mainPage = mainPage;
    }
    ~Caller() {}

    Q_INVOKABLE void triggerUpdate() {
        this->getWeather();
    }

    Q_INVOKABLE void setCity( const QString& city) {
        this->city = city;
    }

    Q_INVOKABLE inline const QString getCity() const {
        return this->city;
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

                this->setTemperature(temperature);
                this->setWinddirection(winddirection);
                this->setWindspeed(windspeed);

                QObject* wetterTemp = this->mainPage->findChild<QObject *>("wetterTemperatur");
                wetterTemp->setProperty("text", temperature);
                QObject* wetterWindSpeed = this->mainPage->findChild<QObject *>("wetterWindgeschindigkeit");
                wetterWindSpeed->setProperty("text", windspeed);
                QObject* wetterWindDir = this->mainPage->findChild<QObject *>("wetterRichtung");
                wetterWindDir->setProperty("text", winddirection);

            });
            QObject::connect(manager, &QNetworkAccessManager::finished, manager, &QNetworkAccessManager::deleteLater);
            QObject::connect(manager, &QNetworkAccessManager::finished, rep, &QNetworkReply::deleteLater);
        });
        QObject::connect(manager, &QNetworkAccessManager::finished, manager, &QNetworkAccessManager::deleteLater);
        QObject::connect(manager, &QNetworkAccessManager::finished, rep, &QNetworkReply::deleteLater);
    }
};

#endif // CALLER_H



