#include "cleakedmorethancountfilter.h"

using namespace logic;

void CLeakedMoreThanCountFilter::setCount(size_t count)
{
    auto filter = [count] (gui::ILogEntry *pLogEntry)
    {
        const auto newCount = pLogEntry->getNewCount();
        const auto oldCount = pLogEntry->getOldCount();
        if ((newCount >= oldCount) && (newCount - oldCount  >= count))
            return true;

        return false;
    };
    setLogEntryFilters({filter});
}
