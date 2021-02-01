#include "cfilterfactory.h"

#include "cdefaultfilter.h"
#include "cwithsymbolsfilter.h"
#include "cwithsourcesfilter.h"
#include "cwithpatternfilter.h"
#include "cwithoutpatternfilter.h"
#include "cleakedmorethanbytesfilter.h"
#include "cleakedmorethancountfilter.h"
#include "cwithoutsystemmodulesfilter.h"
#include "metamodel.h"

#pragma warning(push, 0)
#include <cassert>
#include <unordered_map>
#include <functional>
#pragma warning(pop)


using namespace logic;

namespace
{
    auto getReportFiltersTypeMap()
    {
        static std::unordered_map<std::wstring, std::function<gui::unique_ptr<gui::IReportFilter>()>> result =
        {
            {cDefaultFilterType, [] { return gui::make_unique<CDefaultFilter>(); }},
            {cReportFilterWithoutPatternType, [] { return gui::make_unique<CWithoutPatternFilter>(); }},
            {cReportFilterWithSymbolsType, [] { return gui::make_unique<CWithSymbolsFilter>(); }},
            {cReportFilterWithSourcesType, [] { return gui::make_unique<CWithSourcesFilter>(); }},
            {cReportFilterWithPatternType, [] { return gui::make_unique<CWithPatternFilter>(); }},
            {cReportFilterLeakedBytesType, [] { return gui::make_unique<CLeakedMoreThanBytesFilter>(); }},
            {cReportFilterLeakedNTimesType, [] { return gui::make_unique<CLeakedMoreThanCountFilter>(); }},
            {cReportFilterWithoutSystemModulesType, [] { return gui::make_unique<CWithoutSystemModulesFilter>(); }}
        };

        return result;
    }
}


gui::unique_ptr<gui::IReportFilter> CFilterFactory::get(std::wstring typeId) const
{
    const auto &typeGetterMap = getReportFiltersTypeMap();
    const auto &it = typeGetterMap.find(typeId);
    if (it == typeGetterMap.cend())
        return typeGetterMap.at(cDefaultFilterType)();
    return (it->second)();
}
