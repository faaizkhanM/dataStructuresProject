#include "mapview.h"
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QBrush>
#include <QPen>
#include <QtMath>
#include <QPainterPath>

MapView::MapView(QWidget* parent)
    : QGraphicsView(parent), cityMap(nullptr), scene(new QGraphicsScene(this)), hoveredCity(-1), selectedSource(-1), selectedDestination(-1) {
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
}

void MapView::setCityMap(CityMap* cityMap) {
    this->cityMap = cityMap;
    drawMap();
}

void MapView::setMapImage(const QString& imagePath) {
    mapPixmap = QPixmap(imagePath);
    drawMap();
}

void MapView::drawMap() {
    scene->clear();
    cityNodes.clear();
    roadLines.clear();
    if (!mapPixmap.isNull()) {
        scene->addPixmap(mapPixmap);
    }
    if (cityMap) {
        drawRoads();
        drawCities();
    }
}

void MapView::drawCities() {
    const auto& cities = cityMap->getCities();
    for (int i = 0; i < cities.size(); ++i) {
        const CityNode& city = cities[i];
        QGraphicsEllipseItem* node = scene->addEllipse(city.x-12, city.y-12, 24, 24, QPen(Qt::black, 2), QBrush(QColor(0, 200, 255)));
        node->setZValue(1);
        cityNodes[i] = node;
        // Spread out city labels more to avoid overlap
        int xOffset = 20, yOffset = -25;
        if (city.name == "Karachi") { xOffset = 40; yOffset = 30; }
        else if (city.name == "Hyderabad") { xOffset = 40; yOffset = 40; }
        else if (city.name == "Sukkur") { xOffset = 40; yOffset = 20; }
        else if (city.name == "Quetta") { xOffset = -70; yOffset = 0; }
        else if (city.name == "Multan") { xOffset = 40; yOffset = 20; }
        else if (city.name == "Bahawalpur") { xOffset = 40; yOffset = 40; }
        else if (city.name == "Lahore") { xOffset = 40; yOffset = -30; }
        else if (city.name == "Faisalabad") { xOffset = -70; yOffset = 0; }
        else if (city.name == "Sialkot") { xOffset = 40; yOffset = -30; }
        else if (city.name == "Gujranwala") { xOffset = 40; yOffset = 40; }
        else if (city.name == "Islamabad") { xOffset = 40; yOffset = -30; }
        else if (city.name == "Rawalpindi") { xOffset = -70; yOffset = 0; }
        else if (city.name == "Peshawar") { xOffset = -70; yOffset = 0; }
        else if (city.name == "Abbottabad") { xOffset = -70; yOffset = 0; }
        else if (city.name == "Murree") { xOffset = 40; yOffset = -30; }
        // Draw city name with outline for pop
        QGraphicsTextItem* label = scene->addText("");
        QFont font = label->font();
        font.setPointSize(14);
        font.setBold(true);
        label->setFont(font);
        // Use QPainterPath for outline effect
        QPainterPath path;
        path.addText(0, 0, font, city.name);
        QGraphicsPathItem* outline = scene->addPath(path, QPen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        outline->setPos(city.x + xOffset, city.y + yOffset);
        outline->setZValue(2);
        QGraphicsPathItem* fill = scene->addPath(path, QPen(Qt::NoPen), QBrush(QColor(0,255,255)));
        fill->setPos(city.x + xOffset, city.y + yOffset);
        fill->setZValue(3);
    }
}

void MapView::drawRoads() {
    const auto& cities = cityMap->getCities();
    const auto& adj = cityMap->getAdjList();
    for (int i = 0; i < adj.size(); ++i) {
        for (const Road& road : adj[i]) {
            int j = road.to;
            if (i < j) { // Avoid duplicate lines
                QGraphicsLineItem* line = scene->addLine(cities[i].x, cities[i].y, cities[j].x, cities[j].y, QPen(Qt::gray, 2));
                roadLines[{i, j}] = line;
            }
        }
    }
}

void MapView::highlightPath(const QList<int>& path) {
    clearHighlights();
    // Highlight the path with a thick blue line
    for (int k = 1; k < path.size(); ++k) {
        int i = path[k-1], j = path[k];
        QPair<int, int> key = (i < j) ? QPair<int, int>(i, j) : QPair<int, int>(j, i);
        if (roadLines.contains(key)) {
            roadLines[key]->setPen(QPen(Qt::blue, 4));
        }
        if (cityNodes.contains(i))
            cityNodes[i]->setBrush(QBrush(Qt::cyan));
    }
    if (!path.isEmpty() && cityNodes.contains(path.last()))
        cityNodes[path.last()]->setBrush(QBrush(Qt::cyan));
}

void MapView::clearHighlights() {
    // Reset all lines and nodes to default
    for (auto line : roadLines.values())
        line->setPen(QPen(Qt::gray, 2));
    for (auto node : cityNodes.values())
        node->setBrush(QBrush(QColor(0, 120, 255)));
}

void MapView::mousePressEvent(QMouseEvent* event) {
    QPointF pt = mapToScene(event->pos());
    int clickedCity = -1;
    // Find city under mouse
    for (auto it = cityNodes.begin(); it != cityNodes.end(); ++it) {
        if (it.value()->contains(it.value()->mapFromScene(pt))) {
            clickedCity = it.key();
            break;
        }
    }
    if (clickedCity != -1) {
        if (selectedSource == -1) {
            selectedSource = clickedCity;
            cityNodes[clickedCity]->setBrush(QBrush(Qt::green)); // Highlight source
            emit citySelected(clickedCity, true);
        } else if (selectedDestination == -1 && clickedCity != selectedSource) {
            selectedDestination = clickedCity;
            cityNodes[clickedCity]->setBrush(QBrush(Qt::red)); // Highlight destination
            emit citySelected(clickedCity, false);
        } else {
            // Reset selection
            clearHighlights();
            selectedSource = clickedCity;
            selectedDestination = -1;
            cityNodes[clickedCity]->setBrush(QBrush(Qt::green));
            emit citySelected(clickedCity, true);
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void MapView::mouseMoveEvent(QMouseEvent* event) {
    QPointF pt = mapToScene(event->pos());
    int hover = -1;
    for (auto it = cityNodes.begin(); it != cityNodes.end(); ++it) {
        if (it.value()->contains(it.value()->mapFromScene(pt))) {
            hover = it.key();
            break;
        }
    }
    if (hover != hoveredCity) {
        if (hoveredCity != -1 && cityNodes.contains(hoveredCity) && hoveredCity != selectedSource && hoveredCity != selectedDestination)
            cityNodes[hoveredCity]->setBrush(QBrush(QColor(0, 120, 255)));
        if (hover != -1 && cityNodes.contains(hover) && hover != selectedSource && hover != selectedDestination)
            cityNodes[hover]->setBrush(QBrush(Qt::magenta));
        hoveredCity = hover;
    }
    QGraphicsView::mouseMoveEvent(event);
}

int MapView::getSelectedSource() const { return selectedSource; }
int MapView::getSelectedDestination() const { return selectedDestination; } 