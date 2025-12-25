#include "../include/forecast.hpp"

Forecast::Forecast() {
    m_weathers.reserve(5);
}

QVector<ForecastEntry> Forecast::days() const {
    return m_weathers;
}

void Forecast::addEntry(const ForecastEntry& wd) {
    if (m_weathers.size() < 5) m_weathers.push_back(wd);
    else m_weathers.removeFirst();
}


bool Forecast::operator==(const Forecast& other) const {
    if (m_weathers.size() != other.m_weathers.size()) return false;

    for (int i = 0; i < m_weathers.size(); ++i) {
        const auto& d1 = m_weathers[i];
        const auto& d2 = other.m_weathers[i];

        if (d1.m_date != d2.m_date) return false;
        if (d1.m_condition != d2.m_condition) return false;
        if (d1.m_conditionIcon != d2.m_conditionIcon) return false;
        if (!qFuzzyCompare(d1.m_maxTemp, d2.m_maxTemp)) return false;
        if (!qFuzzyCompare(d1.m_minTemp, d2.m_minTemp)) return false;
        if (d1.m_precipitationChance != d2.m_precipitationChance) return false;
        if (d1.m_sunrise != d2.m_sunrise) return false;
        if (d1.m_sunset != d2.m_sunset) return false;
    }

    return true;
}
bool Forecast::operator!=(const Forecast& other) const {
    return !(*this == other);
}
