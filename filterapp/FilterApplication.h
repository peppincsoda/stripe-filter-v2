#ifndef FILTERAPPLICATION_H
#define FILTERAPPLICATION_H

#include "RetryHandler.h"

#include <QObject>

#include <memory>

class QTimer;

namespace cv {
    class Mat;
}

namespace sfv2 {
  
    class FilterSettings;

    class FilterInput;
    class FilterOutput;

    struct FilterInputData;
    struct FilterOutputData;

    class MainWindow;

    class FilterApplication : public QObject
    {
        Q_OBJECT
    public:
        static void requestExit();

        explicit FilterApplication(QObject* parent = nullptr);
        ~FilterApplication();

        FilterSettings& settings();

        void setSettingsFile(const QString& file_name);

        void setMainWindow(MainWindow* main_window);

    Q_SIGNALS:
        void quit();

    private Q_SLOTS:
        void onIdle();
        void closeInput();
        void closeOutput();
        void onSerialPortChanged();

    private:
        void doFrame();
        bool openInput();
        bool openOutput();
        bool tryReadInput(FilterInputData& data);
        bool tryWriteOutput(const FilterOutputData& data);
        void processFrame(const FilterInputData& input_data, FilterOutputData& output_data);
        void getRoiParams(const cv::Mat& img, QPoint& roi_p, QSize& roi_size);
        void applyLowPassFilters(cv::Mat& img);
        void computeHistogram(const cv::Mat& img, cv::Mat& hist, double& entropy);
        void drawHistogram(cv::Mat& hist, cv::Mat& hist_img);
        void applyThresholding(cv::Mat& img);
        void findObject(const cv::Mat& img, FilterOutputData& output_data);
        void optimizeRoi(QPoint& roi_p, QSize& roi_size);

        std::unique_ptr<FilterSettings> settings_;
        std::unique_ptr<QTimer> idle_timer_;

        std::unique_ptr<FilterInput> input_;
        std::unique_ptr<FilterOutput> output_;

        RetryHandler input_handler_;
        RetryHandler output_handler_;

        MainWindow* main_window_;
    };

}

#endif // FILTERAPPLICATION_H
