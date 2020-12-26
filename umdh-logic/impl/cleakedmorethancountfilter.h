#ifndef CLEAKEDMORETHANCOUNTFILTER_H
#define CLEAKEDMORETHANCOUNTFILTER_H

#include "ilogic/ileakedmorethancountfilter.h"
#include "cdefaultfilter.h"

namespace logic
{
    class CLeakedMoreThanCountFilter : public CDefaultFilter, public gui::ILeakedMoreThanCountFilter
    {
    public:
        void setCount(size_t) override;
    };
}
#endif // CLEAKEDMORETHANCOUNTFILTER_H
