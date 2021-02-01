#include "cserializerfactory.h"

#include "cloadedmoduleserializer.h"
#include "clogentryserializer.h"
#include "creportserializer.h"
#include "csettingsserializer.h"
#include "cstacktraceframeserializer.h"
#include "ctotalincreaseserializer.h"

#include "metamodel.h"

#pragma warning(push, 0)
#include <unordered_map>
#include <functional>
#include <cassert>
#pragma warning(pop)

using namespace datamodelserializer;

namespace
{

    template <class T>
    std::pair<std::wstring, std::function<gui::unique_ptr<gui::ISerializer>(gui::IDataObject*,
                                                                            const gui::IObjectFactory*,
                                                                            const gui::ISerializerFactory*)>>
    addSerializerCreator(std::wstring typeId)
    {
        return {typeId, [] (gui::IDataObject* pDataObject,
                            const gui::IObjectFactory *pObjectFactory,
                            const gui::ISerializerFactory* pSerializerFactory)
        {
            return gui::make_unique<T>(pDataObject, pSerializerFactory, pObjectFactory);
        }};
    }

    auto getSerializerTypeMap()
    {
        static std::unordered_map<std::wstring,
                std::function<gui::unique_ptr<gui::ISerializer>(gui::IDataObject*,
                                                                const gui::IObjectFactory*,
                                                                const gui::ISerializerFactory*)>> result =
        {
            addSerializerCreator<CLoadedModuleSerializer>(cLoadedModuleType),
            addSerializerCreator<CLogEntrySerializer>(cLogEntryType),
            addSerializerCreator<CReportSerializer>(cReportType),
            addSerializerCreator<CSettingsSerializer>(cSettingsType),
            addSerializerCreator<CStackTraceFrameSerializer>(cStackFrameType),
            addSerializerCreator<CTotalIncreaseSerializer>(cTotalIncreaseType),
        };
        return result;
    }
}

CSerializerFactory::CSerializerFactory(const gui::IObjectFactory *pObjectFactory) :
    m_pObjectFactory(pObjectFactory)
{
    assert(m_pObjectFactory);
}

gui::unique_ptr<gui::ISerializer> CSerializerFactory::get(gui::IDataObject* pDataObject) const
{
    const auto& typeGetterMap = getSerializerTypeMap();
    return typeGetterMap.at(pDataObject->type())(pDataObject, m_pObjectFactory, this);
}
