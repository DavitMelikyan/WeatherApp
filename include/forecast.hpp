#ifndef FORECAST_HPP
#define FORECAST_HPP

#include <QVector>
#include <QDateTime>


struct ForecastEntry {
    QDateTime m_date;
    double m_maxTemp = 0.0;
    double m_minTemp = 0.0;
    int m_precipitationChance = 0;
    QString m_condition;
    QString m_conditionIcon;
    QDateTime m_sunrise;
    QDateTime m_sunset;
};

class Forecast {
private:
    QVector<ForecastEntry> m_weathers;
public:
    Forecast();
    QVector<ForecastEntry> days() const;
    void addEntry(const ForecastEntry& wd);

    bool operator==(const Forecast& other) const;
    bool operator!=(const Forecast& other) const;
};

#endif // FORECAST_HPP
