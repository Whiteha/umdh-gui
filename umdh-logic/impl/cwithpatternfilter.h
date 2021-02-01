#ifndef CWITHPATTERNFILTER_H
#define CWITHPATTERNFILTER_H

#include "ilogic/ireportfilterwithpattern.h"
#include "cdefaultfilter.h"

#pragma warning(push, 0)
#include <string>
#pragma warning(pop)

namespace logic
{
    class CWithPatternFilter : public CDefaultFilter, public gui::IReportFilterWithPattern
    {
    public:
        void setPattern(std::list<std::wstring> pattern) override;
    };
}

#endif // CWITHPATTERNFILTER_H
