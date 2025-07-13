#ifndef CITYMAPWINDOW_H
#define CITYMAPWINDOW_H

#include <QWidget>
#include "mapview.h"
#include "citymap.h"

class CityMapWindow : public QWidget {
    Q_OBJECT
public:
    explicit CityMapWindow(QWidget* parent = nullptr);
    ~CityMapWindow();

signals:
    void routeSelected(const QString& source, const QString& destination, double distance, const QList<int>& path);

private slots:
    void handleCitySelected(int cityIndex, bool isSource);

private:
    MapView* mapView;
    CityMap* cityMap;
    int sourceIndex;
    int destIndex;
    void setupCitiesAndRoads();
    void showRouteInfo();
};

#endif // CITYMAPWINDOW_H 