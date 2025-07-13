#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QMap>
#include "citymap.h"

class MapView : public QGraphicsView {
    Q_OBJECT
public:
    MapView(QWidget* parent = nullptr);
    void setCityMap(CityMap* cityMap);
    void setMapImage(const QString& imagePath);
    void highlightPath(const QList<int>& path);
    int getSelectedSource() const;
    int getSelectedDestination() const;
    void clearHighlights();
signals:
    void citySelected(int cityIndex, bool isSource);
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
private:
    void drawMap();
    void drawCities();
    void drawRoads();
    CityMap* cityMap;
    QGraphicsScene* scene;
    QMap<int, QGraphicsEllipseItem*> cityNodes;
    QMap<QPair<int, int>, QGraphicsLineItem*> roadLines;
    int hoveredCity;
    int selectedSource;
    int selectedDestination;
    QPixmap mapPixmap;
};

#endif // MAPVIEW_H 