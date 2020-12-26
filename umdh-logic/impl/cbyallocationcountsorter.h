#ifndef CBYALLOCATIONCOUNTSORTER_H
#define CBYALLOCATIONCOUNTSORTER_H

#include "ilogic/ireportsorter.h"

namespace logic
{
    class CByAllocationCountSorter : public gui::IReportSorter
    {
    public:
        gui::unique_ptr<gui::IReport> sort(gui::unique_ptr<gui::IReport>) const override;
    };
}

#endif // CBYALLOCATIONCOUNTSORTER_H
