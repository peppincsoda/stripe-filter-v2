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

        explicit Impl(QWidget *parent);
        ~Impl();

        void setSettings(FilterSettings* settings);

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
        void onTestModeChanged();
        void onTestFilterStatusChanged();
        void onMinValueChanged();
        void onMaxValueChanged();
        void onValueStepChanged();
        void onTimeStepChanged();

    private:
        Ui::SettingsForm *ui;
        FilterSettings* settings_;
    };



    SettingsForm::SettingsForm(QWidget *parent, Qt::WindowFlags flags)
        : QWidget(parent, flags)
        , pimpl_(new Impl(this))
    {
        auto* layout = new QHBoxLayout(this);
        layout->addWidget(pimpl_);
    }

    SettingsForm::~SettingsForm()
    {

    }

    void SettingsForm::setSettings(FilterSettings *settings)
    {
        pimpl_->setSettings(settings);
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

    SettingsForm::Impl::Impl(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::SettingsForm)
        , settings_(nullptr)
    {
        assert(parent != nullptr);

        ui->setupUi(this);

        // Input/Output
        addEnumValues<FilterSettings::InputType>(ui->inputTypeComboBox);
        connect(ui->inputTypeComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onInputTypeChanged()));
        addEnumValues<FilterSettings::OutputType>(ui->outputTypeComboBox);
        connect(ui->outputTypeComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onOutputTypeChanged()));
        connect(ui->optimizeRoiCheckBox, SIGNAL(toggled(bool)),
                this, SLOT(onOptimizeRoiChanged()));

        // Median
        connect(ui->useMedianCheckBox, SIGNAL(toggled(bool)),
                this, SLOT(onUseMedianChanged()));
        addKSizeValues(ui->medianKSizeComboBox);
        connect(ui->medianKSizeComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onMedianKSizeChanged()));

        // Gaussian
        connect(ui->useGaussianCheckBox, SIGNAL(toggled(bool)),
                this, SLOT(onUseGaussianChanged()));
        addKSizeValues(ui->gaussianKSizeComboBox);
        connect(ui->gaussianKSizeComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onGaussianKSizeChanged()));
        connect(ui->gaussianSigmaDoubleSpinBox, SIGNAL(valueChanged(double)),
                this, SLOT(onGaussianSigmaChanged()));

        // Box
        connect(ui->useBoxCheckBox, SIGNAL(toggled(bool)),
                this, SLOT(onUseBoxChanged()));
        addKSizeValues(ui->boxKSizeComboBox);
        connect(ui->boxKSizeComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onBoxKSizeChanged()));

        // Threshold
        connect(ui->useThresholdCheckBox, SIGNAL(toggled(bool)),
                this, SLOT(onUseThresholdChanged()));
        connect(ui->thresholdValueSlider, SIGNAL(valueChanged(int)),
                this, SLOT(onThresholdValueChanged()));
        connect(ui->blackObjectCheckBox, SIGNAL(toggled(bool)),
                this, SLOT(onBlackObjectChanged()));

        // Serial port
        connect(ui->portNameLineEdit, SIGNAL(editingFinished()),
                this, SLOT(onPortNameChanged()));
        addEnumValues<QSerialPort::BaudRate>(ui->baudRateComboBox);
        connect(ui->baudRateComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onBaudRateChanged()));
        addEnumValues<QSerialPort::DataBits>(ui->dataBitsComboBox);
        connect(ui->dataBitsComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onDataBitsChanged()));
        addEnumValues<QSerialPort::Parity>(ui->parityComboBox);
        connect(ui->parityComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onParityChanged()));
        addEnumValues<QSerialPort::StopBits>(ui->stopBitsComboBox);
        connect(ui->stopBitsComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onStopBitsChanged()));
        addEnumValues<QSerialPort::FlowControl>(ui->flowControlComboBox);
        connect(ui->flowControlComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onFlowControlChanged()));

        // Modbus
        connect(ui->modbusSlaveAddressSpinBox, SIGNAL(valueChanged(int)),
                this, SLOT(onModbusSlaveAddressChanged()));
        connect(ui->modbusDataAddressSpinBox, SIGNAL(valueChanged(int)),
                this, SLOT(onModbusDataAddressChanged()));

        // Test
        connect(ui->testModeCheckBox, SIGNAL(toggled(bool)),
                this, SLOT(onTestModeChanged()));
        addEnumValues<FilterOutputData::Status>(ui->testFilterStatusComboBox);
        connect(ui->testFilterStatusComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(onTestFilterStatusChanged()));
        connect(ui->minValueDoubleSpinBox, SIGNAL(valueChanged(double)),
                this, SLOT(onMinValueChanged()));
        connect(ui->maxValueDoubleSpinBox, SIGNAL(valueChanged(double)),
                this, SLOT(onMaxValueChanged()));
        connect(ui->valueStepDoubleSpinBox, SIGNAL(valueChanged(double)),
                this, SLOT(onValueStepChanged()));
        connect(ui->timeStepSpinBox, SIGNAL(valueChanged(int)),
                this, SLOT(onTimeStepChanged()));
        connect(ui->resetPushButton, SIGNAL(clicked(bool)),
                parent, SIGNAL(testResetClicked(bool)));
    }

    SettingsForm::Impl::~Impl()
    {
        delete ui;
    }

    void SettingsForm::Impl::setSettings(FilterSettings *settings)
    {
        settings_ = settings;

        if (settings_ != nullptr) {
            setCurrentIndexFromData(ui->inputTypeComboBox, settings_->inputType());
            setCurrentIndexFromData(ui->outputTypeComboBox, settings_->outputType());

            ui->optimizeRoiCheckBox->setChecked(settings_->optimizeRoi());
            ui->useMedianCheckBox->setChecked(settings_->useMedian());
            setCurrentIndexFromData(ui->medianKSizeComboBox, settings_->medianKSize());
            ui->useGaussianCheckBox->setChecked(settings_->useGaussian());
            setCurrentIndexFromData(ui->gaussianKSizeComboBox, settings_->gaussianKSize());
            ui->gaussianSigmaDoubleSpinBox->setValue(settings_->gaussianSigma());
            ui->useBoxCheckBox->setChecked(settings_->useBox());
            setCurrentIndexFromData(ui->boxKSizeComboBox, settings_->boxKSize());
            ui->useThresholdCheckBox->setChecked(settings_->useThreshold());
            ui->thresholdValueSlider->setValue(settings_->thresholdValue());
            ui->blackObjectCheckBox->setChecked(settings_->blackObject());

            ui->portNameLineEdit->setText(settings_->serialPortName());
            setCurrentIndexFromData(ui->baudRateComboBox, settings_->serialBaudRate());
            setCurrentIndexFromData(ui->dataBitsComboBox, settings_->serialDataBits());
            setCurrentIndexFromData(ui->parityComboBox, settings_->serialParity());
            setCurrentIndexFromData(ui->stopBitsComboBox, settings_->serialStopBits());
            setCurrentIndexFromData(ui->flowControlComboBox, settings_->serialFlowControl());

            ui->modbusSlaveAddressSpinBox->setValue(settings_->modbusSlaveAddress());
            ui->modbusDataAddressSpinBox->setValue(settings_->modbusDataAddress());

            ui->testModeCheckBox->setChecked(settings_->testMode());
            setCurrentIndexFromData(ui->testFilterStatusComboBox, settings_->testFilterStatus());
            ui->minValueDoubleSpinBox->setValue(settings_->testMinMeasurement());
            ui->maxValueDoubleSpinBox->setValue(settings_->testMaxMeasurement());
            ui->valueStepDoubleSpinBox->setValue(settings_->testMeasurementStep());
            ui->timeStepSpinBox->setValue(settings_->testTimeStep());
        }
    }

    void SettingsForm::Impl::onInputTypeChanged()
    {
        if (settings_ != nullptr)
            settings_->setInputType(static_cast<FilterSettings::InputType>(ui->inputTypeComboBox->currentData().toInt()));
    }

    void SettingsForm::Impl::onOutputTypeChanged()
    {
        if (settings_ != nullptr)
            settings_->setOutputType(static_cast<FilterSettings::OutputType>(ui->outputTypeComboBox->currentData().toInt()));
    }

    void SettingsForm::Impl::onOptimizeRoiChanged()
    {
        if (settings_ != nullptr)
            settings_->setOptimizeRoi(ui->optimizeRoiCheckBox->isChecked());
    }

    void SettingsForm::Impl::onUseMedianChanged()
    {
        if (settings_ != nullptr)
            settings_->setUseMedian(ui->useMedianCheckBox->isChecked());
    }

    void SettingsForm::Impl::onMedianKSizeChanged()
    {
        if (settings_ != nullptr)
            settings_->setMedianKSize(ui->medianKSizeComboBox->currentData().toInt());
    }

    void SettingsForm::Impl::onUseGaussianChanged()
    {
        if (settings_ != nullptr)
            settings_->setUseGaussian(ui->useGaussianCheckBox->isChecked());
    }

    void SettingsForm::Impl::onGaussianKSizeChanged()
    {
        if (settings_ != nullptr)
            settings_->setGaussianKSize(ui->gaussianKSizeComboBox->currentData().toInt());
    }

    void SettingsForm::Impl::onGaussianSigmaChanged()
    {
        if (settings_ != nullptr)
            settings_->setGaussianSigma(ui->gaussianSigmaDoubleSpinBox->value());
    }

    void SettingsForm::Impl::onUseBoxChanged()
    {
        if (settings_ != nullptr)
            settings_->setUseBox(ui->useBoxCheckBox->isChecked());
    }

    void SettingsForm::Impl::onBoxKSizeChanged()
    {
        if (settings_ != nullptr)
            settings_->setBoxKSize(ui->boxKSizeComboBox->currentData().toInt());
    }

    void SettingsForm::Impl::onUseThresholdChanged()
    {
        if (settings_ != nullptr)
            settings_->setUseThreshold(ui->useThresholdCheckBox->isChecked());
    }

    void SettingsForm::Impl::onThresholdValueChanged()
    {
        if (settings_ != nullptr)
            settings_->setThresholdValue(ui->thresholdValueSlider->value());
    }

    void SettingsForm::Impl::onBlackObjectChanged()
    {
        if (settings_ != nullptr)
            settings_->setBlackObject(ui->blackObjectCheckBox->isChecked());
    }

    void SettingsForm::Impl::onPortNameChanged()
    {
        if (settings_ != nullptr)
            settings_->setSerialPortName(ui->portNameLineEdit->text());
    }

    void SettingsForm::Impl::onBaudRateChanged()
    {
        if (settings_ != nullptr)
            settings_->setSerialBaudRate(static_cast<QSerialPort::BaudRate>(ui->baudRateComboBox->currentData().toInt()));
    }

    void SettingsForm::Impl::onDataBitsChanged()
    {
        if (settings_ != nullptr)
            settings_->setSerialDataBits(static_cast<QSerialPort::DataBits>(ui->dataBitsComboBox->currentData().toInt()));
    }

    void SettingsForm::Impl::onParityChanged()
    {
        if (settings_ != nullptr)
            settings_->setSerialParity(static_cast<QSerialPort::Parity>(ui->parityComboBox->currentData().toInt()));
    }

    void SettingsForm::Impl::onStopBitsChanged()
    {
        if (settings_ != nullptr)
            settings_->setSerialStopBits(static_cast<QSerialPort::StopBits>(ui->stopBitsComboBox->currentData().toInt()));
    }

    void SettingsForm::Impl::onFlowControlChanged()
    {
        if (settings_ != nullptr)
            settings_->setSerialFlowControl(static_cast<QSerialPort::FlowControl>(ui->flowControlComboBox->currentData().toInt()));
    }

    void SettingsForm::Impl::onModbusSlaveAddressChanged()
    {
        if (settings_ != nullptr)
            settings_->setModbusSlaveAddress(ui->modbusSlaveAddressSpinBox->value());
    }

    void SettingsForm::Impl::onModbusDataAddressChanged()
    {
        if (settings_ != nullptr)
            settings_->setModbusDataAddress(ui->modbusDataAddressSpinBox->value());
    }

    void SettingsForm::Impl::onTestModeChanged()
    {
        if (settings_ != nullptr)
            settings_->setTestMode(ui->testModeCheckBox->isChecked());
    }

    void SettingsForm::Impl::onTestFilterStatusChanged()
    {
        if (settings_ != nullptr)
            settings_->setTestFilterStatus(static_cast<FilterOutputData::Status>(ui->testFilterStatusComboBox->currentData().toInt()));
    }

    void SettingsForm::Impl::onMinValueChanged()
    {
        if (settings_ != nullptr)
            settings_->setTestMinMeasurement(ui->minValueDoubleSpinBox->value());
    }

    void SettingsForm::Impl::onMaxValueChanged()
    {
        if (settings_ != nullptr)
            settings_->setTestMaxMeasurement(ui->maxValueDoubleSpinBox->value());
    }

    void SettingsForm::Impl::onValueStepChanged()
    {
        if (settings_ != nullptr)
            settings_->setTestMeasurementStep(ui->valueStepDoubleSpinBox->value());
    }

    void SettingsForm::Impl::onTimeStepChanged()
    {
        if (settings_ != nullptr)
            settings_->setTestTimeStep(ui->timeStepSpinBox->value());
    }

}

#include "SettingsForm.moc"
