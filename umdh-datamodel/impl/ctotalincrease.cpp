#include "ctotalincrease.h"
#include "customuniqueptr.h"

#pragma warning(push, 0)
#include <cassert>
#pragma warning(pop)

using namespace datamodel;

CTotalIncrease::CTotalIncrease(const gui::IObjectFactory *pObjectFactory, std::wstring typeId)
    :
      m_pObjectFactory(pObjectFactory),
      m_typeId(typeId)
{
    assert(m_pObjectFactory);
    static_assert(std::is_copy_constructible<CopyableState>::value,
            "CTotalIncrease.CopyableState is not copyable!");
}

std::wstring CTotalIncrease::type() const
{
    return m_typeId;
}

gui::unique_ptr<gui::IClonable> CTotalIncrease::clone() const
{
    auto pTotalIncrease = gui::dynamic_unique_cast<CTotalIncrease>(m_pObjectFactory->get(type()));
    pTotalIncrease->m_pObjectFactory = m_pObjectFactory;
    pTotalIncrease->m_copyableState = m_copyableState;
    return pTotalIncrease;
}

void CTotalIncrease::setRequested(size_t requested)
{
    m_copyableState.requested = requested;
}

size_t CTotalIncrease::getRequested() const
{
    return m_copyableState.requested;
}

void CTotalIncrease::setOverhead(size_t overhead)
{
    m_copyableState.overhead = overhead;
}

size_t CTotalIncrease::getOverhead() const
{
    return m_copyableState.overhead;
}
