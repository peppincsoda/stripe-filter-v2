#ifndef FILTERSETTINGS_H
#define FILTERSETTINGS_H

#include <QMetaEnum>

#include <memory>

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

    class FilterSettings
    {
    public:
        explicit FilterSettings(const QString& file_name);
        ~FilterSettings();

        FilterSettings(const FilterSettings&) = delete;
        FilterSettings& operator=(const FilterSettings&) = delete;

        FilterSettings(FilterSettings&&) = default;
        FilterSettings& operator=(FilterSettings&&) = default;

        QString inputType() const;
        QString outputType() const;

        QPoint roiTopLeft() const;
        QSize roiSize() const;
        void setRoiTopLeft(const QPoint& point);
        void setRoiSize(const QSize& size);

        bool useMedian() const;
        int medianKSize() const;

        bool useGaussian() const;
        int gaussianKSize() const;
        double gaussianSigma() const;

        bool useBox() const;
        int boxKSize() const;

        bool useThreshold() const;
        int thresholdValue() const;

        QString serialPortName() const;
        int serialBaudRate() const;
        int serialDataBits() const;
        QString serialParity() const;
        int serialStopBits() const;
        QString serialFlowControl() const;

        int modbusSlaveAddress() const;
        int modbusDataAddress() const;

    private:
        class Impl;
        std::unique_ptr<Impl> pimpl_;
    };

}

#endif // FILTERSETTINGS_H
