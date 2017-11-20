#include "FilterSettings.h"

#include <QSettings>
#include <QPoint>
#include <QSize>
#include <QMetaEnum>

namespace sfv2 {

    template<class _T>
    _T enumValueFromString(const QString& str)
    {
        return static_cast<_T>(
            QMetaEnum::fromType<_T>().keyToValue(str.toUtf8()));
    }

    template<class _T>
    QString enumValueToString(_T value)
    {
        return QMetaEnum::fromType<_T>().valueToKey(static_cast<int>(value));
    }

    class FilterSettings::Impl
    {
    public:
        friend class FilterSettings;
        explicit Impl(const QString& file_name);

    private:
        QSettings s_;
        bool test_mode_; // TestMode always has to be enabled explicitly
    };

    FilterSettings::FilterSettings(const QString& file_name, QObject* parent)
        : QObject(parent)
        , pimpl_(std::make_unique<Impl>(file_name))
    {

    }

    FilterSettings::~FilterSettings()
    {

    }

    FilterSettings::InputType FilterSettings::inputType() const
    {
        return enumValueFromString<InputType>(
                    pimpl_->s_.value("filterapp/input-type", "CVInput").toString());
    }

    void FilterSettings::setInputType(FilterSettings::InputType input_type)
    {
        if (inputType() != input_type) {
            pimpl_->s_.setValue("filterapp/input-type", enumValueToString(input_type));
            emit inputTypeChanged();
        }
    }

    FilterSettings::OutputType FilterSettings::outputType() const
    {
        return enumValueFromString<OutputType>(
                    pimpl_->s_.value("filterapp/output-type", "ConsoleOutput").toString());
    }

    void FilterSettings::setOutputType(FilterSettings::OutputType output_type)
    {
        if (outputType() != output_type) {
            pimpl_->s_.setValue("filterapp/output-type", enumValueToString(output_type));
            emit outputTypeChanged();
        }
    }

    QPoint FilterSettings::roiTopLeft() const
    {
        return pimpl_->s_.value("filter/roi-topleft", QPoint(0, 0)).toPoint();
    }

    QSize FilterSettings::roiSize() const
    {
        return pimpl_->s_.value("filter/roi-size", QSize(100, 100)).toSize();
    }

    void FilterSettings::setRoiTopLeft(const QPoint& point)
    {
        pimpl_->s_.setValue("filter/roi-topleft", point);
    }

    void FilterSettings::setRoiSize(const QSize& size)
    {
        pimpl_->s_.setValue("filter/roi-size", size);
    }

    bool FilterSettings::optimizeRoi() const
    {
        return pimpl_->s_.value("filter/optimize-roi", true).toBool();
    }

    void FilterSettings::setOptimizeRoi(bool optimize_roi)
    {
        pimpl_->s_.setValue("filter/optimize-roi", optimize_roi);
    }

    bool FilterSettings::useMedian() const
    {
        return pimpl_->s_.value("filter/use-median", false).toBool();
    }

    void FilterSettings::setUseMedian(bool use_median)
    {
        pimpl_->s_.setValue("filter/use-median", use_median);
    }

    int FilterSettings::medianKSize() const
    {
        return pimpl_->s_.value("filter/median-ksize", 3).toInt();
    }

    void FilterSettings::setMedianKSize(int k_size)
    {
        pimpl_->s_.setValue("filter/median-ksize", k_size);
    }

    bool FilterSettings::useGaussian() const
    {
        return pimpl_->s_.value("filter/use-gaussian", false).toBool();
    }

    void FilterSettings::setUseGaussian(bool use_gaussian)
    {
        pimpl_->s_.setValue("filter/use-gaussian", use_gaussian);
    }

    int FilterSettings::gaussianKSize() const
    {
        return pimpl_->s_.value("filter/gaussian-ksize", 3).toInt();
    }

    void FilterSettings::setGaussianKSize(int k_size)
    {
        pimpl_->s_.setValue("filter/gaussian-ksize", k_size);
    }

