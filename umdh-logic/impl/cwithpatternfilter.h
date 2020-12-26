#ifndef CWITHPATTERNFILTER_H
#define CWITHPATTERNFILTER_H

#include "ilogic/ireportfilterwithpattern.h"
#include "cdefaultfilter.h"

#include <string>

namespace logic
{
    class CWithPatternFilter : public CDefaultFilter, public gui::IReportFilterWithPattern
    {
    public:
        void setPattern(std::list<std::wstring> pattern) override;
    };
}

#endif // CWITHPATTERNFILTER_H
