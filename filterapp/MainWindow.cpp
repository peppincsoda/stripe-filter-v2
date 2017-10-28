#include "MainWindow.h"

#include <QDesktopWidget>
#include <QLabel>

namespace sfv2 {

    MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
        , image_lbl_(nullptr)
    {
        setObjectName("mainWindow");
        setWindowTitle(tr("stripe-filter-v2"));
        resize(QDesktopWidget().availableGeometry().size() * 0.7);

        image_lbl_ = new QLabel(this);
        setCentralWidget(image_lbl_);
    }

    MainWindow::~MainWindow()
    {

    }

    void MainWindow::setImage(const QImage& qimg)
    {
        image_lbl_->setPixmap(QPixmap::fromImage(qimg));
    }
}
