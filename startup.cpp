#include "canhandler.h"
#include "startup.h"
#include "ui_startup.h"

Startup::Startup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Startup)
{
    ui->setupUi(this);

    canHandler = new CANHandler;
    canHandler->start();

    connect(canHandler, &CANHandler::newMotor, this,&Startup::addScreen);
}

Startup::~Startup()
{
    delete ui;
}

void Startup::addScreen(int id)
{
    if(dynoviews[id] == nullptr)
    {
        dynoviews[id] = new dynoview(id);
        connect(canHandler, &CANHandler::part_1, dynoviews[id], &dynoview::part_1);
        connect(canHandler, &CANHandler::part_2, dynoviews[id], &dynoview::part_2);
        dynoviews[id]->show();
    }
}
