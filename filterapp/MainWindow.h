#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPushButton;

namespace sfv2 {

    class FilterSettings;
    class FilterApplication;
    class ImageView;

    class MainWindow : public QMainWindow
    {
        Q_OBJECT
    public:
        explicit MainWindow(FilterApplication* app, QWidget* parent = nullptr);
        ~MainWindow();

        void showImage(QImage&& qimg);

    private Q_SLOTS:
        void onRoiChanged(const QPoint& top_left, const QSize& size);

    private:
        void addFilterParamsDockWidget();

        FilterApplication* app_;
        ImageView* image_view_;
        QPushButton* select_roi_btn_;
    };

}

#endif // MAINWINDOW_H
