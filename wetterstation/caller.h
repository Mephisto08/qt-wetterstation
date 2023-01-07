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
#include <QDateTime>
#include <QTimeZone>
#include <map>
#include <tuple>
#include <string>

using namespace std;

class Caller : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<QString> temp24hours READ temp24hours WRITE setTemp24hours NOTIFY temp24hoursChanged)
    Q_PROPERTY(QVector<QString> time24hours READ time24hours WRITE setTime24hours NOTIFY time24hoursChanged)

private:
    QObject* mainPage;
    // key = QML-Label Name => value= API call, Bezeichner auf der Oberfläche vor dem eigentlichen Wert
    std::map<QString, std::tuple<QString, QString>> wetterOptions = {
        {"wetterTemperatur", std::make_tuple("temperature_2m", "")},
        {"wetterRegenInMM", std::make_tuple("precipitation", "Nierschlag: ")},
        {"wetterWolkendichte", std::make_tuple("cloudcover", "Bedeckung: ")},
        {"wetterWindgeschindigkeit", std::make_tuple("windspeed_10m", "Wind: ")},
        {"wetterWindRichtung", std::make_tuple("winddirection_10m", "Richtung: ")},
    };

    QString city = "Darmstadt";
    bool editingOptions = false;
    QString lastCity = "Darmstadt";
    QString timezone = "Europe/Berlin";
    QDateTime actualDate;
    QString temperature = "...";
    QString cloudcover = "...";
    QString rainMM = "...";
    QString rainPrecipitaion = "...";
    QString winddirection = "...";
    QString windspeed = "...";
    QString weatherCode = "...";
    QVector<QString> m_temp24hours;
    QVector<QString> m_time24hours;
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
    std::map<int, std::string> wetterIcons {
        {0,  "24.png"},    // Klarer Himmel
        {1,  "22.png"},    // Hauptsächlich klar
        {2,  "21.png"},    // Teilweise bewölkt
        {3,  "23.png"},    // Bewölkt
        {45, "16.png"},    // Nebel
        {48, "17.png"},    // Ablagerung von Raureifnebel
        {51, "15.png"},    // Nieselregen: Leicht
        {53, "15.png"},    // Nieselregen: Mäßig
        {55, "14.png"},    // Nieselregen: Dicht
        {56, "29.png"},    // Gefriertrocknung: Leicht
        {57, "29.png"},    // Gefriertrocknung: Dicht
        {61, "15.png"},    // Regen: Leicht
        {63, "14.png"},    // Regen: Mäßig
        {65, "14.png"},    // Regen: Schwer
        {66, "8.png"},    // Eisregen: Leicht
        {67, "13.png"},    // Eisregen: Schwer
        {71, "7.png"},    // Schneefall: Leicht
        {73, "7.png"},    // Schneefall: Mäßig
        {75, "6.png"},    // Schneefall: Schwer
        {77, "6.png"},    // Schneekörner
        {80, "12.png"},    // Regenschauer: Leicht
        {81, "12.png"},    // Regenschauer: Mäßig
        {82, "9.png"},    // Regenschauer: Heftig
        {85, "5.png"},    // Schneeschauer: Leicht
        {86, "5.png"},    // Schneeschauer: Schwer
        {95, "2.png"},    // Gewitter: Leicht oder mäßig
        {96, "2.png"},    // Gewitter: Leichter Hagel
        {99, "2.png"},    // Gewitter: Schwerer Hagel
    };

