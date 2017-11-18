#include "SerialOutput.h"
#include "FilterSettings.h"
#include "ModBus.h"

#include <QSerialPort>
#include <QDebug>

namespace sfv2 {

    SerialOutput::SerialOutput()
        : sp_()
        , slave_address_(0)
        , data_address_(0)
        , buffer_()
    {

    }

    SerialOutput::~SerialOutput()
    {

    }

    bool SerialOutput::open(const FilterSettings& settings)
    {
        const auto& port_name = settings.serialPortName();

        sp_ = std::make_unique<QSerialPort>(port_name);

        if (!sp_->setBaudRate(settings.serialBaudRate())) {
            qWarning() << "Invalid baud rate setting for serial port";
            return false;
        }
        if (!sp_->setDataBits(settings.serialDataBits())) {
            qWarning() << "Invalid data bits setting for serial port";
            return false;
        }
        if (!sp_->setParity(settings.serialParity())) {
            qWarning() << "Invalid parity setting for serial port";
            return false;
        }
        if (!sp_->setStopBits(settings.serialStopBits())) {
            qWarning() << "Invalid stop bits setting for serial port";
            return false;
        }
        if (!sp_->setFlowControl(settings.serialFlowControl())) {
            qWarning() << "Invalid flow control setting for serial port";
            return false;
        }
        if (!sp_->open(QSerialPort::WriteOnly)) {
            qWarning() << "Failed to open serial port: " << port_name;
            return false;
        }

        qInfo() << "Opened serial port:" << port_name;

        slave_address_ = settings.modbusSlaveAddress();
        data_address_ = settings.modbusDataAddress();

        return true;
    }

    bool SerialOutput::write(const FilterOutputData& data)
    {
        // Convert entropy to 32-bit float and copy it bit-by-bit to an integer
        const auto f_entropy = static_cast<float>(data.entropy());
        const auto i_entropy = *reinterpret_cast<const uint32_t*>(&f_entropy);

        uint16_t values[] = {static_cast<uint16_t>(data.measurement()),
                             static_cast<uint16_t>(data.status()),
                             static_cast<uint16_t>(i_entropy >> 16),
                             static_cast<uint16_t>(i_entropy & 0xFFFF)};
        ModBus::preset_multiple_registers(slave_address_,
                                          data_address_,
                                          sizeof(values) / sizeof(values[0]),
                                          values,
                                          buffer_);

        if (sp_->write(buffer_.constData(), buffer_.size()) != buffer_.size()) {
            qWarning() << "Failed to write output buffer";
            return false;
        }

        return true;
    }

}
