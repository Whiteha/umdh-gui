#ifndef IREPORTFILTER_H
#define IREPORTFILTER_H

#include "idatamodel/ireport.h"

#pragma warning(push, 0)
#include <vector>
#include <functional>
#pragma warning(pop)

namespace gui
{
    using LogEntryFilter = std::function<bool(gui::ILogEntry*)>;

    struct IReportFilter
    {
        virtual ~IReportFilter() = default;

        virtual void setLogEntryFilters(std::vector<gui::LogEntryFilter>) = 0;
        virtual std::vector<LogEntryFilter> getLogEntryFilters() = 0;

        virtual unique_ptr<IReport> get(unique_ptr<IReport>) = 0;
    };
}
#endif // IREPORTFILTER_H
