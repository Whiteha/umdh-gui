#ifndef ILEAKEDMORETHANCOUNTFILTER_H
#define ILEAKEDMORETHANCOUNTFILTER_H

#pragma warning(push, 0)
#include <cctype>
#pragma warning(pop)

namespace gui
{
    struct ILeakedMoreThanCountFilter
    {
        virtual ~ILeakedMoreThanCountFilter() = default;

        virtual void setCount(size_t) = 0;
    };
}
#endif // ILEAKEDMORETHANCOUNTFILTER_H
