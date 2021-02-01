#ifndef IREPORTSORTER_H
#define IREPORTSORTER_H

#include "idatamodel/ireport.h"

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

namespace gui
{
    struct IReportSorter
    {
        virtual ~IReportSorter() = default;

        virtual unique_ptr<IReport> sort(unique_ptr<IReport>) const = 0;
    };
}

#endif // IREPORTSORTER_H
