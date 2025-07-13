#include "cabbooking.h"
#include "ui_cabbooking.h"
#include "priorityqueue.h"
#include "profilemanager.h"
#include <QMessageBox>

#include <QMediaPlayer>
#include <QVideoWidget>
#include "mapview.h"
#include "citymap.h"
#include <QSet>
#include <QTimer>
#include <QSqlQuery>
#include <QSqlError>

void CabBooking::showThankYouMessage()
{
    if (scene) {
        QGraphicsTextItem* textItem = new QGraphicsTextItem("Thank you for riding with us!");
        textItem->setDefaultTextColor(Qt::white);
        textItem->setFont(QFont("Arial", 24, QFont::Bold));
        textItem->setPos(scene->width()/2 - textItem->boundingRect().width()/2,
                        scene->height()/2 - textItem->boundingRect().height()/2);
        scene->addItem(textItem);
    }
}

void CabBooking::showRatingPrompt()
{
    bool ok;
    int rating = QInputDialog::getInt(this, "Rate Your Ride",
                                    "Please rate your ride (1-5):", 5, 1, 5, 1, &ok);
    if (ok) {
        // Store the rating and show tip prompt
        driverID = rating; // Store rating temporarily
        showTipPrompt();
    }
}

void CabBooking::showTipPrompt()
{
    bool ok;
    double tip = QInputDialog::getDouble(this, "Driver Tip",
                                       "Enter tip amount:", 0.0, 0.0, 1000.0, 2, &ok);
    if (ok) {
        onTipSubmitted(tip);
    } else {
        onTipSubmitted(0.0); // No tip
    }
}

void CabBooking::onTipSubmitted(double tip)
{
    // Store the tip amount
    userTip = tip;
    
    // Update cab earnings in database
    updateCabEarnings(cabID, rideDistance, tip);

    // Update all relevant database entries
    updateDatabaseAfterRide();

    QMessageBox::information(this, "Ride Complete",
        "Thank you for your feedback!\nWe hope to serve you again soon.");
}

void CabBooking::updateDatabaseAfterRide()
{
    if (sqlitedb.open()) {
        QSqlQuery query(sqlitedb);

        // Update ride history
        query.prepare("INSERT INTO RideHistory (user_id, cab_id, distance, tip_amount, rating) "
                     "VALUES (:user_id, :cab_id, :distance, :tip, :rating)");
        query.bindValue(":user_id", currentUser->getUsername());
        query.bindValue(":cab_id", cabID);
        query.bindValue(":distance", rideDistance);
        query.bindValue(":tip", userTip);
        query.bindValue(":rating", driverID); // Use the stored rating
        query.exec();

        // Update cab statistics
        query.prepare("UPDATE CabStats SET total_rides = total_rides + 1, "
                     "total_earnings = total_earnings + :earnings "
                     "WHERE cab_id = :cab_id");
        query.bindValue(":earnings", rideDistance * 2.5 + userTip); // Base fare + tip
        query.bindValue(":cab_id", cabID);
        query.exec();

        sqlitedb.close();
    }
}

