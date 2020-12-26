#ifndef CSERIALIZERFACTORY_H
#define CSERIALIZERFACTORY_H

#include "idatamodelserializer/iserializerfactory.h"
#include "idatamodel/iobjectfactory.h"

namespace datamodelserializer
{    
    class CSerializerFactory : public gui::ISerializerFactory
    {
    public:
        CSerializerFactory(const gui::IObjectFactory*);
        gui::unique_ptr<gui::ISerializer> get(gui::IDataObject*) const override;

    private:
        const gui::IObjectFactory *m_pObjectFactory = nullptr;
    };
}

#endif // CSERIALIZERFACTORY_H
