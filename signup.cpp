#include "signup.h"
#include "ui_signup.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

Signup::Signup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Signup),
    profileManager(nullptr)
{
    ui->setupUi(this);
    setupBackgroundVideo();
}

Signup::~Signup()
{
    delete ui;
}

void Signup::setupBackgroundVideo()
{
    QMediaPlayer *player = new QMediaPlayer(this);
    QVideoWidget *videoWidget = ui->VideoWidget;
    player->setVideoOutput(videoWidget);
    player->setSource(QUrl::fromLocalFile("C:/Users/Faaiz Khan/Videos/Screen Recordings/Signup.mp4"));
    player->setLoops(-1);
    player->play();
    qDebug() << "Is QVideoWidget visible? " << ui->VideoWidget->isVisible();
}

void Signup::on_pushButton_signup_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    // Validate input
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Username and password cannot be empty!");
        return;
    }

    // Check if user already exists
    if (profileManager && profileManager->findUser(username)) {
        QMessageBox::warning(this, "Error", "Username already exists!");
        return;
    }

    // Create new user
    User newUser(username, password);
    
    // Save to database
    QSqlDatabase sqlitedb = QSqlDatabase::addDatabase("QSQLITE");
    sqlitedb.setDatabaseName("C:/Users/Faaiz Khan/Downloads/University Folder/4th Sem Course Contents/DSA/CabBookingSys/CabBookingSystem/build/Desktop_Qt_6_9_0_MinGW_64_bit-Debug/login.db");

    if (sqlitedb.open()) {
        QSqlQuery query(sqlitedb);
        query.prepare("INSERT INTO Logindetails (usernames, passwords) VALUES (:username, :password)");
        query.bindValue(":username", username);
        query.bindValue(":password", password);

        if (query.exec()) {
            // Add to profile manager
            if (profileManager) {
                profileManager->addUser(newUser);
            }
            
            ui->lineEdit_status->setText("Signup successful!");
            QMessageBox::information(this, "Success", "Account created successfully!");
            accept(); // Close the dialog
        } else {
            ui->lineEdit_status->setText("Signup failed: " + query.lastError().text());
            QMessageBox::critical(this, "Error", "Failed to create account: " + query.lastError().text());
        }

        sqlitedb.close();
    } else {
        ui->lineEdit_status->setText("Database connection error!");
        QMessageBox::critical(this, "Error", "Database connection error!");
    }
}
