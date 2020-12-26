#include "cserializerfactorycreator.h"
#include "cserializerfactory.h"

using namespace datamodelserializer;

gui::unique_ptr<gui::ISerializerFactory> CSerializerFactoryCreator::create(const gui::IObjectFactory* pObjectFactory) const
{
    return gui::make_unique<CSerializerFactory>(pObjectFactory);
}
