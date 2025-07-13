#include "citymapwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QtMath>
#include <QLabel>

CityMapWindow::CityMapWindow(QWidget* parent)
    : QWidget(parent), mapView(new MapView(this)), cityMap(new CityMap), sourceIndex(-1), destIndex(-1)
{
    setMinimumSize(800, 600); // Ensure window is visible and large
    resize(1000, 700);        // Match main window size if needed
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(mapView);
    setLayout(layout);
    setupCitiesAndRoads();
    mapView->setCityMap(cityMap);
    mapView->setMapImage("images/grid2.png"); // Change filename if needed
    connect(mapView, &MapView::citySelected, this, &CityMapWindow::handleCitySelected);
}

CityMapWindow::~CityMapWindow() {
    delete cityMap;
}

void CityMapWindow::setupCitiesAndRoads() {
    // Add cities with coordinates (adjust as needed for your map)
    cityMap->addCity("Islamabad", 300, 100);
    cityMap->addCity("Lahore", 500, 200);
    cityMap->addCity("Attock", 250, 120);
    cityMap->addCity("Murree", 320, 60);
    cityMap->addCity("Peshawar", 150, 80);
    cityMap->addCity("Multan", 480, 400);
    cityMap->addCity("Quetta", 100, 500);
    cityMap->addCity("Karachi", 600, 700);
    // Add roads (distances in km, example values)
    cityMap->addRoad("Islamabad", "Attock", 80);
    cityMap->addRoad("Islamabad", "Murree", 60);
    cityMap->addRoad("Islamabad", "Lahore", 375);
    cityMap->addRoad("Islamabad", "Peshawar", 180);
    cityMap->addRoad("Lahore", "Multan", 340);
    cityMap->addRoad("Multan", "Quetta", 600);
    cityMap->addRoad("Quetta", "Karachi", 700);
    cityMap->addRoad("Lahore", "Karachi", 1200);
    cityMap->addRoad("Peshawar", "Attock", 90);
    cityMap->addRoad("Attock", "Murree", 100);
    cityMap->addRoad("Multan", "Karachi", 900);
}

void CityMapWindow::handleCitySelected(int cityIndex, bool isSource) {
    if (isSource) {
        sourceIndex = cityIndex;
        destIndex = -1;
    } else {
        destIndex = cityIndex;
    }
    if (sourceIndex != -1 && destIndex != -1) {
        // Both source and destination selected
        const auto& cities = cityMap->getCities();
        QString srcName = cities[sourceIndex].name;
        QString dstName = cities[destIndex].name;
        auto result = cityMap->dijkstra(srcName, dstName);
        double totalDistance = result.first;
        QList<int> path = result.second;
        mapView->highlightPath(path);
        showRouteInfo();
        emit routeSelected(srcName, dstName, totalDistance, path);
    }
}

void CityMapWindow::showRouteInfo() {
    if (sourceIndex == -1 || destIndex == -1) return;
    const auto& cities = cityMap->getCities();
    QString srcName = cities[sourceIndex].name;
    QString dstName = cities[destIndex].name;
    auto result = cityMap->dijkstra(srcName, dstName);
    double totalDistance = result.first;
    QList<int> path = result.second;
    int fare = qCeil(totalDistance / 4.0) * 50;
    QString routeStr;
    for (int idx : path) {
        routeStr += cities[idx].name + " -> ";
    }
    if (!routeStr.isEmpty())
        routeStr.chop(4); // Remove last arrow
    QMessageBox::information(this, "Route Info",
        QString("Shortest route: %1\nTotal distance: %2 km\nFare: %3 PKR")
            .arg(routeStr)
            .arg(totalDistance, 0, 'f', 1)
            .arg(fare));
} 