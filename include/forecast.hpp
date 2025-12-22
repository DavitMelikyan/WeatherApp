#ifndef FORECAST_HPP
#define FORECAST_HPP

#include <QVector>
#include <QDateTime>
#include "weatherdata.hpp"


struct ForecastEntry {
    WeatherData m_data;
    QDateTime m_date;
};

class Forecast {
private:
    QVector<ForecastEntry> m_weathers;
public:
    Forecast();
    QVector<ForecastEntry> getForecast() const;
    void addEntry(const ForecastEntry& wd);
};

#endif // FORECAST_HPP
