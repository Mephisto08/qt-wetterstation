#ifndef CALLER_H
#define CALLER_H
#include <iostream>
#include <QString>
#include <QObject>
using namespace std;

class Caller : public QObject {
    Q_OBJECT

private:
    QString city = "Darmstadt";
    QString temperature = "...";
    QString winddirection = "...";
    QString windspeed = "...";

public:
    Caller() {}
    ~Caller() {}

    Q_INVOKABLE void triggerUpdate() {
        cout << ("TODO: Update weather data") << endl;
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
};


#endif // CALLER_H
