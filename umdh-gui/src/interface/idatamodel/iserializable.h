#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

#pragma warning(push, 0)
#include <string>
#pragma warning(pop)

namespace gui
{
    struct ISerializable
    {
        virtual ~ISerializable() = default;

        virtual std::wstring toString() const = 0;
        virtual void fromString(std::wstring) = 0;
    };
}

#endif // ISERIALIZABLE_H
