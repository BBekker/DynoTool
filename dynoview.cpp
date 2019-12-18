#include "dynoview.h"
#include "ui_dynoview.h"
#include <QString>
#include <QCanBus>
#include <iostream>
#include <QDebug>
#include <random>

dynoview::dynoview(int id, QWidget *parent)
    : QMainWindow(parent)
    , id(id)
    , ui(new Ui::dynoview)
{
    ui->setupUi(this);
    connectControls();
    setupCharts();


    timer = new QTimer(this);
    timer->setInterval(32);
    connect(timer, &QTimer::timeout, this, &dynoview::updateUI);
    timer->start();

}


void dynoview::connectControls()
{
    connect(ui->velocityControl, &QSlider::valueChanged, this, [=](int value){ui->spinBox->setValue(value);});
    connect(ui->torqueControl, &QSlider::valueChanged, this, [=](int value){ui->spinBox_2->setValue(value);});
    connect(ui->torqueControl, &QSlider::valueChanged, this, [=](int value){
        if(ui->checkBox->isChecked())
            ui->negTorqueControl->setValue(value);
    });
    connect(ui->negTorqueControl, &QSlider::valueChanged, this, [=](int value){ui->spinBox_3->setValue(-1*value);});
}


void dynoview::setupCharts()
{


    chart = new QChart();
    series = new QLineSeries();


    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);
    *series << QPointF(11, 0) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Simple line chart example");

    ui->graphicsView->setChart(chart);

}


void dynoview::plot()
{
    static int i = 20;
    std::mt19937 gen(1);
    std::uniform_real_distribution<float> dis(0.0, 10.0);
    series->append(i++, dis(gen));
    chart->removeAllSeries();
    chart->addSeries(series);
    chart->scroll(1,0);
}

void dynoview::sendSetpoints()
{
    int velocity = ui->velocityControl->value();
    uint16_t torque = ui->torqueControl->value();
    uint16_t neg_torque = ui->negTorqueControl->value();

    emit setpoints(id, velocity, torque, neg_torque);
}

void dynoview::updateUI()
{
    ui->velocityDisplay->setText(QString("%1").arg(actual_velocity));
    ui->IdDisplay->setText(QString("%1").arg(magnetizing_current,5,'f',2));
    ui->IqDisplay->setText(QString("%1").arg(torque_current,5,'f',2));
    ui->Error->setText(QString("%1").arg(errornum));
}

void dynoview::part_1(int id, uint8_t status, int16_t actual_velocity, double torque_current, double magnetizing_current)
{
    if(id == this->id)
    {
        this->status = status;
        this->actual_velocity = actual_velocity;
        this->torque_current = torque_current;
        this->magnetizing_current = magnetizing_current;
    }
}

void dynoview::part_2(int id, uint16_t temperature_motor, uint16_t temperature_inverter, uint16_t error, uint16_t temperature_igbt)
{
    if(id == this->id)
    {
        this->Tmotor = temperature_motor;
        this->Tinverter = temperature_inverter;
        this->Tigbt = temperature_igbt;
        this->errornum = error;
    }
}

dynoview::~dynoview()
{
    delete ui;
    delete timer;
}

