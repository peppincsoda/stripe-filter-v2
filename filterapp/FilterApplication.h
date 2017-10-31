#ifndef FILTERAPPLICATION_H
#define FILTERAPPLICATION_H

#include "RetryHandler.h"

#include <QApplication>

#include <memory>

class QTimer;

namespace sfv2 {
  
    class FilterSettings;

    class FilterInput;
    class FilterOutput;

    struct FilterInputData;
    struct FilterOutputData;

    class MainWindow;

    class FilterApplication : public QApplication
    {
        Q_OBJECT
    public:
        static void requestExit();

        FilterApplication(int& argc, char** argv);
        ~FilterApplication();

        FilterSettings& settings();

        void setSettingsFile(const QString& file_name);

        void setMainWindow(MainWindow* main_window);

    private Q_SLOTS:
        void onIdle();

    private:
        void doFrame();
        bool openInput();
        bool openOutput();
        bool tryReadInput(FilterInputData& data);
        bool tryWriteOutput(const FilterOutputData& data);
        void processFrame(const FilterInputData& input_data, FilterOutputData& output_data);

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
