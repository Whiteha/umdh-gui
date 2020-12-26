#ifndef ISORTERFACTORY_H
#define ISORTERFACTORY_H

#include "ireportsorter.h"

#include <string>

namespace gui
{
    struct ISorterFactory
    {
        virtual ~ISorterFactory() = default;

        virtual unique_ptr<IReportSorter> get(std::wstring typeId) const = 0;
    };
}

#endif // ISORTERFACTORY_H
