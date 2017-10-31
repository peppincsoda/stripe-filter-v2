#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>

class QImage;

namespace sfv2 {

    class FilterSettings;

    class SettingsForm : public QWidget
    {
        Q_OBJECT

    public:
        explicit SettingsForm(FilterSettings* settings, QWidget *parent = nullptr);
        ~SettingsForm();

        int histDisplayHeight() const;
        void setHistImage(QImage&& qimg);

    private:
        class Impl;
        Impl* pimpl_;
    };

}

#endif // SETTINGSFORM_H
