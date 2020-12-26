#ifndef IREPORTSORTER_H
#define IREPORTSORTER_H

#include "idatamodel/ireport.h"

#include <vector>

namespace gui
{
    struct IReportSorter
    {
        virtual ~IReportSorter() = default;

        virtual unique_ptr<IReport> sort(unique_ptr<IReport>) const = 0;
    };
}

#endif // IREPORTSORTER_H
