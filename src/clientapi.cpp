#include "../include/clientapi.hpp"

ClientApi::ClientApi(QObject *parent) : QObject(parent), m_baseUrl("https://api.weatherapi.com/v1"), m_cacheDuration(600), m_lastError(WeatherApiError::None)
{ }


void ClientApi::setApiKey(const QString& apiKey) {
    m_apiKey = apiKey;
}

void ClientApi::setLocation(const Location& location) {
    m_location = location;
    m_lastFetchTime = QDateTime();
}

void ClientApi::setCacheDuration(int seconds) {
    m_cacheDuration = seconds;
}

WeatherData ClientApi::currentWeather() const {
    return m_currentWeather;
}

Forecast ClientApi::forecast() const {
    return m_forecast;
}

WeatherApiError ClientApi::lastError() const {
    return m_lastError;
}

void ClientApi::fetchCurrentWeather() {
    if (isCacheValid()) {
        emit currentWeatherUpdated(m_currentWeather);
        return;
    }
    QNetworkRequest request = createRequest("/current.json");
    QNetworkReply* reply = m_networkManager.get(request);

    connect(reply, &QNetworkReply::finished, this, [this,reply](){
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            m_lastError = WeatherApiError::NetworkError;
            emit errorOccurred(m_lastError);
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        if (!doc.isObject()) {
            m_lastError = WeatherApiError::InvalidResponse;
            emit errorOccurred(m_lastError);
            return;
        }

        m_currentWeather = parseCurrentWeatherJson(doc.object());
        m_lastFetchTime = QDateTime::currentDateTimeUtc();
        m_lastError = WeatherApiError::None;
        emit currentWeatherUpdated(m_currentWeather);
    });
}

void ClientApi::fetchForecast() {
    QNetworkRequest request = createRequest("/forecast.json");
    QNetworkReply* reply = m_networkManager.get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            m_lastError = WeatherApiError::NetworkError;
            emit errorOccurred(m_lastError);
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        if (!doc.isObject()) {
            m_lastError = WeatherApiError::InvalidResponse;
            emit errorOccurred(m_lastError);
            return;
        }

        m_forecast = parseForecastJson(doc.object());
        m_lastError = WeatherApiError::None;
        emit forecastUpdated(m_forecast);
    });
}

bool ClientApi::isCacheValid() const {
    if (!m_lastFetchTime.isValid()) return false;
    return m_lastFetchTime.secsTo(QDateTime::currentDateTimeUtc()) < m_cacheDuration;
}

QNetworkRequest ClientApi::createRequest(const QString& endpoint) const {
    QUrl url(m_baseUrl + endpoint);
    QUrlQuery query;
    query.addQueryItem("key",  m_apiKey);
    if (m_location.hasCoordinates()) {
        query.addQueryItem(
            "q",
            QString::number(m_location.latitude()) + "," +
                QString::number(m_location.longitude())
            );
    } else {
        query.addQueryItem("q", m_location.city());
    }

    if (endpoint.contains("forecast")) {
        query.addQueryItem("days", "5");
    }
    url.setQuery(query);

    logDebug("Request URL: " + url.toString());
    return QNetworkRequest(url);
}

void ClientApi::logDebug(const QString& message) const {
    qDebug() << message;
}

WeatherData ClientApi::parseCurrentWeatherJson(const QJsonObject& json) {
    WeatherData data;

    QJsonObject current = json.value("current").toObject();
    QJsonObject condition = current.value("condition").toObject();

    data.m_condition = condition.value("text").toString();
    data.m_temperature = static_cast<int>(current.value("temp_c").toDouble());
    data.m_feelsLike = static_cast<int>(current.value("feelslike_c").toDouble());
    data.m_windSpeed = static_cast<int>(current.value("wind_kph").toDouble());
    data.m_pressure = static_cast<int>(current.value("pressure_mb").toDouble());
    data.m_humidity = static_cast<int>(current.value("humidity").toDouble());
    data.m_uvIndex = static_cast<int>(current.value("uv").toDouble());
    data.m_visibility = static_cast<int>(current.value("vis_km").toDouble());
    QJsonObject astro = current.value("astro").toObject();
    QString sunriseStr = astro.value("sunrise").toString();
    QString sunsetStr = astro.value("sunset").toString();
    data.m_sunrise = QDateTime::fromString(sunriseStr, "hh:mm AP");
    data.m_sunset = QDateTime::fromString(sunsetStr, "hh:mm AP");
    return data;
}

Forecast ClientApi::parseForecastJson(const QJsonObject& json) {
    Forecast forecast;
    QJsonArray days = json.value("forecast").toObject().value("forecastday").toArray();
    for (const QJsonValue& value : days) {
        QJsonObject dayObj = value.toObject();
        QJsonObject day = dayObj.value("day").toObject();
        QJsonObject condition = day.value("condition").toObject();
        ForecastEntry entry;
        entry.m_date = QDateTime::fromString(dayObj.value("date").toString(), Qt::ISODate);
        entry.m_data.m_temperature = static_cast<int>(day.value("avgtemp_c").toDouble());
        entry.m_data.m_condition = condition.value("text").toString();
        entry.m_data.m_humidity = static_cast<int>(day.value("avghumidity").toDouble());
        entry.m_data.m_uvIndex = static_cast<int>(day.value("uv").toDouble());
        forecast.addEntry(entry);
        QJsonObject astro = dayObj.value("astro").toObject();
        entry.m_data.m_sunrise = QDateTime::fromString(astro.value("sunrise").toString(), "hh:mm AP");
        entry.m_data.m_sunset  = QDateTime::fromString(astro.value("sunset").toString(), "hh:mm AP");

    }

    return forecast;
}
