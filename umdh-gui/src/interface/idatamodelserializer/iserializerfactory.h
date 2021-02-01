#ifndef ISERIALIZERFACTORY_H
#define ISERIALIZERFACTORY_H

#include "iserializer.h"
#include "idatamodel/idataobject.h"
#include "customuniqueptr.h"

#pragma warning(push, 0)
#include <string>
#pragma warning(pop)

namespace gui
{
    struct ISerializerFactory
    {
        virtual ~ISerializerFactory() = default;
        virtual unique_ptr<gui::ISerializer> get(IDataObject*) const = 0;
    };

    template <class T>
    void readSerializedString(const ISerializerFactory* pSerializerFactory, T *pDataObject, std::wstring data)
    {
        auto pCasted = dynamic_cast<gui::IDataObject*>(pDataObject);
        auto pNewLoadedModuleSerializer = pSerializerFactory->get(pCasted);
        pNewLoadedModuleSerializer->fromString(data);
    }
}

#endif // ISERIALIZERFACTORY_H
