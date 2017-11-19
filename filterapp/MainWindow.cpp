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

        connect(ui->settingsForm, SIGNAL(testResetClicked(bool)),
                this, SLOT(onResetTest()));
        ui->settingsForm->setSettings(&app_->settings());
    }

    MainWindow::~MainWindow()
    {

    }

    int MainWindow::histDisplayHeight() const
    {
        return ui->settingsForm->histDisplayHeight();
    }

    void MainWindow::setImage(QImage&& qimg, const FilterOutputData &output_data)
    {
        auto pixmap = QPixmap::fromImage(std::move(qimg));

        resize(pixmap.width(), pixmap.height());

        // Draw detection lines
        const auto& roi_p = app_->settings().roiTopLeft();
        const auto& roi_size = app_->settings().roiSize();

        QPainter p(&pixmap);

        p.setPen(Qt::yellow);
        const auto xleft = roi_p.x() + output_data.leftDist();
        p.drawLine(xleft, roi_p.y(), xleft, roi_p.y() + roi_size.height());
        p.setPen(Qt::red);
        const auto xright = roi_p.x() + output_data.rightDist();
        p.drawLine(xright, roi_p.y(), xright, roi_p.y() + roi_size.height());

        p.end();

        ui->imageView->updateImage(std::move(pixmap));
    }

    void MainWindow::setOutputData(const FilterOutputData &output_data)
    {
        ui->outputDataForm->setOutputData(output_data);
    }

    void MainWindow::setHistImage(QImage&& qimg)
    {
        ui->settingsForm->setHistImage(std::move(qimg));
    }

    void MainWindow::onRoiChanged(const QPoint& top_left, const QSize& size)
    {
        app_->settings().setRoiTopLeft(top_left);
        app_->settings().setRoiSize(size);
        ui->actionSelectRoi->setChecked(false);
    }

    void MainWindow::onResetTest()
    {
        app_->resetTest();
    }
}
