#ifndef IREPORT_H
#define IREPORT_H

#include "idatamodel/iloadedmodule.h"
#include "idatamodel/ilogentry.h"
#include "idatamodel/itotalincrease.h"

#include "iserializable.h"

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

namespace gui
{
    struct IReport
    {
        virtual ~IReport() = default;

        virtual void setLoadedModules(std::vector<unique_ptr<gui::ILoadedModule>>) = 0;
        virtual std::vector<unique_ptr<gui::ILoadedModule>> getLoadedModules() const = 0;

        virtual void setLogEntries(std::vector<unique_ptr<ILogEntry>>) = 0;
        virtual std::vector<unique_ptr<ILogEntry>> getLogEntries() const = 0;

        virtual void setTotalIncrease(unique_ptr<gui::ITotalIncrease>) = 0;
        virtual unique_ptr<gui::ITotalIncrease> getTotalIncrease() const = 0;
    };
}

#endif // IREPORT_H
