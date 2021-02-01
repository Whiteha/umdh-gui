#include "chotpathcalculator.h"

#pragma warning(push, 0)
#include <string>
#include <map>
#include <cassert>
#pragma warning(pop)

logic::CHotPathCalculator::CHotPathCalculator(const gui::ISerializerFactory *pSerializerFactory) :
    m_pSerializerFactory(pSerializerFactory)
{
    assert(m_pSerializerFactory);
}

gui::CallsByCountMap logic::CHotPathCalculator::calculateCallsCount(gui::IReport* pReport) const
{
    std::map<std::wstring, std::ptrdiff_t> preResult;
    auto logEntries = pReport->getLogEntries();
    for (auto &logEntry : logEntries)
    {
        auto stackTrace = logEntry->getStackTrace();
        for (size_t i = 0 ; i < stackTrace.size(); ++i)
        {
            auto serializer = m_pSerializerFactory->get(dynamic_cast<gui::IDataObject*>(stackTrace[i].get()));
            std::wstring call = serializer->toString();
            preResult[call] += static_cast<std::ptrdiff_t>(logEntry->getNewCount()) - static_cast<std::ptrdiff_t>(logEntry->getOldCount());
        }
    }

    std::wstring result;
    gui::CallsByCountMap preResultSorted;
    for (const auto &function : preResult)
    {
        preResultSorted.insert(std::make_pair(function.second, function.first));
    }
    return preResultSorted;
}
