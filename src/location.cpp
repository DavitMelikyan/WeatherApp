#include "../include/location.hpp"

Location::Location(const QString& cityName) : m_city(cityName), m_hasCoordinates(false) {

}

Location::Location(const QString& cityName, double latitude, double longitude) : m_city(cityName), m_latitude(latitude), m_longitude(longitude), m_hasCoordinates(true) {

}

QString Location::city() const {
    return m_city;
}

double Location::latitude() const {
    return m_latitude;
}

double Location::longitude() const {
    return m_longitude;
}

void Location::setCity(const QString& cityName) {
    m_city = cityName;
}

void Location::setLatitude(double latitude) {
    m_latitude = latitude;
    m_hasCoordinates = true;
}

void Location::setLongitude(double longitude) {
    m_longitude = longitude;
    m_hasCoordinates = true;
}

bool Location::hasCoordinates() const {
    return m_hasCoordinates;
}
