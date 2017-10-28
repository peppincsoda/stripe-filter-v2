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
        auto r = true;

        sp_ = std::make_unique<QSerialPort>();
        const auto& port_name = settings.serialPortName();
        sp_->setPortName(port_name);

        r = r && sp_->setBaudRate(settings.serialBaudRate());
        r = r && sp_->setDataBits(static_cast<QSerialPort::DataBits>(
            settings.serialDataBits()));
        r = r && sp_->setParity(enumValueFromString<QSerialPort::Parity>(settings.serialParity()));
        r = r && sp_->setStopBits(static_cast<QSerialPort::StopBits>(
            settings.serialStopBits()));
        r = r && sp_->setFlowControl(enumValueFromString<QSerialPort::FlowControl>(settings.serialFlowControl()));

        r = r && sp_->open(QSerialPort::WriteOnly);
        if (!r) {
            qWarning() << "Failed to open serial port: " << port_name;
        }

        slave_address_ = settings.modbusSlaveAddress();
        data_address_ = settings.modbusDataAddress();

        return r;
    }

    bool SerialOutput::write(const FilterOutputData& data)
    {
        if (data.measurement < 0 || data.measurement > UINT16_MAX) {
            qWarning() << "Measurement value out of range";
            return false;
        }

        uint16_t values[] = {static_cast<uint16_t>(data.measurement),
                             static_cast<uint16_t>(data.status)};
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
