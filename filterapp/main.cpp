#include "FilterApplication.h"
#include "MainWindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDateTime>

#include <iostream>
#include <memory>
#include <signal.h>

static void message_handler(QtMsgType type, const QMessageLogContext &/*context*/, const QString &msg)
{
    const auto& date_time = QDateTime::currentDateTimeUtc().toString(Qt::ISODate).toLocal8Bit();
    const auto& local_msg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        std::cerr << date_time.constData() << " DEBUG    " << local_msg.constData() << std::endl;
        break;
    case QtInfoMsg:
        std::cerr << date_time.constData() << " INFO     " << local_msg.constData() << std::endl;
        break;
    case QtWarningMsg:
        std::cerr << date_time.constData() << " WARNING  " << local_msg.constData() << std::endl;
        break;
    case QtCriticalMsg:
        std::cerr << date_time.constData() << " CRITICAL " << local_msg.constData() << std::endl;
        break;
    case QtFatalMsg:
        std::cerr << date_time.constData() << " FATAL    " << local_msg.constData() << std::endl;
        break;
    }
}

static void sig_handler(int signo)
{
    if (signo == SIGINT ||
        signo == SIGTERM) {
        sfv2::FilterApplication::requestExit();
    }
}

static std::unique_ptr<QCoreApplication> create_application(int& argc, char** argv)
{
    for (int i = 1; i < argc; ++i) {
        if (qstrcmp(argv[i], "-g") == 0 || qstrcmp(argv[i], "--gui") == 0) {
            return std::make_unique<QApplication>(argc, argv);
        }
    }
    return std::make_unique<QCoreApplication>(argc, argv);
}

int main(int argc, char* argv[])
{
    qInstallMessageHandler(message_handler);

    if (signal(SIGINT, sig_handler) == SIG_ERR ||
        signal(SIGTERM, sig_handler) == SIG_ERR) {
        qFatal("Cannot install SIGINT handler");
        return -1;
    }

    auto app = create_application(argc, argv);
    app->setApplicationName("stripe-filter");
    app->setApplicationVersion("2.0");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    // This is needed only for displaying the help
    QCommandLineOption guiOption(QStringList() << "g" << "gui",
        QCoreApplication::translate("main", "Show GUI."));
    parser.addOption(guiOption);

    QCommandLineOption settingsFileOption(QStringList() << "s" << "settings-file",
        QCoreApplication::translate("main", "Reads all settings from <file-path>."),
        QCoreApplication::translate("main", "file-path"),
        "settings.ini");
    parser.addOption(settingsFileOption);

    parser.process(*app);

    sfv2::FilterApplication filter_app;
    QObject::connect(&filter_app, SIGNAL(quit()),
                     app.get(), SLOT(quit()));

    filter_app.setSettingsFile(parser.value(settingsFileOption));

    std::unique_ptr<sfv2::MainWindow> window;
    if (qobject_cast<QApplication *>(app.get()) != nullptr) {
        // Show UI
        window = std::make_unique<sfv2::MainWindow>(&filter_app);
        window->show();
    } else {
        qInfo() << "Running in headless mode";
    }

    const auto ret = app->exec();
    qInfo() << "Exiting...";
    return ret;
}
