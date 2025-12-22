#include "../include/forecast.hpp"

Forecast::Forecast() {
    m_weathers.reserve(5);
}

QVector<ForecastEntry> Forecast::getForecast() const {
    return m_weathers;
}

void Forecast::addEntry(const ForecastEntry& wd) {
    if (m_weathers.size() < 5) m_weathers.push_back(wd);
    else m_weathers.removeFirst();
}
