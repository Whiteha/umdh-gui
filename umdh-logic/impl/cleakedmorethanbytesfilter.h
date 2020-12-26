#ifndef CLEAKEDMORETHANBYTESFILTER_H
#define CLEAKEDMORETHANBYTESFILTER_H

#include "ilogic/ileakedmorethanbytesfilter.h"
#include "cdefaultfilter.h"

namespace logic
{
    class CLeakedMoreThanBytesFilter : public CDefaultFilter, public gui::ILeakedMoreThanBytesFilter
    {
    public:
        void setBytes(size_t) override;
    };
}

#endif // CLEAKEDMORETHANBYTESFILTER_H
