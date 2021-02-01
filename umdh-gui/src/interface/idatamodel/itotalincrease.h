#ifndef ITOTALINCREASE_H
#define ITOTALINCREASE_H

#pragma warning(push, 0)
#include <string>
#pragma warning(pop)

namespace gui
{
    struct ITotalIncrease
    {
        virtual ~ITotalIncrease() = default;

        virtual void setRequested(size_t) = 0;
        virtual size_t getRequested() const = 0;

        virtual void setOverhead(size_t) = 0;
        virtual size_t getOverhead() const = 0;
    };
}

#endif // ITOTALINCREASE_H
