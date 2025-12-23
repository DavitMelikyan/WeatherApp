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
#include "clientapi.hpp"
#include "weatherdata.hpp"

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
    QLabel* sunrise;
    QLabel* sunset;
    QLineEdit* locationInput;

    QLabel* locationName;
    QLabel* lastUpdated;
    QPushButton* refreshBtn;

    void setupUI();
    void setupConnections();
    void updateColorScheme(const QString& condition);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;
private slots:
    void onCurrentWeatherUpdated(const WeatherData& data);
    void onErrorOccurred(WeatherApiError error);
};
#endif // MAINWINDOW_HPP
