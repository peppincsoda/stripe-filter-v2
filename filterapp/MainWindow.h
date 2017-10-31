#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPushButton;

namespace sfv2 {

    class FilterApplication;
    class ImageView;
    class SettingsForm;

    class MainWindow : public QMainWindow
    {
        Q_OBJECT
    public:
        explicit MainWindow(FilterApplication* app, QWidget* parent = nullptr);
        ~MainWindow();

        int histDisplayHeight() const;

        void showImage(QImage&& qimg);
        void showHistImage(QImage&& qimg);

    private Q_SLOTS:
        void onRoiChanged(const QPoint& top_left, const QSize& size);

    private:
        void addFilterParamsDockWidget();

        FilterApplication* app_;
        ImageView* image_view_;
        QPushButton* select_roi_btn_;
        SettingsForm* settings_form_;
    };

}

#endif // MAINWINDOW_H
