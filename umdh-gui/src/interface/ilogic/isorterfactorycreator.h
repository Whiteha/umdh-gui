#ifndef ISORTERFACTORYCREATOR_H
#define ISORTERFACTORYCREATOR_H

#pragma warning(push, 0)
#include "isorterfactory.h"
#pragma warning(pop)

namespace gui
{
    struct ISorterFactoryCreator
    {
        virtual ~ISorterFactoryCreator() = default;

        virtual unique_ptr<ISorterFactory> create() const = 0;
    };
}


#endif // ISORTERFACTORYCREATOR_H
