#ifndef CFILTERFACTORY_H
#define CFILTERFACTORY_H

#include "ilogic/ifilterfactory.h"

namespace logic
{
    class CFilterFactory : public gui::IFilterFactory
    {
    public:
        gui::unique_ptr<gui::IReportFilter> get(std::wstring typeId) const override;
    };
}

#endif // CFILTERFACTORY_H
