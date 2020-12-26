#ifndef CSORTERFACTORY_H
#define CSORTERFACTORY_H

#include "ilogic/isorterfactory.h"

namespace logic
{
    class CSorterFactory : public gui::ISorterFactory
    {
    public:
        gui::unique_ptr<gui::IReportSorter> get(std::wstring typeId) const override;
    };
}

#endif // CSORTERFACTORY_H
