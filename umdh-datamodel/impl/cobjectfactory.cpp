#include "cobjectfactory.h"

#include "cloadedmodule.h"
#include "ctotalincrease.h"
#include "cstacktraceframe.h"
#include "creport.h"
#include "clogentry.h"
#include "csettings.h"

#include "metamodel.h"

#pragma warning(push, 0)
#include <unordered_map>
#include <functional>
#pragma warning(pop)

using namespace datamodel;

namespace
{
    template <class T>
    std::pair<std::wstring, std::function<gui::unique_ptr<gui::IDataObject>(const gui::IObjectFactory*)>>
    addObjectCreator(std::wstring typeId)
    {
        return { typeId, [typeId] (const auto* pObjectFactory) { return gui::make_unique<T>(pObjectFactory, typeId); }};
    }

    auto getObjectTypeMap()
    {
        static std::unordered_map<std::wstring, std::function<gui::unique_ptr<gui::IDataObject>(const gui::IObjectFactory*)>> result =
        {
            addObjectCreator<CLoadedModule>(cLoadedModuleType),
            addObjectCreator<CTotalIncrease>(cTotalIncreaseType),
            addObjectCreator<CStackTraceFrame>(cStackFrameType),
            addObjectCreator<CReport>(cReportType),
            addObjectCreator<CLogEntry>(cLogEntryType),
            addObjectCreator<CSettings>(cSettingsType)
        };
        return result;
    }
}

gui::unique_ptr<gui::IDataObject> CObjectFactory::get(std::wstring typeId) const
{
    const auto &typeGetterMap = getObjectTypeMap();
    return typeGetterMap.at(typeId)(this);
}
