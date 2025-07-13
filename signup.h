#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>
#include "userdatastructures.h"

namespace Ui {
class Signup;
}

class Signup : public QDialog
{
    Q_OBJECT

public:
    explicit Signup(QWidget *parent = nullptr);
    ~Signup();
    void setProfileManager(UserProfileManager* manager) { profileManager = manager; }

private slots:
    void on_pushButton_signup_clicked();
    void setupBackgroundVideo();

private:
    Ui::Signup *ui;
    QSqlDatabase sqlitedb;
    UserProfileManager* profileManager;
};

#endif // SIGNUP_H
