#ifndef OUTPUTDATAFORM_H
#define OUTPUTDATAFORM_H

#include <QWidget>

namespace Ui {
class OutputDataForm;
}

namespace sfv2 {

    class FilterOutputData;

    class OutputDataForm : public QWidget
    {
        Q_OBJECT

    public:
        explicit OutputDataForm(QWidget *parent = nullptr);
        ~OutputDataForm();

        void setOutputData(const FilterOutputData& output_data);

    private:
        Ui::OutputDataForm *ui;
    };

}

#endif // OUTPUTDATAFORM_H
