#include "csettingsserializer.h"
#include "cforwardparser.h"
#include "utils.h"
#include "metamodel.h"

#pragma warning(push, 0)
#include <cassert>
#pragma warning(pop)

using namespace datamodelserializer;


CSettingsSerializer::CSettingsSerializer(gui::IDataObject *pSettings,
                                         const gui::ISerializerFactory *pSerializerFactory,
                                         const gui::IObjectFactory *pObjectFactory)
    :
      m_pSettings(dynamic_cast<gui::ISettings*>(pSettings)),
      m_pSerializerFactory(pSerializerFactory),
      m_pObjectFactory(pObjectFactory)

{
    assert(m_pSettings);
    assert(m_pSerializerFactory);
    assert(m_pObjectFactory);
}

std::wstring CSettingsSerializer::toString() const
{
    std::wstring result;
    result += m_pSettings->getUmdhPath() + L'\n' +
              m_pSettings->getNtSymbolPath() + L'\n' +
              m_pSettings->getFolderForSnapshots() + L'\n' +
              m_pSettings->getTargetProgramm()  + L'\n';
    return result;
}

void CSettingsSerializer::fromString(std::wstring fromString)
{
    auto splittedString = splitByLines(fromString);
    m_pSettings->setUmdhPath(splittedString[0]);
    m_pSettings->setNtSymbolPath(splittedString[1]);
    m_pSettings->setFolderForSnapshots(splittedString[2]);
    m_pSettings->setTargetProgramm(splittedString[3]);
}
