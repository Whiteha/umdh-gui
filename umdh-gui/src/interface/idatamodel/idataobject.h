#ifndef IDATAOBJECT_H
#define IDATAOBJECT_H

#include <string>

namespace gui
{
    struct IDataObject
    {
        virtual ~IDataObject() = default;
        virtual std::wstring type() const = 0;
    };
}

#endif // IDATAOBJECT_H
