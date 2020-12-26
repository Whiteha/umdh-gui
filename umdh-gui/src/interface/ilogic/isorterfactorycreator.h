#ifndef ISORTERFACTORYCREATOR_H
#define ISORTERFACTORYCREATOR_H

#include "isorterfactory.h"

namespace gui
{
    struct ISorterFactoryCreator
    {
        virtual ~ISorterFactoryCreator() = default;

        virtual unique_ptr<ISorterFactory> create() const = 0;
    };
}


#endif // ISORTERFACTORYCREATOR_H
