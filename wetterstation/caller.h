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
    QString cloudcover = "...";
    QString rainMM = "...";
    QString rainPrecipitaion = "...";
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
    /* Default Setter Getter
    Q_INVOKABLE void setX( const QString& __) {
        this->x = __;
    }

    Q_INVOKABLE inline const QString getX() const {
        return this->x;
    }
    */
    Q_INVOKABLE void setRainPrecipitaion( const QString& __) {
        this->rainPrecipitaion = __;
    }

    Q_INVOKABLE inline const QString getRainPrecipitaion() const {
        return this->rainPrecipitaion;
    }

    Q_INVOKABLE void setRainMM( const QString& rainMM) {
        this->rainMM = rainMM;
    }

    Q_INVOKABLE inline const QString getRainMM() const {
        return this->rainMM;
    }

    Q_INVOKABLE void setCity( const QString& city) {
        this->lastCity = this->city;
        this->city = city;
    }

    Q_INVOKABLE inline const QString getCity() const {
        return this->city;
    }
    Q_INVOKABLE void setCloudcover( const QString& cloudcover) {
        this->cloudcover = cloudcover;
    }

    Q_INVOKABLE inline const QString getCloudcover() const {
        return this->cloudcover;
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

            QString weatherUrl = "https://api.open-meteo.com/v1/forecast?latitude="+latitude+"&longitude="+longitude+"&hourly=temperature_2m,precipitation,weathercode,cloudcover,windspeed_10m,winddirection_10m&daily=weathercode,temperature_2m_max,temperature_2m_min&timezone=Europe%2FBerlin";

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
                QString  data = (QString)reply->readAll();
                auto doc = QJsonDocument::fromJson(data.toUtf8());
                auto sett2 = doc.object();

                // daily
                QJsonValue dailyUnitValue = sett2.value(QString("daily_units"));
                QJsonObject dailyUnitObject = dailyUnitValue.toObject();

                QJsonValue dailyValuesValue = sett2.value(QString("daily"));
                QJsonObject dailyValuesObject = dailyValuesValue.toObject();

                // hourly
                QJsonValue hourlyUnitValue = sett2.value(QString("hourly_units"));
                QJsonObject hourlyUnitObject = hourlyUnitValue.toObject();

                QJsonValue hourlyValuesValue = sett2.value(QString("hourly"));
                QJsonObject hourlyValuesObject = hourlyValuesValue.toObject();

                // aktuelles Stunde holen um Index für JsonArrays zu bekommen
                // da immer neuer Request, erstmal nur ersten 24 Einträge interresant für aktuelle ansicht,
                // wenn stündliche tagesvorraussicht gewollt ist, muss noch der Tag berechnet werden und ein Offset zu dem index addiert werden
                auto t = std::time(nullptr);
                auto tm = *std::localtime(&t);
                std::ostringstream hour;
                hour << std::put_time(&tm, "%H");
                int hourIndex = stoi(hour.str());


                // Temperatur
                QString temperature_2m__UnitStr = hourlyUnitObject["temperature_2m"].toString();
                QString temperature_2m__ValueStr = QString::number(hourlyValuesObject["temperature_2m"][hourIndex].toDouble());
                QObject* wetterTemperatur__Item = this->mainPage->findChild<QObject *>("wetterTemperatur");
                wetterTemperatur__Item->setProperty("text", temperature_2m__ValueStr + temperature_2m__UnitStr);
                this->setTemperature(temperature_2m__ValueStr + temperature_2m__UnitStr);

                // weatherCode
                int weathercodeInt = hourlyValuesObject["weathercode"][hourIndex].toInt();
                QString weathercodeQS = QString::fromStdString(this->wetterCodes[weathercodeInt]);
                QObject* wetterCode__Item = this->mainPage->findChild<QObject *>("wetterCode");
                wetterCode__Item->setProperty("text", weathercodeQS);
                this->setWeatherCode(weathercodeQS);

                /*
                // precipitation
                QString precipitation__UnitStr = hourlyUnitObject["precipitation"].toString();
                QString precipitation__ValueStr = QString::number(hourlyValuesObject["precipitation"][hourIndex].toDouble());
                QObject* wetterRegenwahrschienlichkeit__Item = this->mainPage->findChild<QObject *>("wetterRegenwahrschienlichkeit");
                wetterRegenwahrschienlichkeit__Item->setProperty("text", "Regenwahrsch.: " + precipitation__ValueStr + precipitation__UnitStr);
                this->setRainPrecipitaion("Regenwahrsch.: " + precipitation__ValueStr + precipitation__UnitStr);
                */

                // rainMM
                QString rain__UnitStr = hourlyUnitObject["precipitation"].toString();
                QString rain__ValueStr = QString::number(hourlyValuesObject["precipitation"][hourIndex].toDouble());
                QObject* wetterRegenInMM__Item = this->mainPage->findChild<QObject *>("wetterRegenInMM");
                wetterRegenInMM__Item->setProperty("text", "Nierderschlag: " + rain__ValueStr + rain__UnitStr);
                this->setRainMM("Niederschlag: " + rain__ValueStr + rain__UnitStr);

                // cloudcover
                QString cloudcover__UnitStr = hourlyUnitObject["cloudcover"].toString();
                QString cloudcover__ValueStr = QString::number(hourlyValuesObject["cloudcover"][hourIndex].toDouble());
                QObject* wetterWolkendichte__Item = this->mainPage->findChild<QObject *>("wetterWolkendichte");
                wetterWolkendichte__Item->setProperty("text", "Bedeckung: " + cloudcover__ValueStr + cloudcover__UnitStr);
                this->setCloudcover("Bedeckung: " + cloudcover__ValueStr + cloudcover__UnitStr);

                // windspeed_10m
                QString windspeed_10m__UnitStr = hourlyUnitObject["windspeed_10m"].toString();
                QString windspeed_10m__ValueStr = QString::number(hourlyValuesObject["windspeed_10m"][hourIndex].toDouble());
                QObject* wetterWindgeschindigkeit__Item = this->mainPage->findChild<QObject *>("wetterWindgeschindigkeit");
                wetterWindgeschindigkeit__Item->setProperty("text", "Windgesch.: " + windspeed_10m__ValueStr + windspeed_10m__UnitStr);
                this->setWindspeed("Windgesch.:: " + windspeed_10m__ValueStr + windspeed_10m__UnitStr);

                // winddirection_10m
                QString winddirection_10m_rUnitStr = hourlyUnitObject["winddirection_10m"].toString();
                QString winddirection_10m_ValueStr = QString::number(hourlyValuesObject["winddirection_10m"][hourIndex].toDouble());
                QObject* wetterWolkendichteItem = this->mainPage->findChild<QObject *>("wetterWindRichtung");
                wetterWolkendichteItem->setProperty("text", "Windrichtung: " + winddirection_10m_ValueStr + winddirection_10m_rUnitStr);
                this->setWinddirection("Windrichtung: " + winddirection_10m_ValueStr + winddirection_10m_rUnitStr);

            });
            QObject::connect(manager, &QNetworkAccessManager::finished, manager, &QNetworkAccessManager::deleteLater);
            QObject::connect(manager, &QNetworkAccessManager::finished, rep, &QNetworkReply::deleteLater);
        });
        QObject::connect(manager, &QNetworkAccessManager::finished, manager, &QNetworkAccessManager::deleteLater);
        QObject::connect(manager, &QNetworkAccessManager::finished, rep, &QNetworkReply::deleteLater);
    }
};

#endif // CALLER_H



