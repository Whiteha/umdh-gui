#include "cbyallocationcountsorter.h"

#pragma warning(push, 0)
#include <algorithm>
#pragma warning(pop)

using namespace logic;

gui::unique_ptr<gui::IReport> CByAllocationCountSorter::sort(gui::unique_ptr<gui::IReport> pReport) const
{
    auto logEntries = pReport->getLogEntries();
    std::sort(logEntries.rbegin(), logEntries.rend(), [] (const gui::unique_ptr<gui::ILogEntry> &pEntryLeft,
                                                          const gui::unique_ptr<gui::ILogEntry> &pEntryRight)
    {
        const auto leftCountNew = pEntryLeft->getNewCount();
        const auto leftCountOld = pEntryLeft->getOldCount();
        const auto rightCountNew = pEntryRight->getNewCount();
        const auto rightCountOld = pEntryRight->getOldCount();
        return (static_cast<std::ptrdiff_t>(leftCountNew - leftCountOld) < static_cast<std::ptrdiff_t>(rightCountNew - rightCountOld));
    });
    pReport->setLogEntries(std::move(logEntries));

    return pReport;
}
