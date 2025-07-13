#include "citymap.h"
#include <QQueue>
#include <QSet>
#include <QDebug>
#include <cmath>

CityMap::CityMap() {}

void CityMap::addCity(const QString& name, int x, int y) {
    if (cityIndexMap.contains(name)) return;
    int idx = cities.size();
    cities.push_back({name, x, y});
    cityIndexMap[name] = idx;
    adjList.resize(cities.size());
}

void CityMap::addRoad(const QString& from, const QString& to, double distance) {
    int fromIdx = getCityIndex(from);
    int toIdx = getCityIndex(to);
    if (fromIdx == -1 || toIdx == -1) return;
    adjList[fromIdx].append({toIdx, distance});
    adjList[toIdx].append({fromIdx, distance}); // undirected
}

int CityMap::getCityIndex(const QString& name) const {
    return cityIndexMap.value(name, -1);
}

const QVector<CityNode>& CityMap::getCities() const {
    return cities;
}

const QVector<QList<Road>>& CityMap::getAdjList() const {
    return adjList;
}

// Dijkstra's algorithm to find shortest path from source to destination
QPair<double, QList<int>> CityMap::dijkstra(const QString& source, const QString& destination) const {
    int n = cities.size();
    int src = getCityIndex(source);
    int dest = getCityIndex(destination);
    if (src == -1 || dest == -1) return {0, {}};

    QVector<double> dist(n, std::numeric_limits<double>::max()); // Distance from source
    QVector<int> prev(n, -1); // Previous node in optimal path
    QSet<int> visited;

    dist[src] = 0;

    // Simple priority queue: (distance, city index)
    QVector<QPair<double, int>> pq;
    pq.append({0, src});

    while (!pq.isEmpty()) {
        // Find node with smallest distance
        int minIdx = 0;
        for (int i = 1; i < pq.size(); ++i) {
            if (pq[i].first < pq[minIdx].first) minIdx = i;
        }
        int u = pq[minIdx].second;
        double d = pq[minIdx].first;
        pq.removeAt(minIdx);

        if (visited.contains(u)) continue;
        visited.insert(u);

        if (u == dest) break;

        // For each neighbor
        for (const Road& road : adjList[u]) {
            int v = road.to;
            double alt = dist[u] + road.distance;
            if (alt < dist[v]) {
                dist[v] = alt;
                prev[v] = u;
                pq.append({alt, v});
            }
        }
    }

    // Reconstruct path
    QList<int> path;
    for (int at = dest; at != -1; at = prev[at]) {
        path.prepend(at);
    }
    if (path.isEmpty() || path.first() != src) return {0, {}}; // No path
    return {dist[dest], path};
} 