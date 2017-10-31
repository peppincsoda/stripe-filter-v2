#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>

namespace sfv2 {

    class FilterSettings;

    class SettingsForm : public QWidget
    {
        Q_OBJECT

    public:
        explicit SettingsForm(FilterSettings* settings, QWidget *parent = nullptr);
        ~SettingsForm();

    private:
        class Impl;
        Impl* pimpl_;
    };

}

#endif // SETTINGSFORM_H
