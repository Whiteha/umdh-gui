#include "cleakedmorethanbytesfilter.h"

using namespace logic;

void CLeakedMoreThanBytesFilter::setBytes(size_t bytes)
{
    auto filter = [bytes] (gui::ILogEntry *pLogEntry)
    {
        const auto newBytes = pLogEntry->getNewBytes();
        const auto oldBytes = pLogEntry->getOldBytes();
        if ((newBytes >= oldBytes) && (newBytes - oldBytes  >= bytes))
            return true;

        return false;
    };
    setLogEntryFilters({filter});
}
