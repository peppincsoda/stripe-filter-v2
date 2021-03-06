#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class QImage;

namespace sfv2 {

    class FilterApplication;
    struct FilterOutputData;

    class MainWindow : public QMainWindow
    {
        Q_OBJECT
    public:
        explicit MainWindow(FilterApplication* app, QWidget* parent = nullptr);
        ~MainWindow();

        int histDisplayHeight() const;

        void setImage(QImage&& qimg, const FilterOutputData &output_data);
        void setOutputData(const FilterOutputData &output_data);
        void setHistImage(QImage&& qimg);

    private Q_SLOTS:
        void onRoiChanged(const QPoint& top_left, const QSize& size);
        void onResetTest();

    private:
        Ui::MainWindow *ui;
        FilterApplication* app_;
    };

}

#endif // MAINWINDOW_H
