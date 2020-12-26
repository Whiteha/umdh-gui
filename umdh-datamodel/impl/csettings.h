#ifndef CSETTINGS_H
#define CSETTINGS_H

#include "idatamodel/idataobject.h"
#include "idatamodel/iclonable.h"
#include "idatamodel/isettings.h"
#include "idatamodel/iobjectfactory.h"

namespace datamodel
{
    class CSettings :
            public gui::IDataObject,
            public gui::IClonable,
            public gui::ISettings
    {
    public:
        CSettings(const gui::IObjectFactory*, std::wstring typeId);

        // gui::IDataObject
        std::wstring type() const override;

        // gui::IClonable,
        gui::unique_ptr<IClonable> clone() const override;

        // gui::ISettings
        void setUmdhPath(std::wstring umdhPath) override;
        std::wstring getUmdhPath() const override;

        void setNtSymbolPath(std::wstring ntSymbolPath) override;
        std::wstring getNtSymbolPath() const override;

        void setFolderForSnapshots(std::wstring folderForSnapshots) override;
        std::wstring getFolderForSnapshots() const override;

        void setTargetProgramm(std::wstring targetProgramm) override;
        std::wstring getTargetProgramm() const override;

        void setPID(std::wstring pid) override;
        std::wstring getPID() const override;

        void setUpdateNotifier(SettingsObserver) override;

    private:
        const gui::IObjectFactory *m_pObjectFactory = nullptr;
        const std::wstring m_typeId;
        SettingsObserver m_notifyer;
        struct CopyableState
        {
            std::wstring umdhPath;
            std::wstring ntSymbolPath;
            std::wstring folderForSnapshots;
            std::wstring targetProgramm;
            std::wstring pid;
        } m_copyableState;
    };
}

#endif // CSETTINGS_H
