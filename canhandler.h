#ifndef CANHANDLER_H
#define CANHANDLER_H

#include "dynoview.h"

#include <QCanBusDevice>
#include <QObject>
#include <QtCore>

class CANHandler : public QThread
{
    Q_OBJECT
public:

    ~CANHandler()
    {
        exit();
        wait();
    }

public slots:
    void onPacket();
    void setpoints(int id, int velocity, uint16_t torque, uint16_t neg_torque);
    void enable(int id, bool enable);
    void error_reset(int id, bool reset);
signals:
    void newMotor(int id);
    void part_1(int id, uint8_t status, int16_t actual_velocity, double torque_current, double magnetizing_current);
    void part_2(int id, uint16_t temperature_motor, uint16_t temperature_inverter, uint16_t error, uint16_t temperature_igbt);

private:
    static const int MAX_DEVICES = 10;

    unsigned int mc_values_1  = 0x282;
    unsigned int mc_values_2  = 0x284;
    unsigned int mc_setpoints = 0x183;

    struct motorvalues
    {
        bool connected;
        int velocity;
        uint16_t torque;
        uint16_t neg_torque;
        uint8_t status;
        bool enable_bit;
        bool error_reset;
    };

    std::array<motorvalues,MAX_DEVICES> devices{};

    QCanBusDevice *canDevice;
    QTimer *sendTimer;

    void run() override;
    void connectToCAN();
    void handlePacket(QCanBusFrame const &frame);
    void sendSetpoints();
};

#endif // CANHANDLER_H
