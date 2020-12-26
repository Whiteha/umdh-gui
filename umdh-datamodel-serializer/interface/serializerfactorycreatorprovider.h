#ifndef SERIALIZERFACTORYCREATORPROVIDER_H
#define SERIALIZERFACTORYCREATORPROVIDER_H

#include "umdh.datamodel.serializer.global.h"
#include "idatamodelserializer/iserializerfactorycreator.h"
#include "customuniqueptr.h"

namespace datamodelserializer
{
    UMDH_DATAMODEL_SERIALIZER_EXPORT gui::unique_ptr<gui::ISerializerFactoryCreator> getSerializerFactoryCreator();
}

#endif // SERIALIZERFACTORYCREATORPROVIDER_H
