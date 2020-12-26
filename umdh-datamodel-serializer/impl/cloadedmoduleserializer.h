#ifndef CLOADEDMODULESERIALIZER_H
#define CLOADEDMODULESERIALIZER_H

#include "idatamodelserializer/iserializer.h"
#include "idatamodelserializer/iserializerfactory.h"

#include "idatamodel/iobjectfactory.h"
#include "idatamodel/idataobject.h"
#include "idatamodel/iloadedmodule.h"


namespace datamodelserializer
{
    class CLoadedModuleSerializer : public gui::ISerializer
    {
    public:
        CLoadedModuleSerializer(gui::IDataObject*,
                                const gui::ISerializerFactory*,
                                const gui::IObjectFactory*);

        std::wstring toString() const override;
        void fromString(std::wstring fromString) override;

    private:
        gui::ILoadedModule *m_pLoadedModule = nullptr;
        const gui::ISerializerFactory *m_pSerializerFactory = nullptr;
        const gui::IObjectFactory *m_pObjectFactory = nullptr;
    };
}

#endif // CLOADEDMODULESERIALIZER_H
