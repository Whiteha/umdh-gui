#ifndef ILEAKEDMORETHANBYTESFILTER_H
#define ILEAKEDMORETHANBYTESFILTER_H

#include "ireportfilter.h"

namespace gui
{
    struct ILeakedMoreThanBytesFilter
    {
        virtual ~ILeakedMoreThanBytesFilter() = default;

        virtual void setBytes(size_t) = 0;
    };
}
#endif // ILEAKEDMORETHANBYTESFILTER_H
