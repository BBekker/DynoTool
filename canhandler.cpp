#include "canhandler.h"
#include <QCanBus>
#include <iostream>
#include "canmsg.h"


void CANHandler::run()
{
    connectToCAN();
    sendTimer = new QTimer();
    sendTimer->moveToThread(this);
    sendTimer->setInterval(4);
    connect(sendTimer, &QTimer::timeout, this, &CANHandler::sendSetpoints);
    sendTimer->start();
    //start event loop to listen for CAN msgs.
    exec();
    delete canDevice;
}

void CANHandler::setpoints(int id, int velocity, uint16_t torque, uint16_t neg_torque)
{
    devices[id].velocity = velocity;
    devices[id].torque = torque;
    devices[id].neg_torque = neg_torque;
}

void CANHandler::enable(int id, bool enable)
{
    devices[id].enable_bit = enable;
}

void CANHandler::error_reset(int id, bool reset)
{
    devices[id].error_reset = reset;
}

void CANHandler::connectToCAN()
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
        this->canDevice = QCanBus::instance()->createDevice(QStringLiteral("peakcan"), device.name());
        this->canDevice->setConfigurationParameter(QCanBusDevice::BitRateKey,1000000);
        if(this->canDevice->connectDevice())
        {
            qDebug("PCAN connected");
            connect(this->canDevice, &QCanBusDevice::framesReceived, this, &CANHandler::onPacket);
        }
        else
        {
            qDebug("failed connection to pcan");
        }

    }
}

void CANHandler::sendSetpoints()
{
    for(unsigned int i = 0; i< devices.size(); i++)
    {
        auto device = devices[i];
        if(device.connected)
        {
            uint8_t setpoint_bytes[8];
            CANMSG::setpoints setpoints(device.control_bits, device.velocity, device.torque, device.neg_torque);
            setpoints.getbytes(setpoint_bytes);

            QCanBusFrame frame;
            frame.setFrameId(i+mc_setpoints);
            frame.setPayload(QByteArray((char*)setpoint_bytes,8));
            canDevice->writeFrame(frame);
        }
    }

}

void CANHandler::onPacket()
{
    auto frames = this->canDevice->readAllFrames();
    for(auto const &frame : frames)
    {
        if(frame.hasErrorStateIndicator())
        {
            //handle some sort of error
        }
        else if(frame.frameType() == QCanBusFrame::FrameType::DataFrame)
        {
            CANHandler::handlePacket(frame);
        }
    }

}



void CANHandler::handlePacket(QCanBusFrame const &frame)
{
    //Check if it belongs to an mc:
    unsigned int base_id = ((frame.frameId() - 2) & ~3) + 2 - mc_values_1;
    if(base_id < MAX_DEVICES && base_id >= 0)
    {
        if(!this->devices[base_id].connected)
        {
            this->devices[base_id].connected = true;
            emit newMotor(base_id);
            printf("Found MC with id %d", base_id);
        }
        else
        {
            if(frame.frameId() == base_id + mc_values_1)
            {
                CANMSG::AMK_1 data((uint8_t*)frame.payload().data());
                emit part_1(base_id, data.status, data.actual_velocity, data.torque_current/16384.0*107.2, data.magnetizing_current/16384.0*107.2);
            }

            if(frame.frameId() == base_id + mc_values_2)
            {
                CANMSG::AMK_2 data((uint8_t*)frame.payload().data());
                emit part_2(base_id, data.temperature_motor, data.temperature_inverter, data.error, data.temperature_igbt);
            }
        }
    }
}
