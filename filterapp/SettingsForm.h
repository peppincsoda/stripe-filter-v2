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
        explicit SettingsForm(FilterSettings* settings,
                              QWidget *parent = nullptr,
                              Qt::WindowFlags flags = Qt::WindowFlags());
        ~SettingsForm();

        int histDisplayHeight() const;
        void setHistImage(QImage&& qimg);

    Q_SIGNALS:
        void testResetClicked(bool);

    private:
        class Impl;
        Impl* pimpl_;
    };

}

#endif // SETTINGSFORM_H
