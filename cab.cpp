#include "cab.h"

// Constructor
Cab::Cab(QString plate, QString name, float rate, float dist, bool avail) {
    numberPlate = plate;
    driverName = name;
    rating = rate;
    distance = dist;
    available = avail;
}

// Getters
QString Cab::getNumberPlate() const { return numberPlate; }
QString Cab::getDriverName() const { return driverName; }
float Cab::getRating() const { return rating; }
float Cab::getDistance() const { return distance; }
bool Cab::isAvailable() const { return available; }

// Setters
void Cab::setAvailability(bool status) { available = status; }
void Cab::setRating(float newRating) { rating = newRating; }
void Cab::setDistance(float dist) { distance = dist; }
