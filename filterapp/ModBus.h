#ifndef MODBUS_H
#define MODBUS_H

#include <cstdint>

class QByteArray;

namespace sfv2 {

    class ModBus
    {
    public:
        ModBus() = delete;
        ~ModBus() = delete;

        //! Create MODBUS FC6 message (Preset Single Register).
        static void preset_single_register(uint8_t slave_address,
                                           uint16_t data_address,
                                           uint16_t value,
                                           QByteArray& buffer);

        //! Create MODBUS FC16 message (Preset Multiple Registers).
        static void preset_multiple_registers(uint8_t slave_address,
                                              uint16_t data_address,
                                              uint16_t num_registers,
                                              uint16_t* values,
                                              QByteArray& buffer);
    };

}

#endif // MODBUS_H