CabBooking::CabBooking(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CabBooking)
    , currentUser(nullptr)
    , cabCount(0)
    , cabID("")
    , rideDistance(0.0)
    , driverID(0)
    , userTip(0.0)
    , scene(nullptr)
    , carItem(nullptr)
    , carAnimation(nullptr)
    , player(nullptr)
    , audioOutput(nullptr)
    , mapView(nullptr)
    , cityMap(nullptr)
    , selectedSourceCity("")
    , selectedDestination("")
    , selectedDriverIndex(-1)
    , optimalDriverIndex(-1)
    , currentCabPlate("")
    , selectedDriverPath()
    , selectedDriverDistance(0.0)
    , selectedDriverFare(0)
{
    ui->setupUi(this);
    // Setup city map and map view
    cityMap = new CityMap();
    // Add cities with user-provided coordinates for best fit (spread out to reduce clutter)
    cityMap->addCity("Karachi", 120, 600);
    cityMap->addCity("Hyderabad", 200, 540);
    cityMap->addCity("Sukkur", 300, 470);
    cityMap->addCity("Quetta", 80, 400);
    cityMap->addCity("Multan", 420, 400);
    cityMap->addCity("Bahawalpur", 480, 440);
    cityMap->addCity("Lahore", 650, 260);
    cityMap->addCity("Faisalabad", 540, 300);
    cityMap->addCity("Sialkot", 720, 210);
    cityMap->addCity("Gujranwala", 670, 230);
    cityMap->addCity("Islamabad", 600, 150);
    cityMap->addCity("Rawalpindi", 570, 170);
    cityMap->addCity("Peshawar", 420, 100);
    cityMap->addCity("Abbottabad", 520, 120);
    cityMap->addCity("Murree", 610, 110);
    cityMap->addRoad("Islamabad", "Rawalpindi", 20);
    cityMap->addRoad("Islamabad", "Peshawar", 180);
    cityMap->addRoad("Islamabad", "Lahore", 375);
    cityMap->addRoad("Islamabad", "Murree", 60);
    cityMap->addRoad("Rawalpindi", "Abbottabad", 110);
    cityMap->addRoad("Lahore", "Faisalabad", 120);
    cityMap->addRoad("Lahore", "Sialkot", 130);
    cityMap->addRoad("Lahore", "Gujranwala", 80);
    cityMap->addRoad("Faisalabad", "Multan", 180);
    cityMap->addRoad("Multan", "Bahawalpur", 90);
    cityMap->addRoad("Multan", "Karachi", 900);
    cityMap->addRoad("Karachi", "Hyderabad", 150);
    cityMap->addRoad("Hyderabad", "Sukkur", 300);
    cityMap->addRoad("Sukkur", "Quetta", 400);
    cityMap->addRoad("Quetta", "Peshawar", 700);
    cityMap->addRoad("Bahawalpur", "Sukkur", 200);
    cityMap->addRoad("Abbottabad", "Peshawar", 150);
    // Setup map view
    mapView = new MapView(this);
    mapView->setCityMap(cityMap);
    mapView->setMapImage("C:/Users/Faaiz Khan/Downloads/University Folder/4th Sem Course Contents/Project Copy/Version 4 CBS/images/pk3.png");
    mapView->setGeometry(ui->videoWidget->geometry());
    mapView->show();
    ui->videoWidget->hide();
    // When booking, hide map and show animation
    connect(ui->btnBook, &QPushButton::clicked, this, [this]() {
        // Check if a driver is selected
        if (selectedDriverIndex < 0 || selectedDriverIndex >= currentDisplayedDrivers.size()) {
            QMessageBox::warning(this, "No Selection", "Please select a cab to book.");
            return;
        }
        
        QVector<Driver>& driverList = currentDisplayedDrivers;
        Driver& d = driverList[selectedDriverIndex];
        
        if (!d.available) {
            QMessageBox::information(this, "Cab Unavailable", "Sorry, this cab is already booked.");
            return;
        }
        
        // Check if we have a valid route
        if (selectedDriverPath.isEmpty()) {
            QMessageBox::warning(this, "No Route", "Please select source and destination cities first.");
            return;
        }
        
        // Book the selected cab
        d.available = false;
        
        // Store the selected driver info for later use
        cabID = d.numberPlate;
        rideDistance = selectedDriverDistance;
        
        // Start animation immediately
        mapView->hide();
        ui->videoWidget->show();
        playRideAnimation();
        
        QMessageBox::information(this, "Booking Confirmed", 
            QString("Cab booked successfully!\nDriver: %1\nRoute: %2 → %3\nDistance: %4 km\nFare: %5 PKR")
            .arg(d.name)
            .arg(selectedSourceCity)
            .arg(selectedDestination)
            .arg(selectedDriverDistance, 0, 'f', 1)
            .arg(selectedDriverFare));
    });
    loadCabsFromDatabase();
    displayCabsInTable();
    connect(ui->comboBoxSort, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBoxSort_currentIndexChanged(int)));
    connect(ui->btnViewProfile, &QPushButton::clicked, this, &CabBooking::on_btnViewProfile_clicked);
    connect(mapView, &MapView::citySelected, this, [this](int cityIndex, bool isSource) {
        static int sourceIndex = -1;
        static int destIndex = -1;
        static bool driversInitialized = false;
        const auto& cities = cityMap->getCities();
        if (!driversInitialized) {
            // Assign at least 5 drivers to each city
            drivers.clear();
            QStringList driverNames = {"Ali", "Ahmed", "Sara", "Usman", "Fatima", "Bilal", "Ayesha", "Zain", "Hina", "Omar", "Sana", "Imran", "Nida", "Kashif", "Rabia", "Danish", "Mariam", "Asad", "Iqra", "Fahad", "Hassan", "Noman", "Tariq", "Shazia", "Jawad", "Lubna", "Saad", "Adeel", "Naveed", "Shan"};
            int nameIdx = 0;
            for (const CityNode& city : cities) {
                for (int i = 0; i < 5; ++i) {
                    Driver d;
                    d.name = driverNames[nameIdx % driverNames.size()] + QString::number(i+1);
                    d.numberPlate = "ABC-" + QString::number(QRandomGenerator::global()->bounded(1000,9999));
                    d.rating = 3.0f + QRandomGenerator::global()->bounded(0, 20) / 4.0f; // 3.0 to 8.0
                    d.city = city.name;
                    d.available = true;
                    // Assign different pricing for each driver (40-80 PKR per 10km)
                    d.pricePer10km = 40.0 + QRandomGenerator::global()->bounded(0, 41); // 40-80 PKR per 10km
                    drivers.append(d);
                    ++nameIdx;
                }
            }
            driversInitialized = true;
        }
        if (isSource) {
            sourceIndex = cityIndex;
            selectedSourceCity = cities[cityIndex].name;
        } else {
            destIndex = cityIndex;
            selectedDestination = cities[cityIndex].name;
        }
        // Only update table if both source and destination are selected
        if (sourceIndex != -1 && destIndex != -1) {
            QString srcName = cities[sourceIndex].name;
            QString dstName = cities[destIndex].name;
            // Filter drivers at the source city
            currentDisplayedDrivers.clear();
            for (const Driver& d : drivers) {
                if (d.city == srcName && d.available)
                    currentDisplayedDrivers.append(d);
            }
            // For each driver, calculate route from source to destination
            QList<QList<int>> driverPaths;
            QList<double> driverDistances;
            QList<int> fares;
            QList<double> scores;
            int optimalDriverIdx = -1;
            double bestScore = -1e9;
            for (int i = 0; i < currentDisplayedDrivers.size(); ++i) {
                auto result = cityMap->dijkstra(srcName, dstName);
                double dist = result.first;
                QList<int> path = result.second;
                driverPaths.append(path);
                driverDistances.append(dist);
                // Use driver's individual pricing
                int fare = qCeil(dist / 10.0) * currentDisplayedDrivers[i].pricePer10km;
                fares.append(fare);
                double score = (currentDisplayedDrivers[i].rating * 10) - (fare * 0.2) - (dist * 0.5);
                scores.append(score);
                if (score > bestScore) {
                    bestScore = score;
                    optimalDriverIdx = i;
                }
            }
            // Sort by distance or rating if needed
            if (ui->comboBoxSort->currentIndex() == 0) { // Sort by distance
                QVector<int> indices(currentDisplayedDrivers.size());
                std::iota(indices.begin(), indices.end(), 0);
                std::sort(indices.begin(), indices.end(), [&](int a, int b) { return driverDistances[a] < driverDistances[b]; });
                QVector<Driver> sortedDrivers;
                QList<QList<int>> sortedPaths;
                QList<double> sortedDistances;
                QList<int> sortedFares;
                QList<double> sortedScores;
                for (int idx : indices) {
                    sortedDrivers.append(currentDisplayedDrivers[idx]);
                    sortedPaths.append(driverPaths[idx]);
                    sortedDistances.append(driverDistances[idx]);
                    sortedFares.append(fares[idx]);
                    sortedScores.append(scores[idx]);
                }
                currentDisplayedDrivers = sortedDrivers;
                driverPaths = sortedPaths;
                driverDistances = sortedDistances;
                fares = sortedFares;
                scores = sortedScores;
            } else if (ui->comboBoxSort->currentIndex() == 1) { // Sort by rating
                QVector<int> indices(currentDisplayedDrivers.size());
                std::iota(indices.begin(), indices.end(), 0);
                std::sort(indices.begin(), indices.end(), [&](int a, int b) { return currentDisplayedDrivers[a].rating > currentDisplayedDrivers[b].rating; });
                QVector<Driver> sortedDrivers;
                QList<QList<int>> sortedPaths;
                QList<double> sortedDistances;
                QList<int> sortedFares;
                QList<double> sortedScores;
                for (int idx : indices) {
                    sortedDrivers.append(currentDisplayedDrivers[idx]);
                    sortedPaths.append(driverPaths[idx]);
                    sortedDistances.append(driverDistances[idx]);
                    sortedFares.append(fares[idx]);
                    sortedScores.append(scores[idx]);
                }
                currentDisplayedDrivers = sortedDrivers;
                driverPaths = sortedPaths;
                driverDistances = sortedDistances;
                fares = sortedFares;
                scores = sortedScores;
            }
            // Update table with path and fare
            ui->tableCabs->setRowCount(currentDisplayedDrivers.size());
            ui->tableCabs->setColumnCount(7);
            QStringList headers = {"Driver", "Number Plate", "Rating", "Available", "Fare", "Path", "Price/10km"};
            ui->tableCabs->setHorizontalHeaderLabels(headers);
            for (int i = 0; i < currentDisplayedDrivers.size(); ++i) {
                const Driver& d = currentDisplayedDrivers[i];
                ui->tableCabs->setItem(i, 0, new QTableWidgetItem(d.name));
                ui->tableCabs->setItem(i, 1, new QTableWidgetItem(d.numberPlate));
                ui->tableCabs->setItem(i, 2, new QTableWidgetItem(QString::number(d.rating, 'f', 1)));
                ui->tableCabs->setItem(i, 3, new QTableWidgetItem(d.available ? "Yes" : "No"));
                ui->tableCabs->setItem(i, 4, new QTableWidgetItem(QString::number(fares[i])));
                QString pathStr;
                for (int idx : driverPaths[i]) pathStr += cities[idx].name + "->";
                if (!pathStr.isEmpty()) pathStr.chop(2);
                ui->tableCabs->setItem(i, 5, new QTableWidgetItem(pathStr));
                ui->tableCabs->setItem(i, 6, new QTableWidgetItem(QString::number(d.pricePer10km) + " PKR"));
            }
            // Highlight optimal driver row
            if (optimalDriverIdx != -1) {
                for (int col = 0; col < 7; ++col) {
                    ui->tableCabs->item(optimalDriverIdx, col)->setBackground(Qt::yellow);
                }
                // Highlight optimal path on map
                mapView->highlightPath(driverPaths[optimalDriverIdx]);
            }
        }
    });
    // Make the table empty on startup
    ui->tableCabs->setRowCount(0);
    ui->tableCabs->setColumnCount(7);
    QStringList headers = {"Driver", "Number Plate", "Rating", "Available", "Fare", "Path", "Price/10km"};
    ui->tableCabs->setHorizontalHeaderLabels(headers);
    connect(ui->tableCabs, &QTableWidget::cellClicked, this, &CabBooking::on_tableCabs_cellClicked);
    connect(ui->btnAISmartBook, &QPushButton::clicked, this, &CabBooking::on_btnAISmartBook_clicked);
}

