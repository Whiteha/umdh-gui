#include "cwithoutpatternfilter.h"

using namespace logic;

void CWithoutPatternFilter::setBadPattern(std::list<std::wstring> patternList)
{
    auto filter = [patternList = std::move(patternList)] (gui::ILogEntry *pLogEntry)
    {
        auto stackTrace = pLogEntry->getStackTrace();
        bool hasPattern = false;
        for (auto &pFrame : stackTrace)
        {
            for (const auto &pattern : patternList)
            {
                hasPattern = (pFrame->getFunction().find(pattern) != std::wstring::npos);
                if (hasPattern)
                    return false;

                hasPattern = (pFrame->getModule().find(pattern) != std::wstring::npos);
                if (hasPattern)
                    return false;

                hasPattern = (pFrame->getSource().find(pattern) != std::wstring::npos);
                if (hasPattern)
                    return false;
            }
        }
        return true;
    };
    setLogEntryFilters({filter});
}

