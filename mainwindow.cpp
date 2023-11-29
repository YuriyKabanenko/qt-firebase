#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include "userpage.h"

MainWindow::MainWindow(QWidget *parent, AuthHandler* authHandler)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(authHandler == NULL){
    user = new User();
    this->authHandler = new AuthHandler(this, user);
    this->authHandler->setAPIKey(apiKey);
    } else{
     this->authHandler = authHandler;
    }

    connect(this->authHandler, &AuthHandler::userSignedIn, this, &MainWindow::on_user_signed_in );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_registerButton_clicked()
{
    QString email = ui->emailEdit->toPlainText();
    QString password = ui->passwordEdit->toPlainText();

    if(!isValid(email, password, this)){
        return;
    }

    authHandler->signUserUp(email,password);

    QMessageBox::information(this, "Congratulations", "You've registered account");

    ui->emailEdit->clear();
    ui->passwordEdit->clear();
}

void MainWindow::on_loginButton_clicked()
{
    QString email = ui->emailEdit->toPlainText();
    QString password = ui->passwordEdit->toPlainText();

    if(!isValid(email, password, this)){
        return;
    }

    authHandler->signUserIn(email,password);

    ui->emailEdit->clear();
    ui->passwordEdit->clear();
}

void MainWindow::on_user_signed_in()
{
    this->userPage = new UserPage(nullptr, this->user, this->authHandler);

    qDebug() << user->getEmail() << " " << "signed in";

    this->userPage->show();
    hide();
    connect(userPage, &UserPage::userSignOut, [this]() {
        this->userPage->hide();
        show();
    });

}

bool MainWindow::isValid(QString& email, QString& password, QWidget* object){
    static QRegularExpression emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    QRegularExpressionMatch match = emailRegex.match(email);

    if (!match.hasMatch()) {
        QMessageBox::warning(object, "Error", "Invalid email address format.");
        return false;
    }
    if (password.isEmpty()) {
        QMessageBox::warning(object, "Error", "Please enter a password.");
        return false;
    }

    return true;
}
