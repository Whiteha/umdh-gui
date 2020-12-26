#ifndef ISERIALIZERFACTORYCREATOR_H
#define ISERIALIZERFACTORYCREATOR_H

#include "idatamodelserializer/iserializerfactory.h"
#include "idatamodel/iobjectfactory.h"

namespace gui
{
    struct ISerializerFactoryCreator
    {
        virtual ~ISerializerFactoryCreator() = default;
        virtual unique_ptr<ISerializerFactory> create(const IObjectFactory*) const = 0;
    };
}

#endif // ISERIALIZERFACTORYCREATOR_H
