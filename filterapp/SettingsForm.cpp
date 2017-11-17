#include "SettingsForm.h"
#include "ui_SettingsForm.h"
#include "FilterSettings.h"

#include <QMetaEnum>
#include <QPixmap>
#include <QPainter>

#include <cassert>

namespace sfv2 {

    template<class _T>
    static void addEnumValues(QComboBox* cb)
    {
        const auto& meta_enum = QMetaEnum::fromType<_T>();
        for (int i = 0; i < meta_enum.keyCount(); i++) {
            cb->addItem(meta_enum.key(i), meta_enum.value(i));
        }
    }

    static void setCurrentIndexFromData(QComboBox* cb, const QVariant& data)
    {
        cb->setCurrentIndex(cb->findData(data));
    }

    static void addKSizeValues(QComboBox* cb)
    {
        for (int k_size = 3; k_size <= 13; k_size += 2) {
            cb->addItem(QString::number(k_size), k_size);
        }
    }

    class SettingsForm::Impl : public QWidget
    {
        Q_OBJECT
    public:
        friend class SettingsForm;

        explicit Impl(FilterSettings* settings, QWidget *parent = nullptr);
        ~Impl();

    private Q_SLOTS:
        void onInputTypeChanged();
        void onOutputTypeChanged();
        void onOptimizeRoiChanged();
        void onUseMedianChanged();
        void onMedianKSizeChanged();
        void onUseGaussianChanged();
        void onGaussianKSizeChanged();
        void onGaussianSigmaChanged();
        void onUseBoxChanged();
        void onBoxKSizeChanged();
        void onUseThresholdChanged();
        void onThresholdValueChanged();
        void onBlackObjectChanged();
        void onPortNameChanged();
        void onBaudRateChanged();
        void onDataBitsChanged();
        void onParityChanged();
        void onStopBitsChanged();
        void onFlowControlChanged();
        void onModbusSlaveAddressChanged();
        void onModbusDataAddressChanged();

    private:
        Ui::SettingsForm *ui;
        FilterSettings* settings_;
    };



    SettingsForm::SettingsForm(FilterSettings* settings, QWidget *parent, Qt::WindowFlags flags)
        : QWidget(parent, flags)
        , pimpl_(new Impl(settings, this))
    {
        auto* layout = new QHBoxLayout(this);
        layout->addWidget(pimpl_);
    }

    SettingsForm::~SettingsForm()
    {

    }

    int SettingsForm::histDisplayHeight() const
    {
        return pimpl_->ui->histogramLabel->height();
    }

    void SettingsForm::setHistImage(QImage&& qimg)
    {
        auto pixmap = QPixmap::fromImage(std::move(qimg));

        QPainter p(&pixmap);
        p.setPen(Qt::yellow);

        const auto x = pimpl_->settings_->thresholdValue();
        p.drawLine(x, 0, x, pixmap.height());
        p.end();

        pimpl_->ui->histogramLabel->setPixmap(std::move(pixmap));
    }

    SettingsForm::Impl::Impl(FilterSettings* settings, QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::SettingsForm)
        , settings_(settings)
    {
        assert(settings != nullptr);

        ui->setupUi(this);

        // Input/Output
        addEnumValues<FilterSettings::InputType>(ui->inputTypeComboBox);
        setCurrentIndexFromData(ui->inputTypeComboBox, settings_->inputType());
        connect(ui->inputTypeComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onInputTypeChanged()));

