#ifndef ISERIALIZER_H
#define ISERIALIZER_H

#include <string>

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
