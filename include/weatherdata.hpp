#ifndef WEATHERDATA_HPP
#define WEATHERDATA_HPP


#include <QString>
#include <QDateTime>

struct WeatherData {
public:
    double m_temperature = 0.0;
    double m_feelsLike = 0.0;
    QString m_condition;
    int m_humidity = 0;
    int m_windSpeed = 0;
    int m_pressure = 0;
    int m_uvIndex = 0;
    int m_visibility = 0;
    QDateTime m_sunrise;
    QDateTime m_sunset;
};

#endif // WEATHERDATA_HPP
