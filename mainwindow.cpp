#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "signup.h"
#include "cabbooking.h"
#include "profilemanager.h"
#include "userdatastructures.h"
#include <QMessageBox>
#include <QMediaPlayer>
#include <QVideoWidget>
#include "citymapwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentUser(nullptr)
    , bookingWindow(nullptr)
    , cityMapWindow(nullptr)
{
    ui->setupUi(this);
    // Setup login background video
    QMediaPlayer *player = new QMediaPlayer(this);
    player->setVideoOutput(ui->VideoWidget);
    player->setSource(QUrl::fromLocalFile("C:/Users/Faaiz Khan/Downloads/University Folder/4th Sem Course Contents/Project Copy/CabBookingSys/CabBookingSystem/images/Login.mp4"));
    player->setLoops(-1);
    player->play();
    loadUsersFromDatabase();
}

MainWindow::~MainWindow()
{
    delete ui;
    if (bookingWindow) {
        delete bookingWindow;
    }
    if (cityMapWindow) {
        delete cityMapWindow;
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Error", "Please enter both username and password.");
        return;
    }

    User* user = profileManager.findUser(username);
    if (user && user->getPassword() == password) {
        currentUser = user;
        // Hide and delete cityMapWindow if it exists
        if (cityMapWindow) {
            cityMapWindow->hide();
            delete cityMapWindow;
            cityMapWindow = nullptr;
        }
        // Show cab booking window
        if (!bookingWindow) {
            bookingWindow = new CabBooking(this);
        }
        bookingWindow->setUser(currentUser);
        bookingWindow->show();
        this->hide();
    } else {
        QMessageBox::warning(this, "Login Error", "Invalid username or password.");
    }
}

void MainWindow::on_pushButton_signup_clicked()
{
    Signup signupDialog(this);
    signupDialog.setProfileManager(&profileManager);
    signupDialog.setModal(true);
    signupDialog.exec();
}

void MainWindow::on_pushButton_viewProfile_clicked()
{
    if (!currentUser) {
        QMessageBox::warning(this, "Profile", "No user is currently logged in.");
        return;
    }
    ProfileManager profileDialog(this);
    profileDialog.setUser(*currentUser);
    profileDialog.setModal(true);
    profileDialog.exec();
}

void MainWindow::loadUsersFromDatabase()
{
    // Load all users from the database into the profile manager (DSA logic)
    QSqlDatabase sqlitedb = QSqlDatabase::addDatabase("QSQLITE");
    sqlitedb.setDatabaseName("C:/Users/Faaiz Khan/Downloads/University Folder/4th Sem Course Contents/DSA/CabBookingSys/CabBookingSystem/build/Desktop_Qt_6_9_0_MinGW_64_bit-Debug/login.db");
    if (sqlitedb.open()) {
        QSqlQuery query(sqlitedb);
        query.exec("SELECT usernames, passwords FROM Logindetails");
        while (query.next()) {
            QString username = query.value(0).toString();
            QString password = query.value(1).toString();
            User user(username, password);
            profileManager.addUser(user);
        }
        sqlitedb.close();
    }
}