CabBooking::~CabBooking()
{
    delete ui;
}

void CabBooking::displayCabsInTable()
{
    ui->tableCabs->setRowCount(cabCount);
    ui->tableCabs->setColumnCount(5);
    QStringList headers = {"Driver", "Number Plate", "Distance", "Rating", "Available"};
    ui->tableCabs->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < cabCount; i++) {
        Cab c = cabList[i];
        ui->tableCabs->setItem(i, 0, new QTableWidgetItem(c.getDriverName()));
        ui->tableCabs->setItem(i, 1, new QTableWidgetItem(c.getNumberPlate()));
        ui->tableCabs->setItem(i, 2, new QTableWidgetItem(QString::number(c.getDistance()) + " km"));
        ui->tableCabs->setItem(i, 3, new QTableWidgetItem(QString::number(c.getRating())));
        ui->tableCabs->setItem(i, 4, new QTableWidgetItem(c.isAvailable() ? "Yes" : "No"));
    }
}

void CabBooking::bookCab()
{
    int row = ui->tableCabs->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a cab to book.");
        return;
    }
    
    QVector<Driver>& driverList = currentDisplayedDrivers; // Always use currentDisplayedDrivers
    
    if (row >= driverList.size()) {
        QMessageBox::warning(this, "Invalid Selection", "Please select a valid cab to book.");
        return;
    }
    
    selectedDriverIndex = row;
    Driver& d = driverList[row];
    
    if (!d.available) {
        QMessageBox::information(this, "Cab Unavailable", "Sorry, this cab is already booked.");
        return;
    }
    
    d.available = false;
    
    // Use the stored path and fare if this driver was selected from the table
    if (selectedDriverIndex == row && !selectedDriverPath.isEmpty()) {
        // Use the pre-calculated path and fare for this selected driver
        mapView->highlightPath(selectedDriverPath);
        rideDistance = selectedDriverDistance;
        cabID = d.numberPlate;
    } else {
        // Calculate route and fare for this driver
        QString destination = selectedDestination;
        
        // Use the new route calculation function
        QList<int> route = getDriverRoute(d, selectedSourceCity, destination);
        
        // Calculate distance for this route
        double totalDistance = 0.0;
        for (int i = 0; i < route.size() - 1; ++i) {
            const auto& cities = cityMap->getCities();
            QString city1 = cities[route[i]].name;
            QString city2 = cities[route[i + 1]].name;
            // Get distance between these cities
            auto result = cityMap->dijkstra(city1, city2);
            totalDistance += result.first;
        }
        
        mapView->highlightPath(route);
        rideDistance = totalDistance;
        cabID = d.numberPlate;
        // Store the fare for this driver using their individual pricing
        selectedDriverFare = qCeil(rideDistance / 10.0) * d.pricePer10km;
    }
    
    // Start animation immediately
    mapView->hide();
    ui->videoWidget->show();
    playRideAnimation();
    
    // The animation will handle showing fare summary and rating prompt
}

