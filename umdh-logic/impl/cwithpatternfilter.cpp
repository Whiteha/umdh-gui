#include "cwithpatternfilter.h"

using namespace logic;

void CWithPatternFilter::setPattern(std::list<std::wstring> patternList)
{
    auto filter = [patternList = std::move(patternList)] (gui::ILogEntry *pLogEntry)
    {
        auto pStackTrace = pLogEntry->getStackTrace();
        for (const auto &pattern : patternList)
        {
            bool hasPattern = false;
            for (auto &pFrame : pStackTrace)
            {
                hasPattern |= (pFrame->getFunction().find(pattern) != std::wstring::npos);
                if (hasPattern)
                    break;

                hasPattern |= (pFrame->getModule().find(pattern) != std::wstring::npos);
                if (hasPattern)
                    break;

                hasPattern |= (pFrame->getSource().find(pattern) != std::wstring::npos);
                if (hasPattern)
                    break;
            }
            if (!hasPattern)
                return false;
        }
        return true;
    };
    setLogEntryFilters({filter});
}
