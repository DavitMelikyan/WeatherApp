#ifndef CLIENTAPI_HPP
#define CLIENTAPI_HPP

#include <QObject>
#include "location.hpp"
#include "weatherdata.hpp"
#include "forecast.hpp"
#include <QDateTime>
#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonArray>
#include <QJsonDocument>

enum class WeatherApiError {
    None,
    NetworkError,
    InvalidResponse,
    CityNotFound
};


class ClientApi : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager m_networkManager;
    QString m_apiKey;
    Location m_location;
    const QString m_baseUrl;
    QDateTime m_lastFetchTime;
    WeatherData m_currentWeather;
    Forecast m_forecast;
    int m_cacheDuration;
    WeatherApiError m_lastError;

    WeatherData parseCurrentWeatherJson(const QJsonObject& json);
    Forecast parseForecastJson(const QJsonObject& json);
    bool isCacheValid() const;
    QNetworkRequest createRequest(const QString& endpoint) const;
    void logDebug(const QString& message) const;
public:
    explicit ClientApi(QObject *parent = nullptr);

    void setApiKey(const QString& apiKey);
    void setLocation(const Location& location);
    void setCacheDuration(int seconds);

    void fetchCurrentWeather();
    void fetchForecast();

    void updateCurrentWeather(const WeatherData& data);
    void updateForecast(const Forecast& forecast);

    WeatherData currentWeather() const;
    Forecast forecast() const;
    WeatherApiError lastError() const;
signals:
    void currentWeatherUpdated(const WeatherData& data);
    void forecastUpdated(const Forecast& forecast);
    void errorOccurred(WeatherApiError error);
};

#endif // CLIENTAPI_HPP
