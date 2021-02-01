#ifndef ISERIALIZER_H
#define ISERIALIZER_H

#pragma warning(push, 0)
#include <string>
#pragma warning(pop)

namespace gui
{
    struct ISerializer
    {
        virtual ~ISerializer() = default;

        virtual std::wstring toString() const = 0;
        virtual void fromString(std::wstring fromString) = 0;
    };
}

#endif // ISERIALIZER_H
