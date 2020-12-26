#ifndef CDEFAULTSORTER_H
#define CDEFAULTSORTER_H

#include "ilogic/ireportsorter.h"

namespace logic
{
    class CDefaultSorter : public gui::IReportSorter
    {
    public:
        gui::unique_ptr<gui::IReport> sort(gui::unique_ptr<gui::IReport>) const override;
    };
}

#endif // CDEFAULTSORTER_H
