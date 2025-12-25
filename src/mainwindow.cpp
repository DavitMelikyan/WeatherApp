#include "../include/mainwindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_api = new ClientApi(this);
    setupUI();
    setupConnections();
    m_api->setApiKey("28d2c7c31f364915a14184619252212");
    m_api->setLocation(Location("Yerevan", 40.1872, 44.5152));
    m_api->fetchCurrentWeather();
    m_api->fetchForecast();
}

void MainWindow::setupUI() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    temperature = new QLabel("°C");
    feelsLike = new QLabel("Feels like: °C");
    conditionIcon = new QLabel;
    condition = new QLabel("");
    humidity = new QLabel("Humidity: %");
    windSpeed = new QLabel("Wind: km/h");
    pressure = new QLabel("Pressure: mb");
    uv = new QLabel("UV:");
    visibility = new QLabel("Visibility: km");
    locationName = new QLabel("Yerevan");
    lastUpdated = new QLabel("Last update:");
    refreshBtn = new QPushButton("Refresh");
    locationInput = new QLineEdit(this);
    locationInput->setPlaceholderText("Enter city (e.g. Yerevan)");
    locationInput->setClearButtonEnabled(true);


    QFont tempFont;
    tempFont.setPointSize(48);
    tempFont.setBold(true);
    temperature->setStyleSheet("color: black");
    condition->setStyleSheet("font-size: 18px; color: black");
    temperature->setFont(tempFont);
    temperature->setAlignment(Qt::AlignCenter);
    condition->setAlignment(Qt::AlignCenter);
    conditionIcon->setAlignment(Qt::AlignCenter);

    feelsLike->setStyleSheet("color: black");
    humidity->setStyleSheet("color: black");
    windSpeed->setStyleSheet("color: black");
    pressure->setStyleSheet("color: black");
    uv->setStyleSheet("color: black");
    visibility->setStyleSheet("color: black");
    locationName ->setStyleSheet("color: black");
    lastUpdated->setStyleSheet("color: black");
    refreshBtn->setStyleSheet("color: black");

    QHBoxLayout* rootLayout = new QHBoxLayout(central);
    QWidget* currentWeatherPanel = new QWidget;
    QVBoxLayout* currentLayout = new QVBoxLayout(currentWeatherPanel);
    QHBoxLayout* hLayout = new QHBoxLayout(central);
    hLayout->addWidget(locationName);
    hLayout->addWidget(locationInput);
    hLayout->addStretch();
    hLayout->addWidget(lastUpdated);
    hLayout->addWidget(refreshBtn);

    QVBoxLayout* weatherLayout = new QVBoxLayout;
    weatherLayout->addWidget(temperature);
    weatherLayout->addWidget(conditionIcon);
    weatherLayout->addWidget(condition);
    weatherLayout->addWidget(feelsLike);
    QGridLayout* detailsLayout = new QGridLayout;
    detailsLayout->addWidget(humidity, 0, 0);
    detailsLayout->addWidget(windSpeed, 0, 1);
    detailsLayout->addWidget(pressure, 1, 0);
    detailsLayout->addWidget(uv, 1, 1);
    detailsLayout->addWidget(visibility, 2, 0);

    currentLayout->addLayout(hLayout);
    currentLayout->addSpacing(10);
    currentLayout->addLayout(weatherLayout);
    currentLayout->addSpacing(15);
    currentLayout->addLayout(detailsLayout);
    currentWeatherPanel->setMinimumWidth(400);

    QWidget* forecastPanel = new QWidget;
    QVBoxLayout* forecastLayout = new QVBoxLayout(forecastPanel);

    QLabel* forecastTitle = new QLabel("5-Day Forecast");
    forecastTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: black");
    forecastLayout->addWidget(forecastTitle);

    forecastList = new QListWidget;
    forecastList->setSpacing(6);
    forecastList->setSelectionMode(QAbstractItemView::SingleSelection);
    forecastList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    forecastLayout->addWidget(forecastList);

    rootLayout->addWidget(currentWeatherPanel, 3);
    rootLayout->addWidget(forecastPanel, 2);

    setMinimumSize(400, 600);
}

