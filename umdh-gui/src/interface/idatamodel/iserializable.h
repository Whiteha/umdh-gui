#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

#include <string>

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
