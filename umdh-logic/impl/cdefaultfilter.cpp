#include "impl/cdefaultfilter.h"

using namespace logic;

void CDefaultFilter::setLogEntryFilters(std::vector<gui::LogEntryFilter> logEntryFilters)
{
    m_logEntryFilters = std::move(logEntryFilters);
}

std::vector<gui::LogEntryFilter> CDefaultFilter::getLogEntryFilters()
{
    return m_logEntryFilters;
}

gui::unique_ptr<gui::IReport> CDefaultFilter::get(gui::unique_ptr<gui::IReport> pReport)
{
    auto logEntries = pReport->getLogEntries();
    std::vector<gui::unique_ptr<gui::ILogEntry>> filteredLogEntry;
    for (auto &pLogEntry : logEntries)
    {
        bool isAllFiltersPassed = true;
        for (auto &logEntryFilter : m_logEntryFilters)
        {
            if (!logEntryFilter(pLogEntry.get()))
            {
                isAllFiltersPassed = false;
                break;
            }
        }

        if (isAllFiltersPassed)
        {
            filteredLogEntry.push_back(std::move(pLogEntry));
        }
    }
    pReport->setLogEntries(std::move(filteredLogEntry));

    return pReport;
}