void CabBooking::playRideAnimation()
{
    // Create a new media player for this animation
    QMediaPlayer *player = new QMediaPlayer(this);
    player->setVideoOutput(ui->videoWidget);
    player->setSource(QUrl::fromLocalFile("C:/Users/Faaiz Khan/Downloads/University Folder/4th Sem Course Contents/Project Copy/Version 4 CBS/images/bookride.mp4"));
    player->setLoops(1);
    
    // Connect the media status changed signal
    connect(player, &QMediaPlayer::mediaStatusChanged, this, [this, player](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            // Animation finished, show fare summary and prompt for rating
            int fare;
            if (selectedDriverFare > 0) {
                // Use the stored fare for the selected driver
                fare = selectedDriverFare;
            } else {
                // Calculate fare based on distance (fallback)
                fare = qCeil(rideDistance / 10.0) * 50; // Default pricing
            }
            showFareSummary(fare, rideDistance);
            showRatingPrompt();
            player->deleteLater();
            
            // Show the map again after animation ends for both AI and manual booking
            ui->videoWidget->hide();
            mapView->show();
            mapView->clearHighlights();
            
            // Clear selected driver info to allow fresh booking
            clearSelectedDriverInfo();
        }
    });
    
    // Start playing the animation
    player->play();
}

void CabBooking::on_comboBoxSort_currentIndexChanged(int index)
{
    // Re-trigger the table update for in-drive drivers
    if (!selectedSourceCity.isEmpty()) {
        int srcIdx = -1;
        for (int i = 0; i < cityMap->getCities().size(); ++i) {
            if (cityMap->getCities()[i].name == selectedSourceCity) {
                srcIdx = i;
                break;
            }
        }
        if (srcIdx != -1) emit mapView->citySelected(srcIdx, true);
    }
}

