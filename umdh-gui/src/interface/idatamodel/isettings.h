#ifndef ISETTINGS_H
#define ISETTINGS_H

#include "iserializable.h"

#pragma warning(push, 0)
#include <string>
#include <functional>
#pragma warning(pop)

using SettingsObserver = std::function<void()>;

namespace gui
{
    struct ISettings
    {
        virtual ~ISettings() = default;

        virtual void setUmdhPath(std::wstring umdhPat) = 0;
        virtual std::wstring getUmdhPath() const = 0;

        virtual void setNtSymbolPath(std::wstring ntSymbolPath) = 0;
        virtual std::wstring getNtSymbolPath() const = 0;

        virtual void setFolderForSnapshots(std::wstring folderForSnapshots) = 0;
        virtual std::wstring getFolderForSnapshots() const = 0;

        virtual void setTargetProgramm(std::wstring targetProgramm) = 0;
        virtual std::wstring getTargetProgramm() const = 0;

        virtual void setPID(std::wstring pid) = 0;
        virtual std::wstring getPID() const = 0;

        virtual void setUpdateNotifier(SettingsObserver) = 0;
    };
}

#endif // ISETTINGS_H