public:
    Caller(QObject* mainPage) {
        this->mainPage = mainPage;
    }
    ~Caller() {}

    QString getTimezone(){
        return this->timezone;
    }

    void setActualDate(QDateTime date){
        this->actualDate = date;
    }
    QDateTime getActualDate(){
        return this->actualDate;
    }

    Q_INVOKABLE void triggerUpdate() {
        this->getWeather();
    }

    Q_INVOKABLE void updateVisibility(QString option, bool checked) {
        string optionString = option.toStdString();
        if(optionString == "Datum"){
            QObject* updateDatum = this->mainPage->findChild<QObject *>("wetterDate");
            updateDatum->setProperty("visible", checked);
        }else if (optionString == "Standort"){
            QObject* updateDatum = this->mainPage->findChild<QObject *>("wetterTitle");
            updateDatum->setProperty("visible", checked);
        }else if (optionString == "Uhrzeit"){
            QObject* updateDatum = this->mainPage->findChild<QObject *>("wetterTime");
            updateDatum->setProperty("visible", checked);

        }else if (optionString == "Wetter Icon"){
            QObject* updateDatum = this->mainPage->findChild<QObject *>("wetterIcon");
            updateDatum->setProperty("visible", checked);
        }else if (optionString == "Temperatur"){
            QObject* updateDatum = this->mainPage->findChild<QObject *>("wetterTemperatur");
            updateDatum->setProperty("visible", checked);
        }else if (optionString == "Wetter Code"){
            QObject* updateDatum = this->mainPage->findChild<QObject *>("wetterCode");
            updateDatum->setProperty("visible", checked);


        }else if (optionString == "Nierderschlag"){
            QObject* updateDatum = this->mainPage->findChild<QObject *>("wetterRegenInMM");
            updateDatum->setProperty("visible", checked);


        }else if (optionString == "Windgeschwindigkeit"){
            QObject* updateDatum = this->mainPage->findChild<QObject *>("wetterWindgeschindigkeit");
            updateDatum->setProperty("visible", checked);
        }else if (optionString == "Windrichtung"){
            QObject* updateDatum = this->mainPage->findChild<QObject *>("wetterWindRichtung");
            updateDatum->setProperty("visible", checked);


        }else if (optionString == "Bewölkung"){
            QObject* updateDatum = this->mainPage->findChild<QObject *>("wetterWolkendichte");
            updateDatum->setProperty("visible", checked);


        }else if (optionString == "24-Stunden Vorschau"){
            QObject* updateDatum = this->mainPage->findChild<QObject *>("scrollBar");
            updateDatum->setProperty("visible", checked);
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
    Q_INVOKABLE void setEditingOptions( const bool editingOptions) {
        this->editingOptions = editingOptions;
    }

    Q_INVOKABLE inline  bool getEditingOptions() const {
        return this->editingOptions;
    }

    Q_INVOKABLE void setRainPrecipitaion( const QString& rainPrecipitaion) {
        this->rainPrecipitaion = rainPrecipitaion;
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

    enum Roles {
        TimeRole = Qt::UserRole + 1,
        TemperatureRole
    };


    QVector<QString> temp24hours() const
    {
        return m_temp24hours;
    }
    QVector<QString> time24hours() const
    {
        return m_time24hours;
    }

    void getWeather() {
        QTimer timer;
        QNetworkAccessManager* manager = new QNetworkAccessManager();
        QString geoUrl = "https://geocoding-api.open-meteo.com/v1/search?count=1&language=de&name="+this->city;

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
            QString timezone = result["timezone"].toString();
            qDebug() << "TIMTIMTIMNT" <<timezone;
            this->timezone = timezone;
            QTimeZone targetTimeZone(this->getTimezone().toUtf8());
            QTimeZone currentTimeZone = QTimeZone::systemTimeZone();

            int currentOffsetFromUtc = currentTimeZone.offsetFromUtc(QDateTime::currentDateTime());
            int targetOffsetFromUtc = targetTimeZone.offsetFromUtc(QDateTime::currentDateTime());

            QDateTime currentDateTime = QDateTime::currentDateTime();
            QDateTime targetDateTime = currentDateTime.addSecs(targetOffsetFromUtc - currentOffsetFromUtc);
            this->setActualDate(targetDateTime);

            QString correctCityName = QString(result["name"].toString());
            this->setCity(correctCityName);
            QObject* wetterTitle = this->mainPage->findChild<QObject *>("wetterTitle");
            wetterTitle->setProperty("text", correctCityName);
            QObject* optionInputCity = this->mainPage->findChild<QObject *>("inputCity");
            optionInputCity->setProperty("text", correctCityName);

            QTimer timer;
            QNetworkAccessManager* manager = new QNetworkAccessManager();

            QString weatherUrl = "https://api.open-meteo.com/v1/forecast?latitude="+latitude+"&longitude="+longitude+"&hourly=temperature_2m,precipitation,weathercode,cloudcover,windspeed_10m,winddirection_10m&daily=weathercode,temperature_2m_max,temperature_2m_min&timezone=" + timezone;

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

                std::string time = this->actualDate.toString("H").toStdString();
                int hourIndex = stoi(time);


                QVector<QString> tmptempvec;
                QVector<QString> tmptimevec;
                for(int i = 0; i < 24; ++i){
                    QString temperature = QString::number(hourlyValuesObject["temperature_2m"][i].toDouble());
                    QString time = hourlyValuesObject["time"][i].toString().mid(11,2)+":00";


                    tmptempvec.push_back(temperature);
                    tmptimevec.push_back(time);
                }
                setTemp24hours(tmptempvec);
                setTime24hours(tmptimevec);

                // Wenn Label welches eine Einheit und Wert hat hier ein else if mit dem setter einfügen. Zusätzlich in map einfügen
                for (const auto& [key, value] : this->wetterOptions)
                {
                    QString __UnitStr = hourlyUnitObject[std::get<0>(value)].toString();
                    QString __ValueStr = QString::number(hourlyValuesObject[std::get<0>(value)][hourIndex].toDouble());

                    if(key.toStdString() == "wetterTemperatur"){
                        this->setTemperature(std::get<1>(value) + __ValueStr + __UnitStr);

                    }else if(key.toStdString() == "wetterRegenInMM"){
                        this->setRainMM(std::get<1>(value) + __ValueStr + __UnitStr);
                    }
                    else if(key.toStdString() == "wetterWolkendichte"){
                        this->setCloudcover(std::get<1>(value) + __ValueStr + __UnitStr);
                    }
                    else if(key.toStdString() == "wetterWindgeschindigkeit"){
                        this->setWindspeed(std::get<1>(value) + __ValueStr + __UnitStr);
                    }
                    else if(key.toStdString() == "wetterWindRichtung"){

                        double windDirection = __ValueStr.toDouble(); // Beispielwert in Grad
                        if (windDirection >= 337.5 || windDirection < 22.5) {
                            __ValueStr = "Nord";
                        } else if (windDirection >= 22.5 && windDirection < 67.5) {
                            __ValueStr = "Nord-Ost";
                        } else if (windDirection >= 67.5 && windDirection < 112.5) {
                            __ValueStr = "Ost";
                        } else if (windDirection >= 112.5 && windDirection < 157.5) {
                            __ValueStr = "Süd-Ost";
                        } else if (windDirection >= 157.5 && windDirection < 202.5) {
                            __ValueStr = "Süd";
                        } else if (windDirection >= 202.5 && windDirection < 247.5) {
                            __ValueStr = "Süd-West";
                        } else if (windDirection >= 247.5 && windDirection < 292.5) {
                            __ValueStr = "West";
                        } else if (windDirection >= 292.5 && windDirection < 337.5) {
                            __ValueStr = "Nord-West";
                        }
                        __UnitStr = "";
                        this->setWinddirection(std::get<1>(value) + __ValueStr);
                    }

                    QObject* __Item = this->mainPage->findChild<QObject *>(key);
                    __Item->setProperty("text", std::get<1>(value) + __ValueStr + __UnitStr);
                }

                int weathercodeInt = hourlyValuesObject["weathercode"][hourIndex].toInt();
                QString weathercodeQS = QString::fromStdString(this->wetterCodes[weathercodeInt]);
                QObject* wetterCode__Item = this->mainPage->findChild<QObject *>("wetterCode");
                wetterCode__Item->setProperty("text", weathercodeQS);
                this->setWeatherCode(weathercodeQS);

                std::string weatherIcon = this->wetterIcons[weathercodeInt];
                QObject* wetterIcon__Item = this->mainPage->findChild<QObject *>("wetterIcon");

                // weatherCode Icon
                std::string directory = "qrc:/images/";
                if(hourIndex > 6 && hourIndex < 20){
                    // day
                    directory += "day/";
                }else{
                    // night
                    directory += "night/";
                }

                QString iconPath = QString::fromStdString(directory + weatherIcon);
                wetterIcon__Item->setProperty("source", iconPath);

                QString backgroundPath = QString::fromStdString(directory + "background.jpeg");
                QObject* background__Item = this->mainPage->findChild<QObject *>("pageBackground");
                background__Item->setProperty("source", backgroundPath);


            });
            QObject::connect(manager, &QNetworkAccessManager::finished, manager, &QNetworkAccessManager::deleteLater);
            QObject::connect(manager, &QNetworkAccessManager::finished, rep, &QNetworkReply::deleteLater);
        });
        QObject::connect(manager, &QNetworkAccessManager::finished, manager, &QNetworkAccessManager::deleteLater);
        QObject::connect(manager, &QNetworkAccessManager::finished, rep, &QNetworkReply::deleteLater);
    }

    void setTemp24hours(const QVector<QString> &temp24hours){
        m_temp24hours = temp24hours;
        emit temp24hoursChanged();
    };

    void setTime24hours(const QVector<QString> &time24hours){
        m_time24hours = time24hours;
        emit time24hoursChanged();
    };

signals:
    void temp24hoursChanged();
    void time24hoursChanged();
};

#endif // CALLER_H



