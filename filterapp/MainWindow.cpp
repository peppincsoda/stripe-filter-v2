#include "MainWindow.h"
#include "ImageView.h"
#include "FilterApplication.h"
#include "FilterSettings.h"
#include "SettingsForm.h"

#include <QDesktopWidget>
#include <QPainter>

#include <QDockWidget>
#include <QPushButton>

namespace sfv2 {

    MainWindow::MainWindow(FilterApplication* app, QWidget* parent)
        : QMainWindow(parent)
        , app_(app)
        , image_view_(nullptr)
        , select_roi_btn_(nullptr)
    {
        if (app != nullptr) {
            app->setMainWindow(this);
        }

        setObjectName("mainWindow");
        setWindowTitle(tr("stripe-filter-v2"));
        resize(QDesktopWidget().availableGeometry().size() * 0.7);

        image_view_ = new ImageView(this);
        image_view_->setRoi(app_->settings().roiTopLeft(),
                            app_->settings().roiSize());
        setCentralWidget(image_view_);

        connect(image_view_, SIGNAL(roiChanged(QPoint,QSize)),
                this, SLOT(onRoiChanged(QPoint,QSize)));

        select_roi_btn_ = new QPushButton(tr("Select ROI"), this);
        select_roi_btn_->setCheckable(true);
        connect(select_roi_btn_, SIGNAL(toggled(bool)),
                image_view_, SLOT(setSelectRoi(bool)));

        addFilterParamsDockWidget();
    }

    MainWindow::~MainWindow()
    {

    }

    void MainWindow::showImage(QImage&& qimg)
    {
        auto pixmap = QPixmap::fromImage(std::move(qimg));

        QPainter p(&pixmap);
        p.setPen(Qt::white);

        p.drawLine(0, 0, 200, 200);
        p.end();

        image_view_->updateImage(std::move(pixmap));
    }

    void MainWindow::onRoiChanged(const QPoint& top_left, const QSize& size)
    {
        app_->settings().setRoiTopLeft(top_left);
        app_->settings().setRoiSize(size);
        select_roi_btn_->setChecked(false);
    }

    void MainWindow::addFilterParamsDockWidget()
    {
        auto* dock_widget = new QDockWidget(QString(), this);

        auto* dock_contents = new SettingsForm(&app_->settings(), this);

        dock_widget->setObjectName("filterParamsDockWidget");
        dock_widget->setWindowTitle(tr("Filter Parameters"));
        dock_widget->setFeatures(QDockWidget::NoDockWidgetFeatures);
        dock_widget->setWidget(dock_contents);
        addDockWidget(Qt::RightDockWidgetArea, dock_widget);
    }
}
