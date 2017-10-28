#ifndef SERIALOUTPUT_H
#define SERIALOUTPUT_H

#include "FilterOutput.h"

#include <QByteArray>

#include <memory>

class QSerialPort;

namespace sfv2 {

    class SerialOutput : public FilterOutput
    {
    public:
        SerialOutput();
        ~SerialOutput();

        SerialOutput(const SerialOutput&) = delete;
        SerialOutput& operator=(const SerialOutput&) = delete;

        SerialOutput(SerialOutput&&) = default;
        SerialOutput& operator=(SerialOutput&&) = default;

        bool open(const FilterSettings& settings) override;
        bool write(const FilterOutputData& data) override;

    private:
        std::unique_ptr<QSerialPort> sp_;

        int slave_address_;
        int data_address_;

        QByteArray buffer_;
    };

}


#endif // SERIALOUTPUT_H
