#ifndef FILTERAPPLICATION_H
#define FILTERAPPLICATION_H

#include "RetryHandler.h"

#include <QApplication>

#include <memory>

class QSettings;
class QTimer;

namespace sfv2 {
  
    class FilterInput;
    class FilterOutput;

    struct FilterInputData;
    struct FilterOutputData;

    class FilterApplication : public QApplication
    {
        Q_OBJECT
    public:
        enum class InputType
        {
            CVInput,
            BaslerInput,
            UnknownInput = -1,
        };
        Q_ENUM(InputType)

        enum class OutputType
        {
            ConsoleOutput,
            SerialOutput,
            UnknownOutput = -1,
        };
        Q_ENUM(OutputType)

        static void requestExit();

        FilterApplication(int& argc, char** argv);
        ~FilterApplication();

        void setSettingsFile(const QString& settings_file);

    private Q_SLOTS:
        void onIdle();

    private:
        void doFrame();
        bool openInput();
        bool openOutput();
        bool tryReadInput(FilterInputData& data);
        bool tryWriteOutput(const FilterOutputData& data);
        void processFrame(const FilterInputData& input_data, FilterOutputData& output_data);

        std::unique_ptr<QSettings> settings_;
        std::unique_ptr<QTimer> idle_timer_;

        std::unique_ptr<FilterInput> input_;
        std::unique_ptr<FilterOutput> output_;

        RetryHandler input_handler_;
        RetryHandler output_handler_;
    };

}

#endif // FILTERAPPLICATION_H