void CabBooking::showBookingConfirmation()
{
    QMessageBox::information(this, "Booking Confirmed",
        "We look forward to riding with you!\nYour ride will begin shortly.");
}

// Update rating in the database
void CabBooking::updateCabRating(const QString &plate, float newRating)
{
    QSqlQuery query(sqlitedb);

    query.prepare("UPDATE Cabs SET rating = ((rating * num_rides + :newRating) / (num_rides + 1)), num_rides = num_rides + 1 WHERE numberPlate = :plate");
    query.bindValue(":newRating", newRating);
    query.bindValue(":plate", plate);

    if (!query.exec()) {
        qDebug() << "Failed to update rating: " << query.lastError().text();
    } else {
        qDebug() << "Rating updated successfully!";
    }
}

// Load cabs from SQLite database
void CabBooking::loadCabsFromDatabase()
{
    if (!QSqlDatabase::contains("qt_sql_default_connection")) {
        sqlitedb = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
        sqlitedb.setDatabaseName("C:/Users/Faaiz Khan/Downloads/University Folder/4th Sem Course Contents/Project Copy/Version 4 CBS/login.db");
    } else {
        sqlitedb = QSqlDatabase::database("qt_sql_default_connection");
    }

    qDebug() << "Attempting to open database...";
    if (!sqlitedb.open()) {
        qDebug() << "Database connection failed: " << sqlitedb.lastError().text();
        return;
    }

    QSqlQuery query(sqlitedb);
    query.prepare("SELECT numberPlate, driverName, rating, distance, available FROM Cabs");

    qDebug() << "Executing query...";
    if (!query.exec()) {
        qDebug() << "Query execution failed: " << query.lastError().text();
        return;
    }

    cabCount = 0;
    while (query.next() && cabCount < MAX_CABS) {
        QString plate = query.value(0).toString();
        QString driver = query.value(1).toString();
        float rate = query.value(2).toFloat();
        float dist = query.value(3).toFloat();
        bool avail = query.value(4).toInt();

        cabList[cabCount++] = Cab(plate, driver, rate, dist, avail);
    }

    // Ensure table updates with new data!
    displayCabsInTable();
}

