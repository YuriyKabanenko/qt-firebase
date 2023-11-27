#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    authHandler = new AuthHandler(this);
    authHandler->setAPIKey(apiKey);

    qDebug() << QSslSocket::sslLibraryBuildVersionString();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_registerButton_clicked()
{
    QString email = ui->emailEdit->toPlainText();
    QString password = ui->passwordEdit->toPlainText();

    if (email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter both email and password.");
        return;
    }
    authHandler->signUserUp(email,password);
}
