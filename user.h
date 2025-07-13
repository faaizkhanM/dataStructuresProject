#ifndef USER_H
#define USER_H

#include <QString>
#include <QDate>
#include <QPixmap>

class User {
public:
    User();
    User(const QString& username, const QString& password);
    
    // Getters
    QString getUsername() const;
    QString getPassword() const;
    QString getFullName() const;
    QString getEmail() const;
    QString getPhone() const;
    QDate getDateOfBirth() const;
    QString getAddress() const;
    QPixmap getProfilePicture() const;
    
    // Setters
    void setUsername(const QString& username);
    void setPassword(const QString& password);
    void setFullName(const QString& fullName);
    void setEmail(const QString& email);
    void setPhone(const QString& phone);
    void setDateOfBirth(const QDate& dateOfBirth);
    void setAddress(const QString& address);
    void setProfilePicture(const QPixmap& picture);
    
    // Database operations
    bool saveToDatabase();
    bool loadFromDatabase(const QString& username);
    bool updateProfile();
    bool deleteProfile();
    
private:
    QString username;
    QString password;
    QString fullName;
    QString email;
    QString phone;
    QDate dateOfBirth;
    QString address;
    QPixmap profilePicture;
};

#endif // USER_H 