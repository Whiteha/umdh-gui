#include "creport.h"

#pragma warning(push, 0)
#include <cassert>
#pragma warning(pop)

using namespace datamodel;


CReport::CReport(const gui::IObjectFactory *pObjectFactory, std::wstring typeId)
    :
      m_pObjectFactory(pObjectFactory),
      m_typeId(std::move(typeId))
{
    assert(m_pObjectFactory);
}

std::wstring CReport::type() const
{
    return m_typeId;
}

gui::unique_ptr<gui::IClonable> CReport::clone() const
{
    auto pReport = gui::dynamic_unique_cast<CReport>(m_pObjectFactory->get(type()));
    pReport->m_pObjectFactory = m_pObjectFactory;
    pReport->m_loadedModules = getLoadedModules();
    pReport->m_logEntries = getLogEntries();
    pReport->m_totalIncrease = getTotalIncrease();
    return pReport;
}

void CReport::setLoadedModules(std::vector<gui::unique_ptr<gui::ILoadedModule>> loadedModules)
{
    m_loadedModules = std::move(loadedModules);
}

std::vector<gui::unique_ptr<gui::ILoadedModule>> CReport::getLoadedModules() const
{
    std::vector<gui::unique_ptr<gui::ILoadedModule>> result;
    for (auto& pLoadedModules : m_loadedModules)
    {
        auto pClonableLoadedModules = dynamic_cast<IClonable*>(pLoadedModules.get());
        result.push_back(gui::dynamic_unique_cast<gui::ILoadedModule>(pClonableLoadedModules->clone()));
    }
    return result;
}

void CReport::setLogEntries(std::vector<gui::unique_ptr<gui::ILogEntry>> logEntries)
{
    m_logEntries = std::move(logEntries);
}

std::vector<gui::unique_ptr<gui::ILogEntry>> CReport::getLogEntries() const
{
    std::vector<gui::unique_ptr<gui::ILogEntry>> result;
    for (auto& pLogEntry : m_logEntries)
    {
        auto pClonableLogEntry = dynamic_cast<IClonable*>(pLogEntry.get());
        result.push_back(gui::dynamic_unique_cast<gui::ILogEntry>(pClonableLogEntry->clone()));
    }
    return result;
}

void CReport::setTotalIncrease(gui::unique_ptr<gui::ITotalIncrease> totalIncrease)
{
    m_totalIncrease = std::move(totalIncrease);
}

gui::unique_ptr<gui::ITotalIncrease> CReport::getTotalIncrease() const
{
    if (!m_totalIncrease)
        return nullptr;

    auto pClonableTotalIncrease = dynamic_cast<IClonable*>(m_totalIncrease.get());
    return gui::dynamic_unique_cast<gui::ITotalIncrease>(pClonableTotalIncrease->clone());
}
