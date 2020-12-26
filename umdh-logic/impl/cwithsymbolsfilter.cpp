#include "cwithsymbolsfilter.h"

namespace
{
    const auto c_unknownFucntion = L"???";
}

using namespace logic;

CWithSymbolsFilter::CWithSymbolsFilter()
{
    auto filter = [] (gui::ILogEntry *pLogEntry)
    {
        const auto &stackTrace = pLogEntry->getStackTrace();
        bool hasSymbols = false;
        for (auto &pFrame : stackTrace)
        {
            const auto &functionName = pFrame->getFunction();
            hasSymbols = !(functionName.empty() || functionName == c_unknownFucntion);
            if (hasSymbols)
                break;
        }

        if (hasSymbols)
            return true;

        return false;
    };
    setLogEntryFilters({filter});
}
