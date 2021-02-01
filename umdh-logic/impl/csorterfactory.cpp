#include "csorterfactory.h"
#include "cdefaultsorter.h"
#include "cbyallocationcountsorter.h"
#include "cbyallocatedbytessorter.h"
#include "metamodel.h"

#pragma warning(push, 0)
#include <cassert>
#include <unordered_map>
#include <functional>
#pragma warning(pop)


using namespace logic;

namespace
{
    auto getReportSorterTypeMap()
    {
        static std::unordered_map<std::wstring, std::function<gui::unique_ptr<gui::IReportSorter>()>> result =
        {
            {cDefaultSorterType, [] { return gui::make_unique<CDefaultSorter>(); }},
            {cReportSorterByAllocationCountType, [] { return gui::make_unique<CByAllocationCountSorter>(); }},
            {cReportSorterByAllocatedBytesType, [] { return gui::make_unique<CByAllocatedBytesSorter>(); }},
        };

        return result;
    }
}

gui::unique_ptr<gui::IReportSorter> CSorterFactory::get(std::wstring typeId) const
{
    const auto &typeGetterMap = getReportSorterTypeMap();
    const auto &it = typeGetterMap.find(typeId);
    if (it == typeGetterMap.cend())
        return typeGetterMap.at(cDefaultSorterType)();
    return (it->second)();
}
