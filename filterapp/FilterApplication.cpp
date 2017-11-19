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
#include <QFileInfo>

#include <opencv2/opencv.hpp>

namespace sfv2 {

    static bool exit_requested_ = false;
    static const int min_period_ms_ = 50;

    void FilterApplication::requestExit()
    {
        exit_requested_ = true;
    }

    FilterApplication::FilterApplication(QObject* parent)
        : QObject(parent)
        , settings_()
        , idle_timer_()
        , test_timer_()
        , fps_timer_()
        , frames_this_second_(0)
        , fps_(0)
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

        test_timer_ = std::make_unique<QElapsedTimer>();
        test_timer_->start();

        fps_timer_ = std::make_unique<QElapsedTimer>();
        fps_timer_->start();
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
        QFileInfo info(file_name);
        if (!info.exists() || !info.isFile()) {
            qWarning() << "Settings file" << file_name << "was not found; using default parameters";
        } else {
            qInfo() << "Settings file is:" << file_name;
        }

        settings_ = std::make_unique<FilterSettings>(file_name);
        connect(settings_.get(), SIGNAL(inputTypeChanged()),
                this, SLOT(closeInput()));
        connect(settings_.get(), SIGNAL(outputTypeChanged()),
                this, SLOT(closeOutput()));
        connect(settings_.get(), SIGNAL(serialPortChanged()),
                this, SLOT(onSerialPortChanged()));
    }

    void FilterApplication::setMainWindow(MainWindow* main_window)
    {
        main_window_ = main_window;
    }

    void FilterApplication::resetTest()
    {
        test_timer_->start();
    }

    void FilterApplication::onIdle()
    {
        if (exit_requested_) {
            emit quit();
            return;
        }

        QElapsedTimer timer;
        timer.start();

        doFrame();

        // Measure FPS
        frames_this_second_++;
        if (fps_timer_->elapsed() > 1000) {
            fps_ = frames_this_second_;
            frames_this_second_ = 0;
            fps_timer_->start();
        }

        // Never process the next frame before min_period_ms_ has been elapsed
        idle_timer_->start(std::max(min_period_ms_ - static_cast<int>(timer.elapsed()), 0));
    }

    void FilterApplication::doFrame()
    {
        FilterOutputData output_data;
        output_data.setFps(fps_);

        if (settings_->testMode()) {
            processTestMode(output_data);

        } else {
            FilterInputData input_data;

            cv::Mat frame;
            input_data.frame = &frame;

            if (tryReadInput(input_data)) {
                processFrame(input_data, output_data);
            } else {
                output_data.reset();
                output_data.setStatus(FilterOutputData::InputFailed);
            }
        }

        if (main_window_ != nullptr)
            main_window_->setOutputData(output_data);

        tryWriteOutput(output_data);
    }

    void FilterApplication::closeInput()
    {
        qInfo() << "Closing input";
        input_ = nullptr;
        input_handler_.reset();
    }

    void FilterApplication::closeOutput()
    {
        qInfo() << "Closing output";
        output_ = nullptr;
        output_handler_.reset();
    }

    void FilterApplication::onSerialPortChanged()
    {
        if (settings_->outputType() == FilterSettings::SerialOutput) {
            closeOutput();
        }
    }

    bool FilterApplication::openInput()
    {
        assert(input_ == nullptr);

        switch (settings_->inputType()) {
        case FilterSettings::CVInput:
            qInfo() << "Opening OpenCV input";
            input_ = std::make_unique<CVInput>();
            break;
        case FilterSettings::BaslerInput:
            qInfo() << "Opening Basler camera input";
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
            qInfo() << "Opening console output";
            output_ = std::make_unique<ConsoleOutput>();
            break;
        case FilterSettings::SerialOutput:
            qInfo() << "Opening serial output";
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
        if (settings_->inputType() == FilterSettings::UnknownInput) {
            return false;
        }

        if (!input_handler_.succeeded()) {
            return false;
        }

        if (!input_->read(data)) {
            closeInput();
            return false;
        }

        return true;
    }

    bool FilterApplication::tryWriteOutput(const FilterOutputData &data)
    {
        if (settings_->outputType() == FilterSettings::UnknownOutput) {
            return false;
        }

        if (!output_handler_.succeeded()) {
            return false;
        }

        if (!output_->write(data)) {
            closeOutput();
            return false;
        }

        return true;
    }

    void FilterApplication::processFrame(const FilterInputData& input_data, FilterOutputData& output_data)
    {
        cv::Mat* input_img = input_data.frame;

        // Convert the full image to grayscale in GUI mode (if it is not already grayscale)
        cv::Mat gray_img;
        if (main_window_ != nullptr &&
            input_img->type() != CV_8UC1) {
            cv::cvtColor(*input_img, gray_img, cv::COLOR_BGR2GRAY);
            input_img = &gray_img;
        }

        QPoint roi_p;
        QSize roi_size;
        getRoiParams(*input_img, roi_p, roi_size);

        if (settings_->optimizeRoi())
            optimizeRoi(roi_p, roi_size);

        if (roi_size.width() > 0 && roi_size.height() > 0) {
            // Get ROI
            cv::Mat roi_img = (*input_img)(cv::Rect(roi_p.x(),
                                                    roi_p.y(),
                                                    roi_size.width(),
                                                    roi_size.height()));
            cv::Mat* proc_img = &roi_img;

            // Convert ROI to grayscale in headless mode
            cv::Mat roi_gray_img;
            if (main_window_ == nullptr &&
                proc_img->type() != CV_8UC1) {
                cv::cvtColor(roi_img, roi_gray_img, cv::COLOR_BGR2GRAY);
                proc_img = &roi_gray_img;
            }

            assert(proc_img->type() == CV_8UC1);

            applyLowPassFilters(*proc_img);

            cv::Mat hist;
            {
                double entropy = 0;
                computeHistogram(*proc_img, hist, entropy);
                output_data.setEntropy(entropy);
            }

            // Draw histogram if running interactively
            if (main_window_ != nullptr) {
                cv::Mat hist_img;
                drawHistogram(hist, hist_img);
                main_window_->setHistImage(QImage(hist_img.data,
                                                  hist_img.cols,
                                                  hist_img.rows,
                                                  hist_img.step,
                                                  QImage::Format_Grayscale8));
            }

            applyThresholding(*proc_img);

            findObject(*proc_img, output_data);

        } else {
            // Invalid ROI parameters
            output_data.reset();
            output_data.setStatus(FilterOutputData::ProcessingInvalidParams);
        }

        // Send data to the UI
        if (main_window_ != nullptr) {
            main_window_->setImage(QImage(input_img->data,
                                          input_img->cols,
                                          input_img->rows,
                                          input_img->step,
                                          QImage::Format_Grayscale8),
                                          output_data);
        }
    }

    void FilterApplication::processTestMode(FilterOutputData& output_data)
    {
        output_data.setStatus(settings_->testFilterStatus());

        const auto elapsed = test_timer_->elapsed();

        const auto min_val = settings_->testMinMeasurement();
        const auto max_val = settings_->testMaxMeasurement();
        const auto val_step = settings_->testMeasurementStep();
        const auto time_step = settings_->testTimeStep();

        if (time_step == 0) {
            output_data.setMeasurement(0, 0, 0);
            output_data.setEntropy(0);
        } else {
            const auto val = min_val + (elapsed / time_step) * val_step;
            const auto clamped_val = max_val < val ? max_val : val;
            output_data.setMeasurement(0, 0, clamped_val);
            output_data.setEntropy(clamped_val);
        }
    }

    void FilterApplication::getRoiParams(const cv::Mat& img, QPoint& roi_p, QSize& roi_size)
    {
        // Get ROI parameters from settings
        roi_p = settings_->roiTopLeft();
        roi_size = settings_->roiSize();
        const auto roi_p_save = roi_p;
        const auto roi_size_save = roi_size;
        // Ensure that ROI width and height are positive
        if (roi_size.width() < 0) {
            roi_size.setWidth(-roi_size.width());
            roi_p.setX(roi_p.x() - roi_size.width());
        }
        if (roi_size.height() < 0) {
            roi_size.setHeight(-roi_size.height());
            roi_p.setY(roi_p.y() - roi_size.height());
        }
        // Ensure that ROI is inside the image
        if (roi_p.x() < 0 || roi_p.x() >= img.cols)
            roi_p.setX(0);
        if (roi_p.y() < 0 || roi_p.y() >= img.rows)
            roi_p.setY(0);
        if (roi_p.x() + roi_size.width() > img.cols)
            roi_size.setWidth(img.cols - roi_p.x());
        if (roi_p.y() + roi_size.height() > img.rows)
            roi_size.setHeight(img.rows - roi_p.y());
        // Store corrected ROI parameters
        if (roi_p != roi_p_save)
            settings_->setRoiTopLeft(roi_p);
        if (roi_size != roi_size_save)
            settings_->setRoiSize(roi_size);
    }

    void FilterApplication::applyLowPassFilters(cv::Mat& img)
    {
        if (settings_->useMedian()) {
            auto ksize = settings_->medianKSize();
            if (!(3 <= ksize && (ksize % 2) == 1)) {
                ksize = 3;
                settings_->setMedianKSize(ksize);
                qWarning() << "Invalid setting for filter/median-ksize; value reverted to default";
            }
            cv::medianBlur(img, img, ksize);
        }

        if (settings_->useGaussian()) {
            auto ksize = settings_->gaussianKSize();
            auto sigma = settings_->gaussianSigma();
            if (!(3 <= ksize && (ksize % 2) == 1)) {
                ksize = 3;
                settings_->setGaussianKSize(ksize);
                qWarning() << "Invalid setting for filter/gaussian-ksize; value reverted to default";
            }
            if (!(0.1 <= sigma)) {
                sigma = 1.5;
                settings_->setGaussianSigma(sigma);
                qWarning() << "Invalid setting for filter/gaussian-sigma; value reverted to default";
            }
            cv::GaussianBlur(img, img, cv::Size(ksize, ksize), sigma, sigma);
        }

        if (settings_->useBox()) {
            auto ksize = settings_->boxKSize();
            if (!(3 <= ksize && (ksize % 2) == 1)) {
                ksize = 3;
                settings_->setBoxKSize(ksize);
                qWarning() << "Invalid setting for filter/box-ksize; value reverted to default";
            }
            cv::blur(img, img, cv::Size(ksize, ksize));
        }
    }

    void FilterApplication::computeHistogram(const cv::Mat& img, cv::Mat& hist, double& entropy)
    {
        int channels[] = { 0 };
        const int num_bins = 256;
        float sranges[] = { 0, 256 };
        const float* ranges[] = { sranges };

        cv::calcHist(&img, 1, channels, cv::Mat(), // do not use mask
                     hist, 1, &num_bins, ranges,
                     true, // the histogram is uniform
                     true  // clear the result matrix before calculation
                     );
        cv::normalize(hist, hist);

        // Compute entropy
        cv::Mat log_p;
        cv::log(hist, log_p);
        entropy = -cv::sum(hist.mul(log_p)).val[0];
    }

    void FilterApplication::drawHistogram(cv::Mat& hist, cv::Mat& hist_img)
    {
        assert(main_window_ != nullptr);

        const auto hist_height = main_window_->histDisplayHeight();
        cv::normalize(hist, hist, hist_height);
        //double max_val = 0;
        //cv::minMaxLoc(hist, 0, &max_val, 0, 0);

        // Generate histogram image
        const int num_bins = 256;
        hist_img = cv::Mat::zeros(hist_height, num_bins, CV_8UC1);
        for (int i = 0; i < num_bins; i++) {
            const auto bin_val = hist.at<float>(i);
            const auto intensity = cvRound(bin_val);
            //const auto intensity = cvRound(bin_val * hist_height / max_val);

            cv::line(hist_img, cv::Point(i, hist_height - intensity), cv::Point(i, hist_height), 255);
        }
    }

    void FilterApplication::applyThresholding(cv::Mat& img)
    {
        if (settings_->useThreshold()) {
            auto thresh = settings_->thresholdValue();
            if (!(0 <= thresh && thresh < 255)) {
                thresh = 127;
                settings_->setThresholdValue(thresh);
                qWarning() << "Invalid setting for filter/threshold-value; value reverted to default";

            }
            cv::threshold(img, img, thresh, 255, cv::THRESH_BINARY);
        }
    }

    //! Find the object on the horizontal centerline
    void FilterApplication::findObject(const cv::Mat& img, FilterOutputData& output_data)
    {
        const auto intensity = settings_->blackObject() ? 0 : 255;

        const int half_rows = img.rows / 2;
        // Find intensity from left
        int col_left = 0;
        while (col_left < img.cols &&
               img.at<uchar>(half_rows, col_left) != intensity) {
            col_left++;
        }

        // Find intensity from right
        int col_right = img.cols - 1;
        while (col_right >= 0 &&
               img.at<uchar>(half_rows, col_right) != intensity) {
            col_right--;
        }

        if (col_left >= img.cols ||
            col_right < 0 ||
            col_left > col_right) {

            output_data.setMeasurement(col_left, col_right, 0);
            output_data.setStatus(FilterOutputData::ProcessingFailed);
        } else {
            output_data.setMeasurement(col_left, col_right, col_right - col_left + 1);
            output_data.setStatus(FilterOutputData::OK);
        }
    }

    void FilterApplication::optimizeRoi(QPoint& roi_p, QSize& roi_size)
    {
        // Compute the minimum height required for the ROI
        auto sum_k_size = 1;
        if (settings_->useMedian())
            sum_k_size += settings_->medianKSize() - 1;
        if (settings_->useGaussian())
            sum_k_size += settings_->gaussianKSize() - 1;
        if (settings_->useBox())
            sum_k_size += settings_->boxKSize() - 1;

        // Adjust ROI height
        const auto center_y = roi_p.y() + roi_size.height() / 2;
        roi_p.setY(center_y - ((sum_k_size - 1) / 2));
        roi_size.setHeight(sum_k_size);
    }

}
