#include "cloadedmodule.h"

#pragma warning(push, 0)
#include <cassert>
#pragma warning(pop)

using namespace datamodel;


CLoadedModule::CLoadedModule(const gui::IObjectFactory *pObjectFactory, std::wstring typeId)
    :
      m_pObjectFactory(pObjectFactory),
      m_typeId(std::move(typeId))
{
    assert(m_pObjectFactory);
    static_assert(std::is_copy_constructible<CopyableState>::value,
            "CLoadedModule.CopyableState is not copyable!");
}

std::wstring CLoadedModule::type() const
{
    return m_typeId;
}

gui::unique_ptr<gui::IClonable> CLoadedModule::clone() const
{
    auto pLoadedModule = gui::dynamic_unique_cast<CLoadedModule>(m_pObjectFactory->get(type()));
    pLoadedModule->m_copyableState = m_copyableState;
    pLoadedModule->m_pObjectFactory = m_pObjectFactory;
    return std::move(pLoadedModule);
}

void CLoadedModule::setModuleName(std::wstring moduleName)
{
    m_copyableState.moduleName = std::move(moduleName);
}

std::wstring CLoadedModule::getModuleName() const
{
    return m_copyableState.moduleName;
}

void CLoadedModule::setAddrBegin(size_t addrBegin)
{
    m_copyableState.addrBegin = addrBegin;
}

size_t CLoadedModule::getAddrBegin() const
{
    return m_copyableState.addrBegin;
}

void CLoadedModule::setAddrEnd(size_t addrEnd)
{
    m_copyableState.addrEnd = addrEnd;
}

size_t CLoadedModule::getAddrEnd() const
{
    return m_copyableState.addrEnd;
}

void CLoadedModule::setIsSymbolsLoaded(bool isSymbolsLoaded)
{
    m_copyableState.isSymbolsLoaded = isSymbolsLoaded;
}

bool CLoadedModule::getIsSymbolsLoaded() const
{
    return m_copyableState.isSymbolsLoaded;
}

void CLoadedModule::setSymbolsPath(std::wstring symbolsPath)
{
    m_copyableState.symbolsPath = std::move(symbolsPath);
}

std::wstring CLoadedModule::getSymbolsPath() const
{
    return m_copyableState.symbolsPath;
}
