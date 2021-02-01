#ifndef IDATAOBJECT_H
#define IDATAOBJECT_H

#pragma warning(push, 0)
#include <string>
#pragma warning(pop)

namespace gui
{
    struct IDataObject
    {
        virtual ~IDataObject() = default;
        virtual std::wstring type() const = 0;
    };
}

#endif // IDATAOBJECT_H
