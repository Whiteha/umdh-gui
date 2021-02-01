#ifndef IREPORTFILTERWITHPATTERN_H
#define IREPORTFILTERWITHPATTERN_H

#pragma warning(push, 0)
#include <string>
#include <list>
#pragma warning(pop)

namespace gui
{
    struct IReportFilterWithPattern
    {
        virtual ~IReportFilterWithPattern() = default;

        virtual void setPattern(std::list<std::wstring> pattern) = 0;
    };
}

#endif // IREPORTFILTERWITHPATTERN_H
