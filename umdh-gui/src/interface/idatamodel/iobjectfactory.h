#ifndef IOBJECTFACTORY_H
#define IOBJECTFACTORY_H

#include "idatamodel/idataobject.h"
#include "customuniqueptr.h"

#include <string>

namespace gui
{
    struct IObjectFactory
    {
        virtual ~IObjectFactory() = default;

        virtual unique_ptr<gui::IDataObject> get(std::wstring typeId) const = 0;
    };
}

#endif // IOBJECTFACTORY_H
