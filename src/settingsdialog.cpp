#include "../include/settingsdialog.hpp"

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Settings");
    setModal(true);
    resize(380, 360);

    setupUI();
    loadSettings();
}

void SettingsDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QGroupBox* unitsGroup = new QGroupBox("Units");
    QVBoxLayout* unitsLayout = new QVBoxLayout(unitsGroup);
    tempUnitBox = new QComboBox;
    tempUnitBox->addItems({"Celsius", "Fahrenheit"});
    windUnitBox = new QComboBox;
    windUnitBox->addItems({"km/h", "mp/h"});
    unitsLayout->addWidget(tempUnitBox);
    unitsLayout->addWidget(windUnitBox);

    QGroupBox* refreshGroup = new QGroupBox("Auto refresh");
    QVBoxLayout* refreshLayout = new QVBoxLayout(refreshGroup);
    refreshBox = new QComboBox;
    refreshBox->addItems({"Every 15 minutes", "Every 30 minutes", "Every hour", "Manual"});
    refreshLayout->addWidget(refreshBox);

    QGroupBox* themeGroup = new QGroupBox("Appearance");
    QVBoxLayout* themeLayout = new QVBoxLayout(themeGroup);
    themeBox = new QComboBox;
    themeBox->addItems({"Light", "Dark", "Dynamic (Weather based)"});
    themeLayout->addWidget(themeBox);


    QGroupBox* notifGroup = new QGroupBox("Notifications");
    QVBoxLayout* notifLayout = new QVBoxLayout(notifGroup);
    notificationsCheck = new QCheckBox("Enable notifications");
    notificationFreqBox = new QComboBox;
    notificationFreqBox->addItems({"Hourly", "Daily", "Only severe weather"});
    notifLayout->addWidget(notificationsCheck);
    notifLayout->addWidget(notificationFreqBox);

    mainLayout->addWidget(unitsGroup);
    mainLayout->addWidget(refreshGroup);
    mainLayout->addWidget(themeGroup);
    mainLayout->addWidget(notifGroup);
    mainLayout->addStretch();

    connect(tempUnitBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        settings.setValue("tempUnit", index);
        emit temperatureUnitChanged(static_cast<TemperatureUnit>(index));
    });

    connect(windUnitBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        settings.setValue("windUnit", index);
        emit windSpeedUnitChanged(static_cast<WindSpeedUnit>(index));
    });

    connect(refreshBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        settings.setValue("refreshInterval", index);
        emit refreshIntervalChanged(static_cast<RefreshInterval>(index));
    });

    connect(themeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        settings.setValue("themeMode", index);
        emit themeModeChanged(static_cast<ThemeMode>(index));
    });

    connect(notificationsCheck, &QCheckBox::toggled, this, [this](bool enabled) {
        settings.setValue("notificationsEnabled", enabled);
        emit notificationsEnabledChanged(enabled);
    });

    connect(notificationFreqBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        settings.setValue("notificationFrequency", index);
        emit notificationFrequencyChanged(static_cast<NotificationFrequency>(index));
    });
}

void SettingsDialog::loadSettings() {
    tempUnitBox->setCurrentIndex(settings.value("tempUnit", 0).toInt());
    windUnitBox->setCurrentIndex(settings.value("windUnit", 0).toInt());
    refreshBox->setCurrentIndex(settings.value("refreshInterval", 0).toInt());
    themeBox->setCurrentIndex(settings.value("themeMode", 2).toInt());

    notificationsCheck->setChecked(settings.value("notificationsEnabled", false).toBool());

    notificationFreqBox->setCurrentIndex(settings.value("notificationFrequency", 0).toInt());

    notificationFreqBox->setEnabled(notificationsCheck->isChecked());
}
