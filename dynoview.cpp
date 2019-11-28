#include "dynoview.h"
#include "ui_dynoview.h"
#include <QString>
#include <QCanBus>
#include <iostream>
#include <QDebug>
#include <random>

dynoview::dynoview(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::dynoview)
{
    ui->setupUi(this);
    connectControls();
    connectToCAN();
    setupCharts();


    timer = new QTimer(this);
    timer->setInterval(16);
    connect(timer, &QTimer::timeout, this, &dynoview::plot);
    timer->start();
}


void dynoview::connectControls()
{
    connect(ui->verticalSlider, &QSlider::valueChanged, this, [=](int value){ui->spinBox->setValue(value);});
    connect(ui->verticalSlider_2, &QSlider::valueChanged, this, [=](int value){ui->spinBox_2->setValue(value);});
    connect(ui->verticalSlider_2, &QSlider::valueChanged, this, [=](int value){
        if(ui->checkBox->isChecked())
            ui->verticalSlider_3->setValue(value);
    });
    connect(ui->verticalSlider_3, &QSlider::valueChanged, this, [=](int value){ui->spinBox_3->setValue(-1*value);});
}

void dynoview::connectToCAN()
{
    if (QCanBus::instance()->plugins().contains(QStringLiteral("peakcan"))) {
        std::cout << "pcan plugin available" << std::endl;
    }

    QString errorString;
    const QList<QCanBusDeviceInfo> devices = QCanBus::instance()->availableDevices(
        QStringLiteral("peakcan"), &errorString);
    if (!errorString.isEmpty())
        qDebug() << errorString;

    for(auto device : devices)
    {
        QCanBus::instance()->createDevice(QStringLiteral("peakcan"), device.name());
    }
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

dynoview::~dynoview()
{
    delete ui;
    delete timer;
}

