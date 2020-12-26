#ifndef CSETTINGSSERIALIZER_H
#define CSETTINGSSERIALIZER_H

#include "idatamodelserializer/iserializer.h"
#include "idatamodelserializer/iserializerfactory.h"

#include "idatamodel/iobjectfactory.h"
#include "idatamodel/idataobject.h"
#include "idatamodel/isettings.h"


namespace datamodelserializer
{    
    class CSettingsSerializer : public gui::ISerializer
    {
    public:
        CSettingsSerializer(gui::IDataObject*,
                            const gui::ISerializerFactory*,
                            const gui::IObjectFactory*);

        std::wstring toString() const override;
        void fromString(std::wstring fromString) override;

    private:
        gui::ISettings *m_pSettings = nullptr;
        const gui::ISerializerFactory *m_pSerializerFactory = nullptr;
        const gui::IObjectFactory *m_pObjectFactory = nullptr;
    };
}

#endif // CSETTINGSSERIALIZER_H