void CabBooking::updateCabEarnings(const QString &cabID, double distanceKm, double tipAmount)
{
    double earnings = (distanceKm * 10) + tipAmount; // ✅ Rs 10/km + tip

    QSqlQuery query;
    query.prepare("UPDATE cabs SET earnings = earnings + :earnings WHERE numberPlate = :cabID");
    query.bindValue(":earnings", earnings);
    query.bindValue(":cabID", cabID);  // ✅ Make sure `cabID` holds the numberPlate

    if (query.exec()) {
        qDebug() << "Earnings updated: " << earnings << " Rs for cab ID " << cabID;
    } else {
        qDebug() << "Error updating earnings: " << query.lastError().text();
    }
}

double CabBooking::getUserTip()
{
    bool ok;
    double tipAmount = QInputDialog::getDouble(this, "Tip Your Driver",
                                               "Enter Tip Amount (Rs):",
                                               0, 0, 500, 2, &ok);
    return ok ? tipAmount : 0;
}

int CabBooking::showRatingsPrompt()
{
    bool ok;
    int rating = QInputDialog::getInt(this, "Rate Your Driver",
                                      "Enter Rating (1-10):",
                                      1, 1, 10, 1, &ok);
    return ok ? rating : -1;
}

void CabBooking::updateDriverRating(int driverID, int rating)
{
    if (rating == -1) return;

    QSqlQuery query;
    query.prepare("UPDATE cabs SET rating = ((rating * distance) + :rating) / (distance + 1) WHERE numberPlate = :cabID");
    query.bindValue(":rating", rating);
    query.bindValue(":cabID", cabID);  // ✅ Ensure `cabID` stores the correct cab identifier

    if (query.exec()) {
        qDebug() << "Driver rating updated!";
    } else {
        qDebug() << "Error updating rating: " << query.lastError().text();
    }
}

