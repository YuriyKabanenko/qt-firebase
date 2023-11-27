#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "authhandler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_registerButton_clicked();

private:
    Ui::MainWindow *ui;
    AuthHandler* authHandler;
    const QString apiKey = "AIzaSyAvHF2PK56LaFSqsmtPumsDJNepC7xp0bc";
};
#endif // MAINWINDOW_H