void MainWindow::setupConnections() {
    connect(m_api, &ClientApi::currentWeatherUpdated, this, &MainWindow::onCurrentWeatherUpdated);

    connect(m_api, &ClientApi::errorOccurred, this, &MainWindow::onErrorOccurred);

    connect(refreshBtn, &QPushButton::clicked, this, [this]() {
        m_api->fetchCurrentWeather();
        m_api->fetchForecast();
    });

    connect(locationInput, &QLineEdit::returnPressed, this, [this]() {
        const QString city = locationInput->text().trimmed();

        if (city.isEmpty()) return;

        city[0].toUpper();
        m_api->setLocation(Location(city));
        m_api->fetchCurrentWeather();
        m_api->fetchForecast();
    });

    connect(m_api, &ClientApi::forecastUpdated, this, &MainWindow::onForecastUpdated);

    connect(forecastList, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        int index = forecastList->row(item);
        const ForecastEntry& day = m_api->forecast().days().at(index);


        showForecastDetails(day);
    });
}

void MainWindow::onCurrentWeatherUpdated(const WeatherData& data) {
    temperature->setText(QString::number(data.m_temperature) + "°C");
    feelsLike->setText("Feels like: " + QString::number(data.m_feelsLike) + "°C");
    condition->setText(data.m_condition);

    humidity->setText("Humidity: " + QString::number(data.m_humidity) + "%");
    windSpeed->setText("Wind: " + QString::number(data.m_windSpeed) + " km/h");
    pressure->setText("Pressure: " + QString::number(data.m_pressure) + " mb");
    uv->setText("UV: " + QString::number(data.m_uvIndex));
    visibility->setText("Visibility: " + QString::number(data.m_visibility) + " km");
    locationName->setText(m_api->location());


    lastUpdated->setText("Last update: " + QDateTime::currentDateTime().toString("hh:mm:ss"));

    QString c = data.m_condition.toLower();

    if (c.contains("sun")) conditionIcon->setPixmap(QPixmap(":/icons/sun.png"));
    else if (c.contains("cloud") || c.contains("overcast") || c.contains("clear")) conditionIcon->setPixmap(QPixmap(":/icons/cloud.png"));
    else if (c.contains("rain") || c.contains("drizzle")) conditionIcon->setPixmap(QPixmap(":/icons/rain.png"));
    else if (c.contains("snow") || c.contains("blizzard")) conditionIcon->setPixmap(QPixmap(":/icons/snow.png"));
    else if (c.contains("thunder")) conditionIcon->setPixmap(QPixmap(":/icons/thunder.png"));
    else conditionIcon->setPixmap(QPixmap(":/icons/default.png"));

    updateColorScheme(data.m_condition);
}

