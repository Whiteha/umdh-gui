#ifndef ILEAKEDMORETHANCOUNTFILTER_H
#define ILEAKEDMORETHANCOUNTFILTER_H

#include <cctype>

namespace gui
{
    struct ILeakedMoreThanCountFilter
    {
        virtual ~ILeakedMoreThanCountFilter() = default;

        virtual void setCount(size_t) = 0;
    };
}
#endif // ILEAKEDMORETHANCOUNTFILTER_H
