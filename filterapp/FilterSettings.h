#ifndef FILTERSETTINGS_H
#define FILTERSETTINGS_H

#include <QPoint>
#include <QSize>
#include <QSerialPort>

#include <memory>

namespace sfv2 {

    class FilterSettings : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(InputType InputType READ inputType WRITE setInputType NOTIFY inputTypeChanged)
        Q_PROPERTY(OutputType OutputType READ outputType WRITE setOutputType NOTIFY outputTypeChanged)
        Q_PROPERTY(QPoint RoiTopLeft READ roiTopLeft)
        Q_PROPERTY(QSize RoiSize READ roiSize)
        Q_PROPERTY(bool OptimizeRoi READ optimizeRoi WRITE setOptimizeRoi)
        Q_PROPERTY(bool UseMedian READ useMedian WRITE setUseMedian)
        Q_PROPERTY(int MedianKSize READ medianKSize WRITE setMedianKSize)
        Q_PROPERTY(bool UseGaussian READ useGaussian WRITE setUseGaussian)
        Q_PROPERTY(int GaussianKSize READ gaussianKSize WRITE setGaussianKSize)
        Q_PROPERTY(double GaussianSigma READ gaussianSigma WRITE setGaussianSigma)
        Q_PROPERTY(bool UseBox READ useBox WRITE setUseBox)
        Q_PROPERTY(int BoxKSize READ boxKSize WRITE setBoxKSize)
        Q_PROPERTY(bool UseThreshold READ useThreshold WRITE setUseThreshold)
        Q_PROPERTY(int ThresholdValue READ thresholdValue WRITE setThresholdValue)
        Q_PROPERTY(bool BlackObject READ blackObject WRITE setBlackObject)
        Q_PROPERTY(QString SerialPortName READ serialPortName WRITE setSerialPortName)
        Q_PROPERTY(QSerialPort::BaudRate SerialBaudRate READ serialBaudRate WRITE setSerialBaudRate)
        Q_PROPERTY(QSerialPort::DataBits SerialDataBits READ serialDataBits WRITE setSerialDataBits)
        Q_PROPERTY(QSerialPort::Parity SerialParity READ serialParity WRITE setSerialParity)
        Q_PROPERTY(QSerialPort::StopBits SerialStopBits READ serialStopBits WRITE setSerialStopBits)
        Q_PROPERTY(QSerialPort::FlowControl SerialFlowControl READ serialFlowControl WRITE setSerialFlowControl)
        Q_PROPERTY(int ModbusSlaveAddress READ modbusSlaveAddress WRITE setModbusSlaveAddress)
        Q_PROPERTY(int ModbusDataAddress READ modbusDataAddress WRITE setModbusDataAddress)
    public:
        enum InputType
        {
            CVInput,
            BaslerInput,
            UnknownInput = -1,
        };
        Q_ENUM(InputType)

        enum OutputType
        {
            ConsoleOutput,
            SerialOutput,
            UnknownOutput = -1,
        };
        Q_ENUM(OutputType)

        explicit FilterSettings(const QString& file_name, QObject* parent = nullptr);
        ~FilterSettings();

        FilterSettings(const FilterSettings&) = delete;
        FilterSettings& operator=(const FilterSettings&) = delete;

        FilterSettings(FilterSettings&&) = default;
        FilterSettings& operator=(FilterSettings&&) = default;

        InputType inputType() const;
        void setInputType(InputType input_type);

        OutputType outputType() const;
        void setOutputType(OutputType output_type);

        QPoint roiTopLeft() const;
        QSize roiSize() const;
        void setRoiTopLeft(const QPoint& point);
        void setRoiSize(const QSize& size);

        bool optimizeRoi() const;
        void setOptimizeRoi(bool optimize_roi);

        bool useMedian() const;
        void setUseMedian(bool use_median);

        int medianKSize() const;
        void setMedianKSize(int k_size);

        bool useGaussian() const;
        void setUseGaussian(bool use_gaussian);

        int gaussianKSize() const;
        void setGaussianKSize(int k_size);

        double gaussianSigma() const;
        void setGaussianSigma(double sigma);

        bool useBox() const;
        void setUseBox(bool use_box);

        int boxKSize() const;
        void setBoxKSize(int k_size);

        bool useThreshold() const;
        void setUseThreshold(bool use_threshold);

        int thresholdValue() const;
        void setThresholdValue(int value);

        bool blackObject() const;
        void setBlackObject(bool black_object);

        QString serialPortName() const;
        void setSerialPortName(const QString& name);

        QSerialPort::BaudRate serialBaudRate() const;
        void setSerialBaudRate(QSerialPort::BaudRate baud_rate);

        QSerialPort::DataBits serialDataBits() const;
        void setSerialDataBits(QSerialPort::DataBits data_bits);

        QSerialPort::Parity serialParity() const;
        void setSerialParity(QSerialPort::Parity parity);

        QSerialPort::StopBits serialStopBits() const;
        void setSerialStopBits(QSerialPort::StopBits stop_bits);

        QSerialPort::FlowControl serialFlowControl() const;
        void setSerialFlowControl(QSerialPort::FlowControl flow_control);

        int modbusSlaveAddress() const;
        void setModbusSlaveAddress(int address);

        int modbusDataAddress() const;
        void setModbusDataAddress(int address);

    Q_SIGNALS:
        void inputTypeChanged();
        void outputTypeChanged();
        void serialPortChanged();

    private:
        class Impl;
        std::unique_ptr<Impl> pimpl_;
    };

}

#endif // FILTERSETTINGS_H
