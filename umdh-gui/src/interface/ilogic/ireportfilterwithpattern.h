#ifndef IREPORTFILTERWITHPATTERN_H
#define IREPORTFILTERWITHPATTERN_H

#include <string>
#include <list>

namespace gui
{
    struct IReportFilterWithPattern
    {
        virtual ~IReportFilterWithPattern() = default;

        virtual void setPattern(std::list<std::wstring> pattern) = 0;
    };
}

#endif // IREPORTFILTERWITHPATTERN_H
