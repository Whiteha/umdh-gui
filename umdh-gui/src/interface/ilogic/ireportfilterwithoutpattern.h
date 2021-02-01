#ifndef IREPORTFILTERWITHOUTPATTERN_H
#define IREPORTFILTERWITHOUTPATTERN_H

#pragma warning(push, 0)
#include <string>
#include <list>
#pragma warning(pop)

namespace gui
{
    struct IReportFilterWithoutPattern
    {
        virtual ~IReportFilterWithoutPattern() = default;

        virtual void setBadPattern(std::list<std::wstring> pattern) = 0;
    };
}

#endif // IREPORTFILTERWITHOUTPATTERN_H
