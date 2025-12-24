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

    bool operator==(const WeatherData& other) const {
        return qFuzzyCompare(m_temperature, other.m_temperature) &&
               qFuzzyCompare(m_feelsLike, other.m_feelsLike) &&
               m_condition == other.m_condition &&
               m_humidity == other.m_humidity &&
               m_windSpeed == other.m_windSpeed &&
               m_pressure == other.m_pressure &&
               m_uvIndex == other.m_uvIndex &&
               m_visibility == other.m_visibility;
    }
    bool operator!=(const WeatherData& other) const {
        return !(*this == other);
    }
};


#endif // WEATHERDATA_HPP
