#ifndef CSTACKTRACEFRAMESERIALIZER_H
#define CSTACKTRACEFRAMESERIALIZER_H

#include "idatamodelserializer/iserializer.h"
#include "idatamodelserializer/iserializerfactory.h"

#include "idatamodel/iobjectfactory.h"
#include "idatamodel/idataobject.h"
#include "idatamodel/istacktraceframe.h"


namespace datamodelserializer
{    
    class CStackTraceFrameSerializer : public gui::ISerializer
    {
    public:
        CStackTraceFrameSerializer(gui::IDataObject*,
                                   const gui::ISerializerFactory*,
                                   const gui::IObjectFactory*);

        std::wstring toString() const override;
        void fromString(std::wstring fromString) override;

    private:
        gui::IStackTraceFrame *m_pStackTraceFrame = nullptr;
        const gui::ISerializerFactory *m_pSerializerFactory = nullptr;
        const gui::IObjectFactory *m_pObjectFactory = nullptr;
    };
}

#endif // CSTACKTRACEFRAMESERIALIZER_H
