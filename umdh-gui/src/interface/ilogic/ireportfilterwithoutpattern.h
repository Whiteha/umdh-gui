#ifndef IREPORTFILTERWITHOUTPATTERN_H
#define IREPORTFILTERWITHOUTPATTERN_H

#include <string>
#include <list>

namespace gui
{
    struct IReportFilterWithoutPattern
    {
        virtual ~IReportFilterWithoutPattern() = default;

        virtual void setBadPattern(std::list<std::wstring> pattern) = 0;
    };
}

#endif // IREPORTFILTERWITHOUTPATTERN_H
