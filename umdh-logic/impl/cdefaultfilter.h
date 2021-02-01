#ifndef CDEFAULTFILTER_H
#define CDEFAULTFILTER_H

#include "ilogic/ireportfilter.h"
#include "idatamodel/ilogentry.h"

#pragma warning(push, 0)
#include <vector>
#include <functional>
#pragma warning(pop)

namespace logic
{
    class CDefaultFilter : public gui::IReportFilter
    {
    public:
        void setLogEntryFilters(std::vector<gui::LogEntryFilter>) override;
        std::vector<gui::LogEntryFilter> getLogEntryFilters() override;

        gui::unique_ptr<gui::IReport> get(gui::unique_ptr<gui::IReport>) override;
    private:
        std::vector<std::function<bool(gui::ILogEntry*)>> m_logEntryFilters;
    };
}

#endif // CDEFAULTFILTER_H
