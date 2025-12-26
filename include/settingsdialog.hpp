#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

#include <QDialog>
#include <QSettings>
#include <QComboBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QGroupBox>


enum class TemperatureUnit { Celsius, Fahrenheit };

enum class WindSpeedUnit { KmH, Mph };

enum class RefreshInterval { Min15, Min30, Hour1, Manual };

enum class ThemeMode { Light, Dark, Dynamic };

enum class NotificationFrequency { Hourly, Daily, SevereOnly };

class SettingsDialog : public QDialog {
    Q_OBJECT
private:
    QComboBox* tempUnitBox;
    QComboBox* windUnitBox;
    QComboBox* refreshBox;
    QComboBox* notificationBox;
    QComboBox* themeBox;
    QComboBox* notificationFreqBox;
    QCheckBox* notificationsCheck;
    QSettings settings;

    void setupUI();
    void loadSettings();
public:
    SettingsDialog(QWidget* parent = nullptr);

signals:
    void temperatureUnitChanged(TemperatureUnit unit);
    void windSpeedUnitChanged(WindSpeedUnit speed);
    void refreshIntervalChanged(RefreshInterval interval);
    void themeModeChanged(ThemeMode theme);
    void notificationsEnabledChanged(bool enabled);
    void notificationFrequencyChanged(NotificationFrequency frequency);
};


#endif // SETTINGSDIALOG_HPP
