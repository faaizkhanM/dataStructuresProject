#ifndef CAB_H
#define CAB_H

#include <QString>
#include <QGraphicsPixmapItem>
#include <QObject>

#include <QObject>
#include <QGraphicsPixmapItem>

class AnimatedCab : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    explicit AnimatedCab(const QPixmap &pixmap, QObject *parent = nullptr)
        : QObject(parent), QGraphicsPixmapItem(pixmap) {} // ✅ Corrected inheritance order
};


class Cab {
private:
    QString numberPlate;
    QString driverName;
    float rating;
    float distance;  // Distance from user
    bool available;

public:
    // Constructor
    Cab(QString plate = "", QString name = "", float rate = 5.0, float dist = 0.0, bool avail = true);

    // Getters
    QString getNumberPlate() const;
    QString getDriverName() const;
    float getRating() const;
    float getDistance() const;
    bool isAvailable() const;
    
    // Priority calculation based on rating and distance
    float getPriority() const {
        // Higher rating and lower distance means higher priority
        return rating - (distance * 0.1); // Distance has a smaller impact on priority
    }

    // Setters
    void setAvailability(bool status);
    void setRating(float newRating);
    void setDistance(float dist);
    void setAvailable(bool status) {
        available = status;
    }
};

#endif // CAB_H