void CabBooking::on_btnViewProfile_clicked()
{
    if (!currentUser) {
        QMessageBox::warning(this, "Profile", "No user is currently logged in.");
        return;
    }
    ProfileManager profileDialog(this);
    profileDialog.setUser(*currentUser);
    profileDialog.setModal(true);
    profileDialog.exec();
}

void CabBooking::on_tableCabs_cellClicked(int row, int column) {
    QVector<Driver>& driverList = currentDisplayedDrivers;
    selectedDriverIndex = row;
    if (row < 0 || row >= driverList.size()) return;
    
    const Driver& d = driverList[row];
    
    // Highlight the selected row
    ui->tableCabs->selectRow(row);
    
    // Use Dijkstra for the selected cab's route
    if (!selectedSourceCity.isEmpty() && !selectedSourceCity.isNull()) {
        QString destination = selectedDestination;
        
        // Use the new route calculation function
        QList<int> route = getDriverRoute(d, selectedSourceCity, destination);
        
        // Calculate distance for this route
        double totalDistance = 0.0;
        for (int i = 0; i < route.size() - 1; ++i) {
            const auto& cities = cityMap->getCities();
            QString city1 = cities[route[i]].name;
            QString city2 = cities[route[i + 1]].name;
            // Get distance between these cities
            auto result = cityMap->dijkstra(city1, city2);
            totalDistance += result.first;
        }
        
        // Store the path and distance for this selected driver
        selectedDriverPath = route;
        selectedDriverDistance = totalDistance;
        selectedDriverFare = qCeil(selectedDriverDistance / 10.0) * d.pricePer10km;
        
        // Highlight the path on the map
        mapView->highlightPath(selectedDriverPath);
        
        // Show fare preview in the label
        showFareSummary(selectedDriverFare, selectedDriverDistance);
        
        // Show a brief message about the selected driver
        QMessageBox::information(this, "Driver Selected", 
            QString("Selected: %1\nRoute: %2 → %3\nDistance: %4 km\nFare: %5 PKR\nPrice: %6 PKR/10km")
            .arg(d.name)
            .arg(selectedSourceCity)
            .arg(destination)
            .arg(selectedDriverDistance, 0, 'f', 1)
            .arg(selectedDriverFare)
            .arg(d.pricePer10km));
    }
}

