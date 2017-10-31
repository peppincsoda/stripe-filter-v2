#include "FilterApplication.h"
#include "FilterSettings.h"
#include "MainWindow.h"

#include "CVInput.h"
#include "BaslerInput.h"

#include "ConsoleOutput.h"
#include "SerialOutput.h"

#include <QTimer>
#include <QElapsedTimer>
#include <QDebug>
#include <QImage>

#include <opencv2/opencv.hpp>

namespace sfv2 {

    static bool exit_requested_ = false;
    static int min_period_ms_ = 50;

    void FilterApplication::requestExit()
    {
        exit_requested_ = true;
    }

    FilterApplication::FilterApplication(int& argc, char** argv)
        : QApplication(argc, argv)
        , settings_()
        , idle_timer_()
        , input_()
        , output_()
        , input_handler_(3, 1000, 30000, [this]() { return openInput(); })
        , output_handler_(3, 1000, 30000, [this]() { return openOutput(); })
        , main_window_(nullptr)
    {
        idle_timer_ = std::make_unique<QTimer>();
        connect(idle_timer_.get(), SIGNAL(timeout()), this, SLOT(onIdle()));
        idle_timer_->setSingleShot(true);
        idle_timer_->start(0);
    }

    FilterApplication::~FilterApplication()
    {

    }

    FilterSettings& FilterApplication::settings()
    {
        return *settings_;
    }

    void FilterApplication::setSettingsFile(const QString& file_name)
    {
        qInfo() << "Settings file is:" << file_name;
        // TODO: check if settings file exist
        settings_ = std::make_unique<FilterSettings>(file_name);
    }

    void FilterApplication::setMainWindow(MainWindow* main_window)
    {
        main_window_ = main_window;
    }

    void FilterApplication::onIdle()
    {
        if (exit_requested_) {
            quit();
            return;
        }

        QElapsedTimer timer;
        timer.start();

        doFrame();

        // Never process the next frame before min_period_ms_ has been elapsed
        idle_timer_->start(std::max(min_period_ms_ - static_cast<int>(timer.elapsed()), 0));
    }

    void FilterApplication::doFrame()
    {
        FilterInputData input_data;
        FilterOutputData output_data;

        cv::Mat frame;
        input_data.frame = &frame;

        if (tryReadInput(input_data)) {
            processFrame(input_data, output_data);
        } else {
            output_data.measurement = 0;
            output_data.status = FilterStatus::InputFailed;
        }

        tryWriteOutput(output_data);
    }

    bool FilterApplication::openInput()
    {
        assert(input_ == nullptr);

        switch (settings_->inputType()) {
        case FilterSettings::CVInput:
            input_ = std::make_unique<CVInput>();
            break;
        case FilterSettings::BaslerInput:
            input_ = std::make_unique<BaslerInput>();
            break;
        case FilterSettings::UnknownInput:
            qWarning() << "Unknown input type in settings";
            break;
        }

        if (input_ == nullptr)
            return false;

        if (!input_->open(*settings_)) {
            input_ = nullptr;
            return false;
        }

        return true;
    }

    bool FilterApplication::openOutput()
    {
        assert(output_ == nullptr);

        switch (settings_->outputType()) {
        case FilterSettings::ConsoleOutput:
            output_ = std::make_unique<ConsoleOutput>();
            break;
        case FilterSettings::SerialOutput:
            output_ = std::make_unique<SerialOutput>();
            break;
        case FilterSettings::UnknownOutput:
            qWarning() << "Unknown output type in settings";
            break;
        }

        if (output_ == nullptr)
            return false;

        if (!output_->open(*settings_)) {
            output_ = nullptr;
            return false;
        }

        return true;
    }

    bool FilterApplication::tryReadInput(FilterInputData& data)
    {
        if (!input_handler_.succeeded()) {
            return false;
        }

        if (!input_->read(data)) {
            qWarning() << "Resetting input";
            input_ = nullptr;
            input_handler_.reset();
            return false;
        }

        return true;
    }

    bool FilterApplication::tryWriteOutput(const FilterOutputData &data)
    {
        if (!output_handler_.succeeded()) {
            return false;
        }

        if (!output_->write(data)) {
            qWarning() << "Resetting output";
            output_ = nullptr;
            output_handler_.reset();
            return false;
        }

        return true;
    }

    void FilterApplication::processFrame(const FilterInputData& input_data, FilterOutputData& /*output_data*/)
    {
        cv::Mat img;

        // Convert to grayscale
        cv::cvtColor(*input_data.frame, img, cv::COLOR_BGR2GRAY);

        // Get ROI parameters
        auto roi_p = settings_->roiTopLeft();
        auto roi_size = settings_->roiSize();
        // Ensure that the ROI width and height are positive
        if (roi_size.width() < 0) {
            roi_size.setWidth(-roi_size.width());
            roi_p.setX(roi_p.x() - roi_size.width());
        }
        if (roi_size.height() < 0) {
            roi_size.setHeight(-roi_size.height());
            roi_p.setY(roi_p.y() - roi_size.height());
        }
        // Ensure that the ROI is inside the image
        if (roi_p.x() < 0 || roi_p.x() >= img.cols)
            roi_p.setX(0);
        if (roi_p.y() < 0 || roi_p.y() >= img.rows)
            roi_p.setY(0);
        if (roi_p.x() + roi_size.width() > img.cols)
            roi_size.setWidth(img.cols - roi_p.x());
        if (roi_p.y() + roi_size.height() > img.rows)
            roi_size.setHeight(img.rows - roi_p.y());

        if (roi_size.width() > 0 && roi_size.height() > 0) {
            cv::Mat roi_img = img(cv::Rect(roi_p.x(),
                                           roi_p.y(),
                                           roi_size.width(),
                                           roi_size.height()));

            // Apply required low-pass filters
            if (settings_->useMedian()) {
                const auto ksize = settings_->medianKSize();
                if (3 <= ksize && (ksize % 2) == 1) {
                    cv::medianBlur(roi_img, roi_img, ksize);
                } else {
                    // TODO:
                }
            }

            if (settings_->useGaussian()) {
                const auto ksize = settings_->gaussianKSize();
                const auto sigma = settings_->gaussianSigma();
                if (3 <= ksize && (ksize % 2) == 1) {
                    cv::GaussianBlur(roi_img, roi_img, cv::Size(ksize, ksize), sigma, sigma);
                } else {
                    // TODO:
                }
            }

            if (settings_->useBox()) {
                const auto ksize = settings_->boxKSize();
                if (3 <= ksize && (ksize % 2) == 1) {
                    cv::blur(roi_img, roi_img, cv::Size(ksize, ksize));
                } else {
                    // TODO:
                }
            }

            //cv::calcHist()...

            if (settings_->useThreshold()) {
                const auto thresh = settings_->thresholdValue();
                if (0 <= thresh && thresh < 255) { // TODO: TEST
                    cv::threshold(roi_img, roi_img, thresh, 255, cv::THRESH_BINARY);
                } else {
                    // TODO:
                }
            }

        // find edges
        // find color left->right or right->left

        }

        // draw overlay
        //6. draw overlay: rect for regions, current detection, histogram with threshold
        //cv::line(img, cv::Point(100, 100), cv::Point(200, 100), 255);

        if (main_window_ != nullptr) {
            main_window_->showImage(QImage(img.data,
                                           img.cols,
                                           img.rows,
                                           img.step,
                                           QImage::Format_Grayscale8));
        }
    }


}