    double FilterSettings::gaussianSigma() const
    {
        return pimpl_->s_.value("filter/gaussian-sigma", 1.5).toDouble();
    }

    void FilterSettings::setGaussianSigma(double sigma)
    {
        pimpl_->s_.setValue("filter/gaussian-sigma", sigma);
    }

    bool FilterSettings::useBox() const
    {
        return pimpl_->s_.value("filter/use-box", false).toBool();
    }

    void FilterSettings::setUseBox(bool use_box)
    {
        pimpl_->s_.setValue("filter/use-box", use_box);
    }

    int FilterSettings::boxKSize() const
    {
        return pimpl_->s_.value("filter/box-ksize", 3).toInt();
    }

    void FilterSettings::setBoxKSize(int k_size)
    {
        pimpl_->s_.setValue("filter/box-ksize", k_size);
    }

    bool FilterSettings::useThreshold() const
    {
        return pimpl_->s_.value("filter/use-threshold", false).toBool();
    }

    void FilterSettings::setUseThreshold(bool use_threshold)
    {
        pimpl_->s_.setValue("filter/use-threshold", use_threshold);
    }

    int FilterSettings::thresholdValue() const
    {
        return pimpl_->s_.value("filter/threshold-value", 127).toInt();
    }

    void FilterSettings::setThresholdValue(int value)
    {
        pimpl_->s_.setValue("filter/threshold-value", value);
    }

    bool FilterSettings::blackObject() const
    {
        return pimpl_->s_.value("filter/black-object", true).toBool();
    }

    void FilterSettings::setBlackObject(bool black_object)
    {
        pimpl_->s_.setValue("filter/black-object", black_object);
    }

    QString FilterSettings::serialPortName() const
    {
        return pimpl_->s_.value("serial/port-name", "ttyUSB0").toString();
    }

    void FilterSettings::setSerialPortName(const QString &name)
    {
        if (serialPortName() != name) {
            pimpl_->s_.setValue("serial/port-name", name);
            emit serialPortChanged();
        }
    }

    QSerialPort::BaudRate FilterSettings::serialBaudRate() const
    {
        return enumValueFromString<QSerialPort::BaudRate>(
                    pimpl_->s_.value("serial/baud-rate", "Baud19200").toString());
    }

    void FilterSettings::setSerialBaudRate(QSerialPort::BaudRate baud_rate)
    {
        if (serialBaudRate() != baud_rate) {
            pimpl_->s_.setValue("serial/baud-rate", enumValueToString(baud_rate));
            emit serialPortChanged();
        }
    }

    QSerialPort::DataBits FilterSettings::serialDataBits() const
    {
        return enumValueFromString<QSerialPort::DataBits>(
                    pimpl_->s_.value("serial/databits", "Data8").toString());
    }

    void FilterSettings::setSerialDataBits(QSerialPort::DataBits data_bits)
    {
        if (serialDataBits() != data_bits) {
            pimpl_->s_.setValue("serial/databits", enumValueToString(data_bits));
            emit serialPortChanged();
        }
    }

    QSerialPort::Parity FilterSettings::serialParity() const
    {
        return enumValueFromString<QSerialPort::Parity>(
                    pimpl_->s_.value("serial/parity", "EvenParity").toString());
    }

    void FilterSettings::setSerialParity(QSerialPort::Parity parity)
    {
        if (serialParity() != parity) {
            pimpl_->s_.setValue("serial/parity", enumValueToString(parity));
            emit serialPortChanged();
        }
    }

    QSerialPort::StopBits FilterSettings::serialStopBits() const
    {
        return enumValueFromString<QSerialPort::StopBits>(
                    pimpl_->s_.value("serial/stopbits", "OneStop").toString());
    }

    void FilterSettings::setSerialStopBits(QSerialPort::StopBits stop_bits)
    {
        if (serialStopBits() != stop_bits) {
            pimpl_->s_.setValue("serial/stopbits", enumValueToString(stop_bits));
            emit serialPortChanged();
        }
    }

