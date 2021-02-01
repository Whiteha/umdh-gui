#include "clogentry.h"

#pragma warning(push, 0)
#include <cassert>
#pragma warning(pop)


using namespace datamodel;

CLogEntry::CLogEntry(const gui::IObjectFactory *pObjectFactory, std::wstring typeId)
    :
      m_pObjectFactory(pObjectFactory),
      m_typeId(std::move(typeId))
{
    assert(m_pObjectFactory);
    static_assert(std::is_copy_constructible<CopyableState>::value,
            "CLogEntry.CopyableState is not copyable!");
}

std::wstring CLogEntry::type()  const
{
    return m_typeId;
}

gui::unique_ptr<gui::IClonable> CLogEntry::clone() const
{
    auto pClone = gui::dynamic_unique_cast<CLogEntry>(m_pObjectFactory->get(type()));
    pClone->m_copyableState = m_copyableState;
    pClone->m_pObjectFactory = m_pObjectFactory;
    pClone->m_pStackTrace = getStackTrace();
    return pClone;
}

void CLogEntry::setTraceId(std::wstring traceId)
{
    m_copyableState.trace_id = std::move(traceId);
}

std::wstring CLogEntry::getTraceId() const
{
    return m_copyableState.trace_id;
}

void CLogEntry::setNewCount(size_t newCount)
{
    m_copyableState.new_count = newCount;
}

size_t CLogEntry::getNewCount() const
{
    return m_copyableState.new_count;
}

void CLogEntry::setOldCount(size_t oldCount)
{
    m_copyableState.old_count = std::move(oldCount);
}

size_t CLogEntry::getOldCount() const
{
    return m_copyableState.old_count;
}

void CLogEntry::setNewBytes(size_t newBytes)
{
    m_copyableState.new_bytes = newBytes;
}

size_t CLogEntry::getNewBytes() const
{
    return m_copyableState.new_bytes;
}

void CLogEntry::setOldBytes(size_t oldBytes)
{
    m_copyableState.old_bytes = oldBytes;
}

size_t CLogEntry::getOldBytes() const
{
    return m_copyableState.old_bytes;
}

void CLogEntry::setStackTrace(std::vector<gui::unique_ptr<gui::IStackTraceFrame>> stackTrace)
{
    m_pStackTrace = std::move(stackTrace);
}

std::vector<gui::unique_ptr<gui::IStackTraceFrame>> CLogEntry::getStackTrace() const
{
    std::vector<gui::unique_ptr<gui::IStackTraceFrame>> result;
    for (const auto& pLogEntry : m_pStackTrace)
    {
        auto pClonableLogEntry = dynamic_cast<gui::IClonable*>(pLogEntry.get());
        result.push_back(gui::dynamic_unique_cast<gui::IStackTraceFrame>(pClonableLogEntry->clone()));
    }
    return result;
}
