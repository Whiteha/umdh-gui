#ifndef CSERIALIZERFACTORYCREATOR_H
#define CSERIALIZERFACTORYCREATOR_H

#include "idatamodelserializer/iserializerfactorycreator.h"

namespace datamodelserializer
{
    class CSerializerFactoryCreator : public gui::ISerializerFactoryCreator
    {
    public:
        gui::unique_ptr<gui::ISerializerFactory> create(const gui::IObjectFactory*) const override;
    };
}

#endif // CSERIALIZERFACTORYCREATOR_H
