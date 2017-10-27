#ifndef FILTERSETTINGS_H
#define FILTERSETTINGS_H

#include <QSettings>
#include <QMetaEnum>

namespace sfv2 {

    template<class _T>
    _T getSettingsEnumValue(const QSettings& settings,
                            const QString& key,
                            const QString& defaultValue = QString())
    {
        return static_cast<_T>(
            QMetaEnum::fromType<_T>().keyToValue(
                settings.value(key, defaultValue).toString().toUtf8()));
    }

}

#endif // FILTERSETTINGS_H
