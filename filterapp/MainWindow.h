#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;

namespace sfv2 {

    class MainWindow : public QMainWindow
    {
        Q_OBJECT
    public:
        explicit MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

        void setImage(const QImage& qimg);

    private:
        QLabel* image_lbl_;
    };

}

#endif // MAINWINDOW_H
