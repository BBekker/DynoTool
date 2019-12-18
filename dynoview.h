#ifndef DYNOVIEW_H
#define DYNOVIEW_H


#include <QMainWindow>
#include <QtCore>
#include <QChart>
#include <QLineSeries>
#include <cstdint>

QT_BEGIN_NAMESPACE
namespace Ui { class dynoview; }
QT_END_NAMESPACE

using namespace QtCharts;

class dynoview : public QMainWindow
{
    Q_OBJECT

public:
    dynoview(int id, QWidget *parent = nullptr);
    ~dynoview();

public slots:
    void part_1(int id, uint8_t status, int16_t actual_velocity, double torque_current, double magnetizing_current);
    void part_2(int id, uint16_t temperature_motor, uint16_t temperature_inverter, uint16_t error, uint16_t temperature_igbt);
    void plot();

signals:
    void setpoints(int mc_id, int velocity, uint16_t torque_positive, uint16_t torque_negative);


private:
    int id;
    Ui::dynoview *ui;
    QTimer *timer;
    QLineSeries *series;
    QChart *chart;

    //live values
    int actual_velocity;
    uint8_t status;
    double torque_current;
    double magnetizing_current;
    double Tmotor;
    double Tinverter;
    double Tigbt;
    int errornum;


    void connectToCAN();
    void connectControls();
    void setupCharts();
    void updateUI();

    void sendSetpoints();
};
#endif // DYNOVIEW_H
