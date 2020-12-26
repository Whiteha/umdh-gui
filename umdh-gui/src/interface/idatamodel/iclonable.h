#ifndef ICLONABLE_H
#define ICLONABLE_H

#include "customuniqueptr.h"

namespace gui
{
    struct IClonable
    {
        virtual ~IClonable() = default;
        virtual unique_ptr<IClonable> clone() const = 0;
    };
}
#endif // ICLONABLE_H
