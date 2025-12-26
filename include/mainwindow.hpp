#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QListWidget>
#include <QStringListModel>
#include <QCompleter>
#include <QSettings>
#include "clientapi.hpp"
#include "weatherdata.hpp"
#include "settingsdialog.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    ClientApi* m_api;
    QLabel* temperature;
    QLabel* feelsLike;
    QLabel* conditionIcon;
    QLabel* condition;
    QLabel* humidity;
    QLabel* windSpeed;
    QLabel* pressure;
    QLabel* uv;
    QLabel* visibility;
    QLineEdit* locationInput;

    QLabel* locationName;
    QLabel* lastUpdated;
    QPushButton* refreshBtn;
    QPushButton* settingsBtn;

    QListWidget* forecastList;

    QCompleter* completer;
    QStringListModel* completerModel;

    QListWidget* savedLocationsList;
    QPushButton* addLocationBtn;
    QPushButton* removeLocationBtn;
    QStringList savedLocations;

    void setupUI();
    void setupConnections();
    void updateColorScheme(const QString& condition);
    void showForecastDetails(const ForecastEntry& day);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;
private slots:
    void onCurrentWeatherUpdated(const WeatherData& data);
    void onForecastUpdated(const Forecast& forecast);
    void onErrorOccurred(WeatherApiError error);

    void applyTemperatureUnit(TemperatureUnit unit);
    void applyWindSpeedUnit(WindSpeedUnit speed);
    void applyRefreshInterval(RefreshInterval interval);
    void applyThemeMode(ThemeMode theme);
    void applyNotificationsEnabled(bool enabled);
    void applyNotificationFrequency(NotificationFrequency frequency);
};
#endif // MAINWINDOW_HPP
