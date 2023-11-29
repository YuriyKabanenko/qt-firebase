#include "userpage.h"
#include "./ui_userpage.h"
#include <QMessageBox>

UserPage::UserPage(QWidget *parent, User* user, AuthHandler* authHandler) :
    QWidget(parent),
    ui(new Ui::UserPage)
{
    ui->setupUi(this);
    this->user = user;
    ui->label->setText(ui->label->text() + " " + this->user->getEmail());
    ui->label_2->setText(ui->label_2->text() + " " + this->user->getId());
    this->authHandler = authHandler;

    connect(this->authHandler, &AuthHandler::accountDeleted, this, &UserPage::on_signOutButton_clicked );
    connect(this->authHandler, &AuthHandler::emailVerified, this, &UserPage::on_email_verified );
    connect(this->authHandler, &AuthHandler::userDataUpdated, this, &UserPage::on_user_data_updated);
    if(this->user->getIsEmailVerified()){
        ui->confirmEmailButton->setDisabled(true);
    }
}

UserPage::~UserPage()
{
    delete ui;
    delete user;
}

void UserPage::on_signOutButton_clicked()
{
    qDebug() << "sign out";
    emit userSignOut();
}

void UserPage::on_deleteAccountbutton_clicked()
{
    this->authHandler->deleteAccount(this->user->getToken());
}


void UserPage::on_confirmEmailButton_clicked()
{
    this->authHandler->sendEmailVerification(this->user->getToken());
}

void UserPage::on_email_verified()
{
    QMessageBox infoDialog;
    infoDialog.setIcon(QMessageBox::Information);
    infoDialog.setWindowTitle("Congratulations");
    infoDialog.setText("We've sent mail with confirmation link");

    infoDialog.exec();

    this->authHandler->getUserData(this->user->getToken());
}

void UserPage::on_user_data_updated()
{
    this->user->setIsEmailVerified(this->authHandler->getEmailVerified());

    ui->confirmEmailButton->setDisabled(this->user->getIsEmailVerified());
}

