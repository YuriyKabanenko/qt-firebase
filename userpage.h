#ifndef USERPAGE_H
#define USERPAGE_H

#include <QWidget>
#include "authhandler.h"
#include "user.h"

namespace Ui {
class UserPage;
}

class UserPage : public QWidget
{
    Q_OBJECT

public:
    explicit UserPage(QWidget *parent = nullptr, User* user = nullptr, AuthHandler* authHandler = nullptr);
    ~UserPage();

private slots:
    void on_signOutButton_clicked();

    void on_deleteAccountbutton_clicked();

signals:
    void userSignOut();
private:
    Ui::UserPage *ui;
    AuthHandler* authHandler;
    User* user;

};

#endif // USERPAGE_H
