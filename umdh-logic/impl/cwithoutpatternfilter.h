#ifndef CWITHOUTPATTERNFILTER_H
#define CWITHOUTPATTERNFILTER_H

#include "ilogic/ireportfilterwithoutpattern.h"
#include "cdefaultfilter.h"

#include <string>

namespace logic
{
    class CWithoutPatternFilter : public CDefaultFilter, public gui::IReportFilterWithoutPattern
    {
    public:
        void setBadPattern(std::list<std::wstring> pattern) override;
    };
}

#endif // CWITHOUTPATTERNFILTER_H