void MainWindow::onForecastUpdated(const Forecast& forecast) {
    qDebug() << "Forecast days:" << forecast.days().size();
    forecastList->clear();
    for (const ForecastEntry& day : forecast.days()) {
        QListWidgetItem* item = new QListWidgetItem(forecastList);
        item->setSizeHint(QSize(0, 70));
        QWidget* row = new QWidget;
        QHBoxLayout* rowLayout = new QHBoxLayout(row);
        QLabel* dayLabel = new QLabel(day.m_date.toString("dddd"));
        QLabel* temps = new QLabel(QString("%1° / %2°").arg(day.m_maxTemp).arg(day.m_minTemp));
        QLabel* rainChance = new QLabel(QString("%1%").arg(day.m_precipitationChance));
        QLabel* icon = new QLabel;
        QString c = day.m_condition.toLower();
        QString iconPath = ":/icons/default.png";
        if (c.contains("sun")) iconPath = ":/icons/sun.png";
        else if (c.contains("cloud") || c.contains("overcast") || c.contains("clear")) iconPath = ":/icons/cloud.png";
        else if (c.contains("rain") || c.contains("drizzle")) iconPath = ":/icons/rain.png";
        else if (c.contains("snow") || c.contains("blizzard")) iconPath = ":/icons/snow.png";
        else if (c.contains("thunder")) iconPath = ":/icons/thunder.png";

        icon->setPixmap(QPixmap(iconPath).scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        if (day.m_precipitationChance < 30) rainChance->setStyleSheet("color: green;");
        else if (day.m_precipitationChance < 60) rainChance->setStyleSheet("color: orange;");
        else rainChance->setStyleSheet("color: red;");

        rowLayout->addWidget(dayLabel);
        rowLayout->addStretch();
        rowLayout->addWidget(icon);
        rowLayout->addWidget(temps);
        rowLayout->addWidget(rainChance);

        forecastList->setItemWidget(item, row);
    }
}

void MainWindow::onErrorOccurred(WeatherApiError error) {
    QString message;

    if (error == WeatherApiError::NetworkError) message = "Network error. Try again.";
    else if (error == WeatherApiError::InvalidResponse) message = "Invalid response from weather service.";
    else if (error == WeatherApiError::CityNotFound) {
        message = "Location not found.";
        m_api->setLocation(locationName->text());
    }
    else message = "Unknown error occurred.";

    QMessageBox::warning(this, "Weather Error", message);
}

void MainWindow::updateColorScheme(const QString& condition)
{
    const QString c = condition.toLower();

    if (c.contains("sun")) {
        setStyleSheet(
            "QMainWindow {"
            "background: qlineargradient("
            "x1:0, y1:0, x2:0, y2:1, "
            "stop:0 #fff7c2, "
            "stop:1 #ffd54f);"
            "}"
            );
    } else if (c.contains("cloud") || c.contains("overcast") || c.contains("clear")) {
        setStyleSheet(
            "QMainWindow {"
            "background: qlineargradient("
            "x1:0, y1:0, x2:0, y2:1, "
            "stop:0 #e0e0e0, "
            "stop:1 #bdbdbd);"
            "}"
            );
    } else if (c.contains("rain") || c.contains("drizzle")) {
        setStyleSheet(
            "QMainWindow {"
            "background: qlineargradient("
            "x1:0, y1:0, x2:0, y2:1, "
            "stop:0 #cce7ff, "
            "stop:1 #90caf9);"
            "}"
            );
    } else if (c.contains("thunder")) {
        setStyleSheet(
            "QMainWindow {"
            "background: qlineargradient("
            "x1:0, y1:0, x2:0, y2:1, "
            "stop:0 #616161, "
            "stop:1 #212121);"
            "}"
            );
    } else if (c.contains("snow") || c.contains("blizzard")) {
        setStyleSheet(
            "QMainWindow {"
            "background: qlineargradient("
            "x1:0, y1:0, x2:0, y2:1, "
            "stop:0 #ffffff, "
            "stop:1 #e3f2fd);"
            "}"
            );
    } else if (c.contains("fog") || c.contains("mist")) {
        setStyleSheet(
            "QMainWindow {"
            "background: qlineargradient("
            "x1:0, y1:0, x2:0, y2:1, "
            "stop:0 #eceff1, "
            "stop:1 #b0bec5);"
            "}"
            );
    } else {
        setStyleSheet(
            "QMainWindow {"
            "background: qlineargradient("
            "x1:0, y1:0, x2:0, y2:1, "
            "stop:0 #f5f5f5, "
            "stop:1 #eeeeee);"
            "}"
            );
    }
}

void MainWindow::showForecastDetails(const ForecastEntry& day) {
    QDialog dialog(this);
    dialog.setWindowTitle(day.m_date.toString("dddd"));
    dialog.setModal(true);
    dialog.setMinimumSize(300, 360);

    dialog.setStyleSheet(
        "QDialog {"
        "background: qlineargradient("
        "x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #e3f2fd, "
        "stop:1 #90caf9);"
        "border-radius: 10px;"
        "}"
        "QLabel { color: #0d47a1; }"
        "QPushButton {"
        "background-color: #1976d2;"
        "color: white;"
        "border: none;"
        "border-radius: 6px;"
        "padding: 6px 14px;"
        "}"
        "QPushButton:hover { background-color: #1565c0; }"
        );

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(10);

    QLabel* title = new QLabel(day.m_date.toString("dddd"));
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 18px; font-weight: bold;");

    QLabel* temps = new QLabel(
        QString("High: %1°C    Low: %2°C")
            .arg(day.m_maxTemp)
            .arg(day.m_minTemp)
        );
    temps->setAlignment(Qt::AlignCenter);

    QLabel* rain = new QLabel(
        QString("Rain chance: %1%").arg(day.m_precipitationChance)
        );
    rain->setAlignment(Qt::AlignCenter);

    QLabel* sun = new QLabel(
        QString("Sunrise: %1\nSunset: %2")
            .arg(day.m_sunrise.toString("hh:mm"))
            .arg(day.m_sunset.toString("hh:mm"))
        );
    sun->setAlignment(Qt::AlignCenter);

    QPushButton* closeBtn = new QPushButton("Close");
    closeBtn->setFixedHeight(34);

    layout->addWidget(title);
    layout->addSpacing(6);
    layout->addWidget(temps);
    layout->addWidget(rain);
    layout->addSpacing(8);
    layout->addWidget(sun);
    layout->addStretch();
    layout->addWidget(closeBtn);

    connect(closeBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
}

