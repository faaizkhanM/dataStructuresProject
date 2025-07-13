#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QDialog>
#include "user.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ProfileManager; }
QT_END_NAMESPACE

class ProfileManager : public QDialog
{
    Q_OBJECT

public:
    explicit ProfileManager(QWidget *parent = nullptr);
    ~ProfileManager();
    void setUser(const User& user);
    User getUser() const;

private slots:
    void on_pushButton_save_clicked();
    void on_pushButton_changePicture_clicked();
    void on_pushButton_deleteAccount_clicked();

private:
    Ui::ProfileManager *ui;
    User currentUser;
    void loadUserData();
    void saveUserData();
};

#endif // PROFILEMANAGER_H 