        addEnumValues<FilterSettings::OutputType>(ui->outputTypeComboBox);
        setCurrentIndexFromData(ui->outputTypeComboBox, settings_->outputType());
        connect(ui->outputTypeComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onOutputTypeChanged()));

        ui->optimizeRoiCheckBox->setChecked(settings_->optimizeRoi());
        connect(ui->optimizeRoiCheckBox, SIGNAL(toggled(bool)),
                this, SLOT(onOptimizeRoiChanged()));

        // Median
        ui->useMedianCheckBox->setChecked(settings_->useMedian());
        connect(ui->useMedianCheckBox, SIGNAL(toggled(bool)),
                this, SLOT(onUseMedianChanged()));

        addKSizeValues(ui->medianKSizeComboBox);
        setCurrentIndexFromData(ui->medianKSizeComboBox, settings_->medianKSize());
        connect(ui->medianKSizeComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onMedianKSizeChanged()));

        // Gaussian
        ui->useGaussianCheckBox->setChecked(settings_->useGaussian());
        connect(ui->useGaussianCheckBox, SIGNAL(toggled(bool)),
                this, SLOT(onUseGaussianChanged()));

        addKSizeValues(ui->gaussianKSizeComboBox);
        setCurrentIndexFromData(ui->gaussianKSizeComboBox, settings_->gaussianKSize());
        connect(ui->gaussianKSizeComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onGaussianKSizeChanged()));

        ui->gaussianSigmaDoubleSpinBox->setValue(settings_->gaussianSigma());
        connect(ui->gaussianSigmaDoubleSpinBox, SIGNAL(valueChanged(double)),
                this, SLOT(onGaussianSigmaChanged()));

        // Box
        ui->useBoxCheckBox->setChecked(settings_->useBox());
        connect(ui->useBoxCheckBox, SIGNAL(toggled(bool)),
                this, SLOT(onUseBoxChanged()));

        addKSizeValues(ui->boxKSizeComboBox);
        setCurrentIndexFromData(ui->boxKSizeComboBox, settings_->boxKSize());
        connect(ui->boxKSizeComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onBoxKSizeChanged()));

        // Threshold
        ui->useThresholdCheckBox->setChecked(settings_->useThreshold());
        connect(ui->useThresholdCheckBox, SIGNAL(toggled(bool)),
                this, SLOT(onUseThresholdChanged()));

        ui->thresholdValueSlider->setValue(settings_->thresholdValue());
        connect(ui->thresholdValueSlider, SIGNAL(valueChanged(int)),
                this, SLOT(onThresholdValueChanged()));

        ui->blackObjectCheckBox->setChecked(settings_->blackObject());
        connect(ui->blackObjectCheckBox, SIGNAL(toggled(bool)),
                this, SLOT(onBlackObjectChanged()));

        // Serial port
        ui->portNameLineEdit->setText(settings_->serialPortName());
        connect(ui->portNameLineEdit, SIGNAL(editingFinished()),
                this, SLOT(onPortNameChanged()));

        addEnumValues<QSerialPort::BaudRate>(ui->baudRateComboBox);
        setCurrentIndexFromData(ui->baudRateComboBox, settings_->serialBaudRate());
        connect(ui->baudRateComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onBaudRateChanged()));

        addEnumValues<QSerialPort::DataBits>(ui->dataBitsComboBox);
        setCurrentIndexFromData(ui->dataBitsComboBox, settings_->serialDataBits());
        connect(ui->dataBitsComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onDataBitsChanged()));

        addEnumValues<QSerialPort::Parity>(ui->parityComboBox);
        setCurrentIndexFromData(ui->parityComboBox, settings_->serialParity());
        connect(ui->parityComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onParityChanged()));

        addEnumValues<QSerialPort::StopBits>(ui->stopBitsComboBox);
        setCurrentIndexFromData(ui->stopBitsComboBox, settings_->serialStopBits());
        connect(ui->stopBitsComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onStopBitsChanged()));

        addEnumValues<QSerialPort::FlowControl>(ui->flowControlComboBox);
        setCurrentIndexFromData(ui->flowControlComboBox, settings_->serialFlowControl());
        connect(ui->flowControlComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onFlowControlChanged()));

        // Modbus
        ui->modbusSlaveAddressSpinBox->setValue(settings_->modbusSlaveAddress());
        connect(ui->modbusSlaveAddressSpinBox, SIGNAL(valueChanged(int)),
                this, SLOT(onModbusSlaveAddressChanged()));

        ui->modbusDataAddressSpinBox->setValue(settings_->modbusDataAddress());
        connect(ui->modbusDataAddressSpinBox, SIGNAL(valueChanged(int)),
                this, SLOT(onModbusDataAddressChanged()));
    }

    SettingsForm::Impl::~Impl()
    {
        delete ui;
    }

    void SettingsForm::Impl::onInputTypeChanged()
    {
        settings_->setInputType(static_cast<FilterSettings::InputType>(ui->inputTypeComboBox->currentData().toInt()));
    }

    void SettingsForm::Impl::onOutputTypeChanged()
    {
        settings_->setOutputType(static_cast<FilterSettings::OutputType>(ui->outputTypeComboBox->currentData().toInt()));
    }

    void SettingsForm::Impl::onOptimizeRoiChanged()
    {
        settings_->setOptimizeRoi(ui->optimizeRoiCheckBox->isChecked());
    }

    void SettingsForm::Impl::onUseMedianChanged()
    {
        settings_->setUseMedian(ui->useMedianCheckBox->isChecked());
    }

    void SettingsForm::Impl::onMedianKSizeChanged()
    {
        settings_->setMedianKSize(ui->medianKSizeComboBox->currentData().toInt());
    }

    void SettingsForm::Impl::onUseGaussianChanged()
    {
        settings_->setUseGaussian(ui->useGaussianCheckBox->isChecked());
    }

    void SettingsForm::Impl::onGaussianKSizeChanged()
    {
        settings_->setGaussianKSize(ui->gaussianKSizeComboBox->currentData().toInt());
    }

    void SettingsForm::Impl::onGaussianSigmaChanged()
    {
        settings_->setGaussianSigma(ui->gaussianSigmaDoubleSpinBox->value());
    }

    void SettingsForm::Impl::onUseBoxChanged()
    {
        settings_->setUseBox(ui->useBoxCheckBox->isChecked());
    }

    void SettingsForm::Impl::onBoxKSizeChanged()
    {
        settings_->setBoxKSize(ui->boxKSizeComboBox->currentData().toInt());
    }

    void SettingsForm::Impl::onUseThresholdChanged()
    {
        settings_->setUseThreshold(ui->useThresholdCheckBox->isChecked());
    }

    void SettingsForm::Impl::onThresholdValueChanged()
    {
        settings_->setThresholdValue(ui->thresholdValueSlider->value());
    }

    void SettingsForm::Impl::onBlackObjectChanged()
    {
        settings_->setBlackObject(ui->blackObjectCheckBox->isChecked());
    }

    void SettingsForm::Impl::onPortNameChanged()
    {
        settings_->setSerialPortName(ui->portNameLineEdit->text());
    }

    void SettingsForm::Impl::onBaudRateChanged()
    {
        settings_->setSerialBaudRate(static_cast<QSerialPort::BaudRate>(ui->baudRateComboBox->currentData().toInt()));
    }

    void SettingsForm::Impl::onDataBitsChanged()
    {
        settings_->setSerialDataBits(static_cast<QSerialPort::DataBits>(ui->dataBitsComboBox->currentData().toInt()));
    }

    void SettingsForm::Impl::onParityChanged()
    {
        settings_->setSerialParity(static_cast<QSerialPort::Parity>(ui->parityComboBox->currentData().toInt()));
    }

    void SettingsForm::Impl::onStopBitsChanged()
    {
        settings_->setSerialStopBits(static_cast<QSerialPort::StopBits>(ui->stopBitsComboBox->currentData().toInt()));
    }

    void SettingsForm::Impl::onFlowControlChanged()
    {
        settings_->setSerialFlowControl(static_cast<QSerialPort::FlowControl>(ui->flowControlComboBox->currentData().toInt()));
    }

    void SettingsForm::Impl::onModbusSlaveAddressChanged()
    {
        settings_->setModbusSlaveAddress(ui->modbusSlaveAddressSpinBox->value());
    }

    void SettingsForm::Impl::onModbusDataAddressChanged()
    {
        settings_->setModbusDataAddress(ui->modbusDataAddressSpinBox->value());
    }

}

#include "SettingsForm.moc"