void CabBooking::on_btnAISmartBook_clicked() {
    QVector<Driver>& driverList = currentDisplayedDrivers;
    // Only consider available drivers
    QVector<int> candidateIndices;
    for (int i = 0; i < driverList.size(); ++i) {
        if (driverList[i].available)
            candidateIndices.append(i);
    }
    
    if (candidateIndices.isEmpty()) {
        QMessageBox::warning(this, "No Available Cabs", "No available cabs found for booking.");
        return;
    }
    
    double bestScore = -1e9;
    int bestIdx = -1;
    QList<int> bestPath;
    double bestDistance = 0;
    int bestFare = 0;
    
    for (int idx : candidateIndices) {
        const Driver& d = driverList[idx];
        QString dest = selectedDestination;
        auto result = cityMap->dijkstra(selectedSourceCity, dest);
        double dist = result.first;
        // Use driver's individual pricing
        int fare = qCeil(dist / 10.0) * d.pricePer10km;
        double score = (d.rating * 10) - (fare * 0.2) - (dist * 0.5);
        if (score > bestScore) {
            bestScore = score;
            bestIdx = idx;
            bestPath = result.second;
            bestDistance = dist;
            bestFare = fare;
        }
    }
    
    if (bestIdx != -1) {
        optimalDriverIndex = bestIdx;
        selectedDriverIndex = bestIdx;
        ui->tableCabs->selectRow(bestIdx);
        mapView->highlightPath(bestPath); // Always highlight the best path
        
        // Clear any previously selected driver info since AI is booking a different driver
        clearSelectedDriverInfo();
        
        // Automatically book the best cab and start animation
        Driver& d = driverList[bestIdx];
        d.available = false;
        
        // Store the selected driver info for later use
        cabID = d.numberPlate;
        rideDistance = bestDistance;
        
        // Start animation immediately
        mapView->hide();
        ui->videoWidget->show();
        playRideAnimation();
        
        // The animation will handle showing fare summary and rating prompt
        
        QMessageBox::information(this, "AI Smart Booking", "Best cab selected and booked! Your ride is starting now.");
    }
}

void CabBooking::showFareSummary(double fare, double distance) {
    ui->labelFareSummary->setText(QString("Total Fare: %1 PKR | Distance: %2 km").arg(fare).arg(distance, 0, 'f', 1));
}

void CabBooking::clearSelectedDriverInfo()
{
    selectedDriverPath.clear();
    selectedDriverDistance = 0.0;
    selectedDriverFare = 0;
    selectedDriverIndex = -1;
    mapView->clearHighlights();
}

QList<int> CabBooking::getDriverRoute(const Driver& driver, const QString& source, const QString& destination)
{
    // AI Smart Booking always uses shortest path (Dijkstra)
    if (optimalDriverIndex != -1) {
        auto result = cityMap->dijkstra(source, destination);
        return result.second;
    }
    
    // Individual in-drive drivers can take different routes
    // 70% chance to use shortest path, 30% chance to use alternative route
    if (QRandomGenerator::global()->bounded(100) < 70) {
        // Use shortest path (Dijkstra)
        auto result = cityMap->dijkstra(source, destination);
        return result.second;
    } else {
        // Use alternative route (longer path)
        return getAlternativeRoute(source, destination);
    }
}

QList<int> CabBooking::getAlternativeRoute(const QString& source, const QString& destination)
{
    // Get all cities
    const auto& cities = cityMap->getCities();
    
    // Find source and destination indices
    int srcIdx = -1, dstIdx = -1;
    for (int i = 0; i < cities.size(); ++i) {
        if (cities[i].name == source) srcIdx = i;
        if (cities[i].name == destination) dstIdx = i;
    }
    
    if (srcIdx == -1 || dstIdx == -1) {
        // Fallback to shortest path
        auto result = cityMap->dijkstra(source, destination);
        return result.second;
    }
    
    // Try to find a route through a different city
    for (int i = 0; i < cities.size(); ++i) {
        if (i != srcIdx && i != dstIdx) {
            // Check if we can go source -> intermediate -> destination
            auto path1 = cityMap->dijkstra(source, cities[i].name);
            auto path2 = cityMap->dijkstra(cities[i].name, destination);
            
            if (!path1.second.isEmpty() && !path2.second.isEmpty()) {
                // Combine the paths (remove duplicate intermediate city)
                QList<int> combinedPath = path1.second;
                if (!combinedPath.isEmpty()) combinedPath.removeLast(); // Remove intermediate city from first path
                combinedPath.append(path2.second);
                return combinedPath;
            }
        }
    }
    
    // Fallback to shortest path
    auto result = cityMap->dijkstra(source, destination);
    return result.second;
}
