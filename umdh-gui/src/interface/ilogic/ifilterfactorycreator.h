#ifndef IFILTERFACTORYCREATOR_H
#define IFILTERFACTORYCREATOR_H

#include "ifilterfactory.h"
#include "customuniqueptr.h"

namespace gui
{
    struct IFilterFactoryCreator
    {
        virtual ~IFilterFactoryCreator() = default;

        virtual unique_ptr<IFilterFactory> create() const = 0;
    };
}
#endif // IFILTERFACTORYCREATOR_H
