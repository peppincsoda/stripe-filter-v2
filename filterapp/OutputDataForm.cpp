#include "OutputDataForm.h"
#include "ui_OutputDataForm.h"
#include "FilterOutput.h"

#include <QMetaEnum>

namespace sfv2 {

    OutputDataForm::OutputDataForm(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::OutputDataForm)
    {
        ui->setupUi(this);
    }

    OutputDataForm::~OutputDataForm()
    {
        delete ui;
    }

    void OutputDataForm::setOutputData(const FilterOutputData &output_data)
    {
        ui->leftLabel->setText(QString::number(output_data.leftDist()));
        ui->rightLabel->setText(QString::number(output_data.rightDist()));
        ui->measurementLabel->setText(QString::number(output_data.measurement()));
        ui->entropyLabel->setText(QString::number(output_data.entropy()));
        ui->statusLabel->setText(QMetaEnum::fromType<decltype(output_data.status())>()
                                    .valueToKey(static_cast<int>(output_data.status())));
    }


}
