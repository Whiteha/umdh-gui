#ifndef CTOTALINCREASESERIALIZER_H
#define CTOTALINCREASESERIALIZER_H

#include "idatamodelserializer/iserializer.h"
#include "idatamodelserializer/iserializerfactory.h"

#include "idatamodel/iobjectfactory.h"
#include "idatamodel/idataobject.h"
#include "idatamodel/itotalincrease.h"


namespace datamodelserializer
{    
    class CTotalIncreaseSerializer : public gui::ISerializer
    {
    public:
        CTotalIncreaseSerializer(gui::IDataObject*,
                                 const gui::ISerializerFactory*,
                                 const gui::IObjectFactory*);

        std::wstring toString() const override;
        void fromString(std::wstring fromString) override;

    private:
        gui::ITotalIncrease *m_pTotalIncrease = nullptr;
        const gui::ISerializerFactory *m_pSerializerFactory = nullptr;
        const gui::IObjectFactory *m_pObjectFactory = nullptr;
    };
}

#endif // CTOTALINCREASESERIALIZER_H
