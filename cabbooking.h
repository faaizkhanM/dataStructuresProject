#ifndef CABBOOKING_H
#define CABBOOKING_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include "cab.h"
#include "user.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QDialog>
#include <QSlider>
#include <QVBoxLayout>
#include <QMediaPlayer>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QInputDialog>
#include "mapview.h"
#include "citymap.h"
#include <QVector>
#include <QRandomGenerator>

namespace Ui {
class CabBooking;
}

struct Driver {
    QString name;
    QString numberPlate;
    float rating;
    QString city;
    bool available;
    double pricePer10km;
};

class CabBooking : public QDialog
{
    Q_OBJECT

public:
    explicit CabBooking(QWidget *parent = nullptr);
    ~CabBooking();
    void setUser(User* user) { currentUser = user; }

private slots:
    void bookCab();
    void on_comboBoxSort_currentIndexChanged(int index);
    void on_btnViewProfile_clicked();
    void onTipSubmitted(double tip);
    void displayCabsInTable();
    void on_tableCabs_cellClicked(int row, int column);
    void on_btnAISmartBook_clicked();
    void showFareSummary(double fare, double distance);
    void showRatingPrompt();
    void showTipPrompt();

private:
    Ui::CabBooking *ui;
    static const int MAX_CABS = 10;
    Cab cabList[MAX_CABS];
    int cabCount;
    QString currentCabPlate;
    User* currentUser;  // Add member variable for current user
    double getUserTip();
    int showRatingsPrompt();
    QString cabID;  // Changed from int to QString to store number plate
    double rideDistance;
    int driverID;
    double userTip;  // Store the tip amount
    void updateDriverRating(int driverID, int rating);
    QSqlDatabase sqlitedb = QSqlDatabase::database(); // Use existing connection
    void loadCabsFromDatabase(); // Pull data from SQLite
    Cab bookCabWithPriorityQueue();
    QGraphicsScene *scene;
    QGraphicsPixmapItem *carItem;
    QPropertyAnimation *carAnimation;
    void playRideAnimation();
    void updateCabRating(const QString &plate, float newRating);
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    void updateCabEarnings(const QString &cabID, double distanceKm, double tipAmount);
    void handleVideoFinished(QMediaPlayer::MediaStatus status);
    void showThankYouMessage();
    void showBookingConfirmation();
    void updateDatabaseAfterRide();
    void clearSelectedDriverInfo();  // Clear selected driver information
    QList<int> getDriverRoute(const Driver& driver, const QString& source, const QString& destination);
    QList<int> getAlternativeRoute(const QString& source, const QString& destination);
    // Map integration
    MapView* mapView;
    CityMap* cityMap;
    QVector<Driver> drivers;
    QVector<Driver> currentDisplayedDrivers; // Store currently displayed drivers
    QString selectedSourceCity;
    int selectedDriverIndex;
    int optimalDriverIndex;
    QString selectedDestination;
    QList<int> selectedDriverPath;  // Store the path for selected driver
    double selectedDriverDistance;  // Store the distance for selected driver
    int selectedDriverFare;         // Store the fare for selected driver
};

#endif // CABBOOKING_H
