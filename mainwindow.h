#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>
#include "userdatastructures.h"
#include "profilemanager.h"
#include "cabbooking.h"
class CityMapWindow; // Forward declaration

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_signup_clicked();
    void on_pushButton_viewProfile_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase sqlitedb;
    UserProfileManager profileManager;
    User* currentUser;
    CabBooking* bookingWindow;
    CityMapWindow* cityMapWindow;
    void loadUsersFromDatabase();
    void setupMainOptionsView();
};
#endif // MAINWINDOW_H
