#ifndef CITYMAP_H
#define CITYMAP_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QPair>
#include <QList>
#include <limits>

// Structure to represent a city node
struct CityNode {
    QString name;
    int x, y; // Coordinates for map placement
};

// Structure to represent an edge (road)
struct Road {
    int to;
    double distance; // in kilometers
};

class CityMap {
public:
    CityMap();
    void addCity(const QString& name, int x, int y);
    void addRoad(const QString& from, const QString& to, double distance);
    // Returns: (totalDistance, path as list of city indices)
    QPair<double, QList<int>> dijkstra(const QString& source, const QString& destination) const;
    const QVector<CityNode>& getCities() const;
    const QVector<QList<Road>>& getAdjList() const;
    int getCityIndex(const QString& name) const;
private:
    QVector<CityNode> cities;
    QMap<QString, int> cityIndexMap;
    QVector<QList<Road>> adjList;
};

#endif // CITYMAP_H 