#ifndef CWITHOUTSYSTEMMODULESFILTER_H
#define CWITHOUTSYSTEMMODULESFILTER_H

#include "ilogic/ireportfilter.h"
#include "cdefaultfilter.h"

namespace logic
{
    class CWithoutSystemModulesFilter : public CDefaultFilter
    {
    public:
        CWithoutSystemModulesFilter();
    };
}

#endif // CWITHOUTSYSTEMMODULESFILTER_H
