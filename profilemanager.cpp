#include "profilemanager.h"
#include "ui_profilemanager.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QBuffer>

ProfileManager::ProfileManager(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProfileManager)
{
    ui->setupUi(this);
    setWindowTitle("Profile Management");
}

ProfileManager::~ProfileManager()
{
    delete ui;
}

void ProfileManager::setUser(const User& user)
{
    currentUser = user;
    currentUser.loadFromDatabase(user.getUsername());
    loadUserData();
}

User ProfileManager::getUser() const
{
    return currentUser;
}

void ProfileManager::loadUserData()
{
    ui->lineEdit_username->setText(currentUser.getUsername());
    ui->lineEdit_fullName->setText(currentUser.getFullName());
    ui->lineEdit_email->setText(currentUser.getEmail());
    ui->lineEdit_phone->setText(currentUser.getPhone());
    ui->dateEdit_dob->setDate(currentUser.getDateOfBirth());
    ui->textEdit_address->setText(currentUser.getAddress());

    // Display profile picture
    QPixmap profilePic = currentUser.getProfilePicture();
    if (!profilePic.isNull()) {
        ui->label_profilePicture->setPixmap(profilePic.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void ProfileManager::saveUserData()
{
    currentUser.setFullName(ui->lineEdit_fullName->text());
    currentUser.setEmail(ui->lineEdit_email->text());
    currentUser.setPhone(ui->lineEdit_phone->text());
    currentUser.setDateOfBirth(ui->dateEdit_dob->date());
    currentUser.setAddress(ui->textEdit_address->toPlainText());

    if (currentUser.updateProfile()) {
        QMessageBox::information(this, "Success", "Profile updated successfully!");
        currentUser.loadFromDatabase(currentUser.getUsername());
        loadUserData();
    } else {
        QMessageBox::critical(this, "Error", "Failed to update profile!");
    }
}

void ProfileManager::on_pushButton_save_clicked()
{
    saveUserData();
}

void ProfileManager::on_pushButton_changePicture_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Select Profile Picture", "",
        "Image Files (*.png *.jpg *.jpeg *.bmp)");

    if (!fileName.isEmpty()) {
        QPixmap newPicture(fileName);
        if (!newPicture.isNull()) {
            currentUser.setProfilePicture(newPicture);
            ui->label_profilePicture->setPixmap(newPicture.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            currentUser.updateProfile();
        }
    }
}

void ProfileManager::on_pushButton_deleteAccount_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,
        "Delete Account",
        "Are you sure you want to delete your account? This action cannot be undone.",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (currentUser.deleteProfile()) {
            QMessageBox::information(this, "Success", "Account deleted successfully!");
            accept(); // Close the dialog
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete account!");
        }
    }
} 