#include "FilterApplication.h"

#include <QCommandLineParser>
#include <QDebug>

#include <signal.h>

static void sig_handler(int signo)
{
    if (signo == SIGINT ||
        signo == SIGTERM) {
        sfv2::FilterApplication::requestExit();
    }
}

int main(int argc, char* argv[])
{
    if (signal(SIGINT, sig_handler) == SIG_ERR ||
        signal(SIGTERM, sig_handler) == SIG_ERR) {
        qCritical() << "Cannot install SIGINT handler";
        return -1;
    }

    sfv2::FilterApplication app(argc, argv);
    QCoreApplication::setApplicationName("stripe-filter");
    QCoreApplication::setApplicationVersion("2.0");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption settingsFileOption(QStringList() << "s" << "settings-file",
                QCoreApplication::translate("main", "Reads all settings from <file-path>."),
                QCoreApplication::translate("main", "file-path"),
                "settings.ini");
    parser.addOption(settingsFileOption);

    parser.process(app);

    app.setSettingsFile(parser.value(settingsFileOption));

    return app.exec();
}