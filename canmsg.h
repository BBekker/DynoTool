#ifndef CANMSG_H
#define CANMSG_H

#include <stdint.h>

#define U16(lower, upper) (lower)|((upper) << 8)
#define control_bits(inverter, dc, enable, error_reset) ((inverter) | ((dc)<<1) | ((enable) << 2) | ((error_reset) <<3))


namespace CANMSG
{




    struct setpoints
    {
        uint8_t control_bits;
        int16_t velocity;
        int16_t torque_limit_positive;
        int16_t torque_limit_negative;

        setpoints(  uint8_t control_bits,
                    int16_t velocity,
                    int16_t torque_limit_positive,
                    int16_t torque_limit_negative)
            :control_bits(control_bits),
              velocity(velocity),
              torque_limit_positive(torque_limit_positive),
              torque_limit_negative(torque_limit_negative)
        {}

        void getbytes(uint8_t *bytes)
        {
            bytes[0] = 0;
            bytes[1] = control_bits;
            bytes[2] = (uint8_t) velocity;
            bytes[3] = (uint8_t) (velocity << 8);
            bytes[4] = (uint8_t) torque_limit_positive;
            bytes[5] = (uint8_t) (torque_limit_positive << 8);
            bytes[6] = (uint8_t) torque_limit_negative;
            bytes[7] = (uint8_t) (torque_limit_negative << 8);
        }
    };

    struct AMK_1
    {
        uint8_t status;
        int16_t actual_velocity;
        int16_t torque_current;
        int16_t magnetizing_current;
        AMK_1(uint8_t *bytes) :
            status(bytes[1]),
            actual_velocity(U16(bytes[2],bytes[3])),
            torque_current(U16(bytes[4],bytes[5])),
            magnetizing_current(U16(bytes[6],bytes[7]))
        {}
    };

    struct AMK_2
    {
        uint16_t temperature_motor;
        uint16_t temperature_inverter;
        uint16_t error;
        uint16_t temperature_igbt;
        AMK_2(uint8_t *bytes) :
            temperature_motor(U16(bytes[0],bytes[1])),
            temperature_inverter(U16(bytes[2],bytes[3])),
            error(U16(bytes[4],bytes[5])),
            temperature_igbt(U16(bytes[6],bytes[7]))
        {}
    };

}


#endif // CANMSG_H
