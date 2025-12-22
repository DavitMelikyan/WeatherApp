#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <QString>

class Location {
private:
    QString m_city;
    double m_latitude = 0.0;
    double m_longitude = 0.0;
public:
    Location() = default;
    Location(const QString& cityName, double latitude, double longitude);

    QString city() const;
    double latitude() const;
    double longitude() const;
    void setCity(const QString& cityName);
    void setLatitude(double latitude);
    void setLongitude(double longitude);
};

#endif // LOCATION_HPP
