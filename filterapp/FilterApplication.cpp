#include "FilterApplication.h"
#include "FilterSettings.h"

#include "CVInput.h"
#include "BaslerInput.h"

#include "ConsoleOutput.h"
#include "SerialOutput.h"

#include <QTimer>
#include <QElapsedTimer>
#include <QDebug>

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
    {
        idle_timer_ = std::make_unique<QTimer>();
        connect(idle_timer_.get(), SIGNAL(timeout()), this, SLOT(onIdle()));
        idle_timer_->setSingleShot(true);
        idle_timer_->start(0);
    }

    FilterApplication::~FilterApplication()
    {

    }

    void FilterApplication::setSettingsFile(const QString& settings_file)
    {
        qInfo() << "Settings file is:" << settings_file;
        // TODO: check if settings file exist
        settings_ = std::make_unique<QSettings>(settings_file, QSettings::IniFormat);
    }

    void FilterApplication::onIdle()
    {
        if (exit_requested_) {
            qInfo() << "Exiting...";
            quit();
            return;
        }

        QElapsedTimer timer;
        timer.start();

        doFrame();

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

        const auto input_type = getSettingsEnumValue<InputType>(*settings_,
                                                                "filterapp/input-type",
                                                                "CVInput");
        switch (input_type) {
        case InputType::CVInput:
            input_ = std::make_unique<CVInput>();
            break;
        case InputType::BaslerInput:
            input_ = std::make_unique<BaslerInput>();
            break;
        case InputType::UnknownInput:
            qWarning() << "Unknown input type in settings";
            break;
        }

        if (!input_->open(*settings_)) {
            input_ = nullptr;
            return false;
        }

        return true;
    }

    bool FilterApplication::openOutput()
    {
        assert(output_ == nullptr);

        const auto output_type = getSettingsEnumValue<OutputType>(*settings_,
                                                                  "filterapp/output-type",
                                                                  "ConsoleOutput");
        switch (output_type) {
        case OutputType::ConsoleOutput:
            output_ = std::make_unique<ConsoleOutput>();
            break;
        case OutputType::SerialOutput:
            output_ = std::make_unique<SerialOutput>();
            break;
        case OutputType::UnknownOutput:
            qWarning() << "Unknown output type in settings";
            break;
        }

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
            output_ = nullptr;
            output_handler_.reset();
            return false;
        }

        return true;
    }

    void FilterApplication::processFrame(const FilterInputData& input_data, FilterOutputData& /*output_data*/)
    {
        cv::Mat edges;
        cv::cvtColor(*input_data.frame, edges, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(edges, edges, cv::Size(7,7), 1.5, 1.5);
        cv::Canny(edges, edges, 0, 30, 3);
        cv::imshow("edges", edges);
    }


}
