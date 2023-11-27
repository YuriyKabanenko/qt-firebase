#ifndef USERPAGE_H
#define USERPAGE_H

#include <QWidget>

#include "user.h"

namespace Ui {
class UserPage;
}

class UserPage : public QWidget
{
    Q_OBJECT

public:
    explicit UserPage(QWidget *parent = nullptr, User* user = nullptr);
    ~UserPage();

private slots:
    void on_signOutButton_clicked();

private:
    Ui::UserPage *ui;
    User* user;
};

#endif // USERPAGE_H
