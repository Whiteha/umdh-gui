#include "csettings.h"

#pragma warning(push, 0)
#include <cassert>
#pragma warning(pop)

using namespace datamodel;

CSettings::CSettings(const gui::IObjectFactory *pObjectFactory, std::wstring typeId)
    :
      m_pObjectFactory(pObjectFactory),
      m_typeId(std::move(typeId))
{
    assert(m_pObjectFactory);
    static_assert(std::is_copy_constructible<CopyableState>::value,
            "CSettings.CopyableState is not copyable!");
}

std::wstring CSettings::type() const
{
    return m_typeId;
}

gui::unique_ptr<gui::IClonable> CSettings::clone() const
{
    auto pResult = gui::dynamic_unique_cast<CSettings>(m_pObjectFactory->get(type()));
    pResult->m_pObjectFactory = m_pObjectFactory;
    pResult->m_copyableState = m_copyableState;
    pResult->m_notifyer = m_notifyer;
    return pResult;
}

void CSettings::setUmdhPath(std::wstring umdhPath)
{
    if (m_copyableState.umdhPath == umdhPath)
        return;

    m_copyableState.umdhPath = std::move(umdhPath);
    m_notifyer();
}

std::wstring CSettings::getUmdhPath() const
{
    return m_copyableState.umdhPath;
}

void CSettings::setNtSymbolPath(std::wstring ntSymbolPath)
{
    if (m_copyableState.ntSymbolPath == ntSymbolPath)
        return;

    m_copyableState.ntSymbolPath = std::move(ntSymbolPath);
    m_notifyer();
}

std::wstring CSettings::getNtSymbolPath() const
{
    return m_copyableState.ntSymbolPath;
}

void CSettings::setFolderForSnapshots(std::wstring folderForSnapshots)
{
    if (m_copyableState.folderForSnapshots == folderForSnapshots)
        return;

    m_copyableState.folderForSnapshots = std::move(folderForSnapshots);
    m_notifyer();
}

std::wstring CSettings::getFolderForSnapshots() const
{
    return m_copyableState.folderForSnapshots;
}

void CSettings::setTargetProgramm(std::wstring targetProgramm)
{
    if (m_copyableState.targetProgramm == targetProgramm)
        return;

    m_copyableState.targetProgramm = std::move(targetProgramm);
    m_notifyer();
}

std::wstring CSettings::getTargetProgramm() const
{
    return m_copyableState.targetProgramm;
}

void CSettings::setPID(std::wstring pid)
{
    if (m_copyableState.pid == pid)
        return;

    m_copyableState.pid = std::move(pid);
    m_notifyer();
}

std::wstring CSettings::getPID() const
{
    return m_copyableState.pid;
}

void CSettings::setUpdateNotifier(SettingsObserver notifyer)
{
    m_notifyer = notifyer;
}
