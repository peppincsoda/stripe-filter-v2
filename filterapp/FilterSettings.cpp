#include "FilterSettings.h"

#include <QSettings>
#include <QPoint>
#include <QSize>

namespace sfv2 {

    class FilterSettings::Impl
    {
    public:
        friend class FilterSettings;
        explicit Impl(const QString& file_name);

    private:
        QSettings s_;
    };

    FilterSettings::FilterSettings(const QString& file_name)
        : pimpl_(std::make_unique<Impl>(file_name))
    {

    }

    FilterSettings::~FilterSettings()
    {

    }

    QString FilterSettings::inputType() const
    {
        return pimpl_->s_.value("filterapp/input-type", "CVInput").toString();
    }

    QString FilterSettings::outputType() const
    {
        return pimpl_->s_.value("filterapp/output-type", "ConsoleOutput").toString();
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

    bool FilterSettings::useMedian() const
    {
        return pimpl_->s_.value("filter/use-median", false).toBool();
    }

    int FilterSettings::medianKSize() const
    {
        return pimpl_->s_.value("filter/median-ksize", 3).toInt();
    }

    bool FilterSettings::useGaussian() const
    {
        return pimpl_->s_.value("filter/use-gaussian", false).toBool();
    }

    int FilterSettings::gaussianKSize() const
    {
        return pimpl_->s_.value("filter/gaussian-ksize", 3).toInt();
    }

    double FilterSettings::gaussianSigma() const
    {
        return pimpl_->s_.value("filter/gaussian-sigma", 1.5).toDouble();
    }

    bool FilterSettings::useBox() const
    {
        return pimpl_->s_.value("filter/use-box", false).toBool();
    }

    int FilterSettings::boxKSize() const
    {
        return pimpl_->s_.value("filter/box-ksize", 3).toInt();
    }

    bool FilterSettings::useThreshold() const
    {
        return pimpl_->s_.value("filter/use-threshold", false).toBool();
    }

    int FilterSettings::thresholdValue() const
    {
        return pimpl_->s_.value("filter/threshold-value", 127).toInt();
    }

    QString FilterSettings::serialPortName() const
    {
        return pimpl_->s_.value("serial/port-name", "ttyUSB0").toString();
    }

    int FilterSettings::serialBaudRate() const
    {
        return pimpl_->s_.value("serial/port-name", 38400).toInt();
    }

    int FilterSettings::serialDataBits() const
    {
        return pimpl_->s_.value("serial/databits", 8).toInt();
    }

    QString FilterSettings::serialParity() const
    {
        return pimpl_->s_.value("serial/parity", "EvenParity").toString();
    }

    int FilterSettings::serialStopBits() const
    {
        return pimpl_->s_.value("serial/stopbits", 1).toInt();
    }

    QString FilterSettings::serialFlowControl() const
    {
        return pimpl_->s_.value("serial/flow-control", "NoFlowControl").toString();
    }

    int FilterSettings::modbusSlaveAddress() const
    {
        return pimpl_->s_.value("modbus/slave-address", 1).toInt();
    }

    int FilterSettings::modbusDataAddress() const
    {
        return pimpl_->s_.value("modbus/data-address", 4000).toInt();
    }

    FilterSettings::Impl::Impl(const QString& file_name)
        : s_(file_name, QSettings::IniFormat)
    {

    }



}