    QSerialPort::FlowControl FilterSettings::serialFlowControl() const
    {
        return enumValueFromString<QSerialPort::FlowControl>(
                    pimpl_->s_.value("serial/flow-control", "NoFlowControl").toString());
    }

    void FilterSettings::setSerialFlowControl(QSerialPort::FlowControl flow_control)
    {
        if (serialFlowControl() != flow_control) {
            pimpl_->s_.setValue("serial/flow-control", enumValueToString(flow_control));
            emit serialPortChanged();
        }
    }

    int FilterSettings::modbusSlaveAddress() const
    {
        return pimpl_->s_.value("modbus/slave-address", 1).toInt();
    }

    void FilterSettings::setModbusSlaveAddress(int address)
    {
        if (modbusSlaveAddress() != address) {
            pimpl_->s_.setValue("modbus/slave-address", address);
            emit serialPortChanged();
        }
    }

    int FilterSettings::modbusDataAddress() const
    {
        return pimpl_->s_.value("modbus/data-address", 4000).toInt();
    }

    void FilterSettings::setModbusDataAddress(int address)
    {
        if (modbusDataAddress() != address) {
            pimpl_->s_.setValue("modbus/data-address", address);
            emit serialPortChanged();
        }
    }

    bool FilterSettings::testMode() const
    {
        return pimpl_->test_mode_;
    }

    void FilterSettings::setTestMode(bool test_mode)
    {
        pimpl_->test_mode_ = test_mode;
    }

    FilterOutputData::Status FilterSettings::testFilterStatus() const
    {
        return enumValueFromString<FilterOutputData::Status>(
                    pimpl_->s_.value("test/status", "OK").toString());
    }

    void FilterSettings::setTestFilterStatus(FilterOutputData::Status status)
    {
        if (testFilterStatus() != status) {
            pimpl_->s_.setValue("test/status", enumValueToString(status));
        }
    }

    double FilterSettings::testMinMeasurement() const
    {
        return pimpl_->s_.value("test/min-measurement", 10).toDouble();
    }

    void FilterSettings::setTestMinMeasurement(double measurement)
    {
        pimpl_->s_.setValue("test/min-measurement", measurement);
    }

    double FilterSettings::testMaxMeasurement() const
    {
        return pimpl_->s_.value("test/max-measurement", 300).toDouble();
    }

    void FilterSettings::setTestMaxMeasurement(double measurement)
    {
        pimpl_->s_.setValue("test/max-measurement", measurement);
    }

    double FilterSettings::testMeasurementStep() const
    {
        return pimpl_->s_.value("test/measurement-step", 10).toDouble();
    }

    void FilterSettings::setTestMeasurementStep(double val_step)
    {
        pimpl_->s_.setValue("test/measurement-step", val_step);
    }

    int FilterSettings::testTimeStep() const
    {
        return pimpl_->s_.value("test/time-step", 1000).toInt();
    }

    void FilterSettings::setTestTimeStep(int time_step)
    {
        pimpl_->s_.setValue("test/time-step", time_step);
    }

    int FilterSettings::numEntropySamples() const
    {
        return pimpl_->s_.value("entropy/num-samples", 10).toInt();
    }

    void FilterSettings::setNumEntropySamples(int num)
    {
        pimpl_->s_.setValue("entropy/num-samples", num);
    }

    double FilterSettings::entropyScaleFactor() const
    {
        return pimpl_->s_.value("entropy/scale-factor", 50).toDouble();
    }

    void FilterSettings::setEntropyScaleFactor(double scale_factor)
    {
        pimpl_->s_.setValue("entropy/scale-factor", scale_factor);
    }

    FilterSettings::Impl::Impl(const QString& file_name)
        : s_(file_name, QSettings::IniFormat)
        , test_mode_(false)
    {

    }

}

