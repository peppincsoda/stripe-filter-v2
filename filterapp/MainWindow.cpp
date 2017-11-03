#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ImageView.h"
#include "FilterApplication.h"
#include "FilterSettings.h"
#include "SettingsForm.h"
#include "FilterOutput.h"

#include <QPainter>

#include <cassert>

namespace sfv2 {

    MainWindow::MainWindow(FilterApplication* app, QWidget* parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
        , app_(app)
        , settings_form_(nullptr)
    {
        assert(app != nullptr);

        {
            auto window_flags = windowFlags();
            window_flags.setFlag(Qt::WindowMaximizeButtonHint, false);
            setWindowFlags(window_flags);
        }

        ui->setupUi(this);

        app->setMainWindow(this);

        ui->imageView->setRoi(app_->settings().roiTopLeft(),
                              app_->settings().roiSize());
        connect(ui->imageView, SIGNAL(roiChanged(QPoint,QSize)),
                this, SLOT(onRoiChanged(QPoint,QSize)));

        connect(ui->actionSelectRoi, SIGNAL(toggled(bool)),
                ui->imageView, SLOT(setSelectRoi(bool)));
        connect(ui->actionExit, SIGNAL(triggered(bool)),
                this, SLOT(close()));

        settings_form_ = new SettingsForm(&app_->settings(), this, Qt::Tool);
        settings_form_->show();

        connect(ui->actionShowSettings, SIGNAL(triggered(bool)),
                settings_form_, SLOT(show()));
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

        resize(pixmap.width(), pixmap.height());

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

        ui->imageView->updateImage(std::move(pixmap));
    }

    void MainWindow::setHistImage(QImage&& qimg)
    {
        settings_form_->setHistImage(std::move(qimg));
    }

    void MainWindow::onRoiChanged(const QPoint& top_left, const QSize& size)
    {
        app_->settings().setRoiTopLeft(top_left);
        app_->settings().setRoiSize(size);
        ui->actionSelectRoi->setChecked(false);
    }

    void MainWindow::onShowSettings()
    {
        settings_form_->show();
    }
}
