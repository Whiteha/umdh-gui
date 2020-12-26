#ifndef CLOGENTRYSERIALIZER_H
#define CLOGENTRYSERIALIZER_H

#include "idatamodelserializer/iserializer.h"
#include "idatamodelserializer/iserializerfactory.h"

#include "idatamodel/iobjectfactory.h"
#include "idatamodel/idataobject.h"
#include "idatamodel/ilogentry.h"


namespace datamodelserializer
{    
    class CLogEntrySerializer : public gui::ISerializer
    {
    public:
        CLogEntrySerializer(gui::IDataObject*,
                            const gui::ISerializerFactory*,
                            const gui::IObjectFactory*);

        std::wstring toString() const override;
        void fromString(std::wstring fromString) override;

    private:
        gui::ILogEntry *m_pLogEntry = nullptr;
        const gui::ISerializerFactory *m_pSerializerFactory = nullptr;
        const gui::IObjectFactory *m_pObjectFactory = nullptr;
    };
}

#endif // CLOGENTRYSERIALIZER_H
