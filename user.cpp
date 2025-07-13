#include "user.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QBuffer>
#include <QDebug>

User::User() {}

User::User(const QString& username, const QString& password)
    : username(username), password(password) {}

// Getters
QString User::getUsername() const { return username; }
QString User::getPassword() const { return password; }
QString User::getFullName() const { return fullName; }
QString User::getEmail() const { return email; }
QString User::getPhone() const { return phone; }
QDate User::getDateOfBirth() const { return dateOfBirth; }
QString User::getAddress() const { return address; }
QPixmap User::getProfilePicture() const { return profilePicture; }

// Setters
void User::setUsername(const QString& username) { this->username = username; }
void User::setPassword(const QString& password) { this->password = password; }
void User::setFullName(const QString& fullName) { this->fullName = fullName; }
void User::setEmail(const QString& email) { this->email = email; }
void User::setPhone(const QString& phone) { this->phone = phone; }
void User::setDateOfBirth(const QDate& dateOfBirth) { this->dateOfBirth = dateOfBirth; }
void User::setAddress(const QString& address) { this->address = address; }
void User::setProfilePicture(const QPixmap& picture) { this->profilePicture = picture; }

bool User::saveToDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/Faaiz Khan/Downloads/University Folder/4th Sem Course Contents/DSA/CabBookingSys/CabBookingSystem/build/Desktop_Qt_6_9_0_MinGW_64_bit-Debug/login.db");

    if (!db.open()) {
        qDebug() << "Error: Failed to connect to database:" << db.lastError().text();
        return false;
    }

    // Create users table if it doesn't exist
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "username TEXT PRIMARY KEY,"
               "password TEXT NOT NULL,"
               "full_name TEXT,"
               "email TEXT,"
               "phone TEXT,"
               "date_of_birth TEXT,"
               "address TEXT,"
               "profile_picture BLOB"
               ")");

    // Convert profile picture to byte array
    QByteArray pictureData;
    if (!profilePicture.isNull()) {
        QBuffer buffer(&pictureData);
        buffer.open(QIODevice::WriteOnly);
        profilePicture.save(&buffer, "PNG");
    }

    // Insert or update user data
    query.prepare("INSERT OR REPLACE INTO users "
                 "(username, password, full_name, email, phone, date_of_birth, address, profile_picture) "
                 "VALUES (:username, :password, :full_name, :email, :phone, :date_of_birth, :address, :profile_picture)");

    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":full_name", fullName);
    query.bindValue(":email", email);
    query.bindValue(":phone", phone);
    query.bindValue(":date_of_birth", dateOfBirth.toString("yyyy-MM-dd"));
    query.bindValue(":address", address);
    query.bindValue(":profile_picture", pictureData);

    if (!query.exec()) {
        qDebug() << "Error: Failed to save user data:" << query.lastError().text();
        db.close();
        return false;
    }

    db.close();
    return true;
}

bool User::loadFromDatabase(const QString& username) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/Faaiz Khan/Downloads/University Folder/4th Sem Course Contents/DSA/CabBookingSys/CabBookingSystem/build/Desktop_Qt_6_9_0_MinGW_64_bit-Debug/login.db");

    if (!db.open()) {
        qDebug() << "Error: Failed to connect to database:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Error: Failed to load user data:" << query.lastError().text();
        db.close();
        return false;
    }

    if (query.next()) {
        this->username = query.value("username").toString();
        this->password = query.value("password").toString();
        this->fullName = query.value("full_name").toString();
        this->email = query.value("email").toString();
        this->phone = query.value("phone").toString();
        this->dateOfBirth = QDate::fromString(query.value("date_of_birth").toString(), "yyyy-MM-dd");
        this->address = query.value("address").toString();

        QByteArray pictureData = query.value("profile_picture").toByteArray();
        if (!pictureData.isEmpty()) {
            profilePicture.loadFromData(pictureData);
        }

        db.close();
        return true;
    }

    db.close();
    return false;
}

bool User::updateProfile() {
    return saveToDatabase(); // Reuse saveToDatabase as it handles both insert and update
}

bool User::deleteProfile() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/Faaiz Khan/Downloads/University Folder/4th Sem Course Contents/DSA/CabBookingSys/CabBookingSystem/build/Desktop_Qt_6_9_0_MinGW_64_bit-Debug/login.db");

    if (!db.open()) {
        qDebug() << "Error: Failed to connect to database:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Error: Failed to delete user:" << query.lastError().text();
        db.close();
        return false;
    }

    db.close();
    return true;
} 