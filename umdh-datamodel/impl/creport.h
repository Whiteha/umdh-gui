#ifndef CREPORT_H
#define CREPORT_H

#include "idatamodel/idataobject.h"
#include "idatamodel/iclonable.h"
#include "idatamodel/ireport.h"
#include "idatamodel/iobjectfactory.h"

namespace datamodel
{
    class CReport :
            public gui::IDataObject,
            public gui::IClonable,
            public gui::IReport
    {
    public:
        CReport(const gui::IObjectFactory*, std::wstring typeId);

        // gui::IDataObject
        std::wstring type() const override;

        // gui::IClonable,
        gui::unique_ptr<IClonable> clone() const override;

        // gui::IReport
        void setLoadedModules(std::vector<gui::unique_ptr<gui::ILoadedModule>>) override;
        std::vector<gui::unique_ptr<gui::ILoadedModule>> getLoadedModules() const override;

        void setLogEntries(std::vector<gui::unique_ptr<gui::ILogEntry>>) override;
        std::vector<gui::unique_ptr<gui::ILogEntry>> getLogEntries() const override;

        void setTotalIncrease(gui::unique_ptr<gui::ITotalIncrease>) override;
        gui::unique_ptr<gui::ITotalIncrease> getTotalIncrease() const override;

    private:
        const gui::IObjectFactory *m_pObjectFactory = nullptr;
        const std::wstring m_typeId;
        std::vector<gui::unique_ptr<gui::ILoadedModule>> m_loadedModules;
        std::vector<gui::unique_ptr<gui::ILogEntry>> m_logEntries;
        gui::unique_ptr<gui::ITotalIncrease> m_totalIncrease;
    };
}

#endif // CREPORT_H
