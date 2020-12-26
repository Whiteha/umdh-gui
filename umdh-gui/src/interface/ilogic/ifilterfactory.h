#ifndef IFILTERFACTORY_H
#define IFILTERFACTORY_H

#include "ireportfilter.h"

#include <string>

namespace gui
{
    struct IFilterFactory
    {
        virtual ~IFilterFactory() = default;

        virtual unique_ptr<IReportFilter> get(std::wstring typeId) const = 0;
    };
}

#endif // IFILTERFACTORY_H
