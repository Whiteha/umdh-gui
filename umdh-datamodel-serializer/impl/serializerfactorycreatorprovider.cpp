#include "interface/serializerfactorycreatorprovider.h"
#include "cserializerfactorycreator.h"

gui::unique_ptr<gui::ISerializerFactoryCreator> datamodelserializer::getSerializerFactoryCreator()
{
    return gui::make_unique<datamodelserializer::CSerializerFactoryCreator>();
}
