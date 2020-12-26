#ifndef CREPORTSERIALIZER_H
#define CREPORTSERIALIZER_H

#include "idatamodelserializer/iserializer.h"
#include "idatamodelserializer/iserializerfactory.h"

#include "idatamodel/iobjectfactory.h"
#include "idatamodel/idataobject.h"
#include "idatamodel/ireport.h"


namespace datamodelserializer
{    
    class CReportSerializer : public gui::ISerializer
    {
    public:
        CReportSerializer(gui::IDataObject*,
                          const gui::ISerializerFactory*,
                          const gui::IObjectFactory*);

        std::wstring toString() const override;
        void fromString(std::wstring fromString) override;

    private:
        gui::IReport *m_pReport = nullptr;
        const gui::ISerializerFactory *m_pSerializerFactory = nullptr;
        const gui::IObjectFactory *m_pObjectFactory = nullptr;
    };
}

#endif // CREPORTSERIALIZER_H
