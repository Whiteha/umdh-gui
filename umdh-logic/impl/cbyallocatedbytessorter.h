#ifndef CBYALLOCATEDBYTESSORTER_H
#define CBYALLOCATEDBYTESSORTER_H

#include "ilogic/ireportsorter.h"

namespace logic
{
    class CByAllocatedBytesSorter : public gui::IReportSorter
    {
    public:
        gui::unique_ptr<gui::IReport> sort(gui::unique_ptr<gui::IReport>) const override;
    };
}

#endif // CBYALLOCATEDBYTESSORTER_H
