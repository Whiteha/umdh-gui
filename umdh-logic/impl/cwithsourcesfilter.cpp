#include "cwithsourcesfilter.h"

using namespace logic;

CWithSourcesFilter::CWithSourcesFilter()
{
    auto filter = [] (gui::ILogEntry *pLogEntry)
    {
        const auto &stackTrace = pLogEntry->getStackTrace();
        bool hasSource = false;
        for (auto &pFrame : stackTrace)
        {
            hasSource = !pFrame->getSource().empty();
            if (hasSource)
                break;
        }

        if (hasSource)
            return true;

        return false;
    };
    setLogEntryFilters({filter});
}
