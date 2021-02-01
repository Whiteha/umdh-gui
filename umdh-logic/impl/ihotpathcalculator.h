#ifndef IHOTPATHCALCULATOR_H
#define IHOTPATHCALCULATOR_H

#include "idatamodel/ireport.h"
#include "ilogic/ireportmanager.h"

#pragma warning(push, 0)
#include <string>
#pragma warning(pop)

namespace logic
{
    struct IHotPathCalculator
    {
        virtual ~IHotPathCalculator() = default;

        virtual gui::CallsByCountMap calculateCallsCount(gui::IReport*) const = 0;
    };
}

#endif // IHOTPATHCALCULATOR_H
