#include "cbyallocatedbytessorter.h"

#pragma warning(push, 0)
#include <algorithm>
#pragma warning(pop)

using namespace logic;

gui::unique_ptr<gui::IReport> CByAllocatedBytesSorter::sort(gui::unique_ptr<gui::IReport> pReport) const
{
    auto logEntries = pReport->getLogEntries();
    std::sort(logEntries.rbegin(), logEntries.rend(), [] (const gui::unique_ptr<gui::ILogEntry> &pEntryLeft,
                                                          const gui::unique_ptr<gui::ILogEntry> &pEntryRight)
    {
        const auto leftBytesNew = pEntryLeft->getNewBytes();
        const auto leftBytesOld = pEntryLeft->getOldBytes();
        const auto rightBytesNew = pEntryRight->getNewBytes();
        const auto rightBytesOld = pEntryRight->getOldBytes();
        return (static_cast<std::ptrdiff_t>(leftBytesNew - leftBytesOld) < static_cast<std::ptrdiff_t>(rightBytesNew - rightBytesOld));
    });
    pReport->setLogEntries(std::move(logEntries));

    return pReport;
}
