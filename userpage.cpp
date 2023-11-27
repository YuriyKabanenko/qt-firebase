#include "userpage.h"
#include "./ui_userpage.h"

UserPage::UserPage(QWidget *parent, User* user) :
    QWidget(parent),
    ui(new Ui::UserPage)
{
    ui->setupUi(this);
    this->user = user;
    ui->label->setText(ui->label->text() + " " + user->getEmail());
}

UserPage::~UserPage()
{
    delete ui;
}
