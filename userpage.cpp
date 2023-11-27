#include "userpage.h"
#include "./ui_userpage.h"
#include "mainwindow.h"
UserPage::UserPage(QWidget *parent, User* user, AuthHandler* authHandler) :
    QWidget(parent),
    ui(new Ui::UserPage)
{
    ui->setupUi(this);
    this->user = user;
    ui->label->setText(ui->label->text() + " " + user->getEmail());
    this->authHandler = authHandler;

    connect(this->authHandler, &AuthHandler::accountDeleted, this, &UserPage::on_signOutButton_clicked );
}

UserPage::~UserPage()
{
    delete ui;
    delete user;
}

void UserPage::on_signOutButton_clicked()
{
    emit userSignOut();
}

void UserPage::on_deleteAccountbutton_clicked()
{
    this->authHandler->deleteAccount(this->user->getToken());
}


