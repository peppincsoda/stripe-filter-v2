#include "MainWindow.h"
#include "ImageView.h"
#include "FilterApplication.h"
#include "FilterSettings.h"
#include "SettingsForm.h"
#include "FilterOutput.h"

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
        , settings_form_(nullptr)
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

        settings_form_ = new SettingsForm(&app_->settings(), this);

        addFilterParamsDockWidget();
    }

    MainWindow::~MainWindow()
    {

    }

    int MainWindow::histDisplayHeight() const
    {
        return settings_form_->histDisplayHeight();
    }

    void MainWindow::setImageAndOutput(QImage&& qimg, const FilterOutputData &output_data)
    {
        auto pixmap = QPixmap::fromImage(std::move(qimg));

        // Draw detection lines
        const auto& roi_p = app_->settings().roiTopLeft();
        const auto& roi_size = app_->settings().roiSize();

        QPainter p(&pixmap);

        p.setPen(Qt::yellow);
        const auto xleft = roi_p.x() + output_data.left_dist;
        p.drawLine(xleft, roi_p.y(), xleft, roi_p.y() + roi_size.height());
        p.setPen(Qt::red);
        const auto xright = roi_p.x() + output_data.right_dist;
        p.drawLine(xright, roi_p.y(), xright, roi_p.y() + roi_size.height());

        p.end();

        image_view_->updateImage(std::move(pixmap));
    }

    void MainWindow::setHistImage(QImage&& qimg)
    {
        settings_form_->setHistImage(std::move(qimg));
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

        dock_widget->setObjectName("filterParamsDockWidget");
        dock_widget->setWindowTitle(tr("Filter Parameters"));
        dock_widget->setFeatures(QDockWidget::NoDockWidgetFeatures);
        dock_widget->setWidget(settings_form_);
        addDockWidget(Qt::RightDockWidgetArea, dock_widget);
    }
}
