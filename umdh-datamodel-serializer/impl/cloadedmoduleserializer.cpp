#include "cloadedmoduleserializer.h"
#include "cforwardparser.h"
#include "utils.h"
#include "metamodel.h"

#pragma warning(push, 0)
#include <cassert>
#pragma warning(pop)

using namespace datamodelserializer;


CLoadedModuleSerializer::CLoadedModuleSerializer(gui::IDataObject *pLoadedModule,
                                                 const gui::ISerializerFactory *pSerializerFactory,
                                                 const gui::IObjectFactory *pObjectFactory)
    :
      m_pLoadedModule(dynamic_cast<gui::ILoadedModule*>(pLoadedModule)),
      m_pSerializerFactory(pSerializerFactory),
      m_pObjectFactory(pObjectFactory)

{
    assert(m_pLoadedModule);
    assert(m_pSerializerFactory);
    assert(m_pObjectFactory);
}

std::wstring CLoadedModuleSerializer::toString() const
{
    std::wstring result;
    result += toHexStr(m_pLoadedModule->getAddrBegin(), true) + L'-'
              + toHexStr(m_pLoadedModule->getAddrEnd(), true) + L" DBGHELP: "
              + m_pLoadedModule->getModuleName() + L" - ";
    if (m_pLoadedModule->getIsSymbolsLoaded())
    {
        if (m_pLoadedModule->getSymbolsPath().empty())
        {
            result += L"export symbols";
        }
        else
        {
            result += L"private symbols & lines\n        " + m_pLoadedModule->getSymbolsPath();
        }
    }
    else
    {
        result += L"no symbols loaded";
    }
    return result;
}

void CLoadedModuleSerializer::fromString(std::wstring fromString)
{
    std::vector<gui::unique_ptr<ITokenParser>> m_tokensGetters;

    struct AddrBeginParser : BaseTokenParser
    {
        AddrBeginParser(gui::ILoadedModule* pLoadedModule)
        {
            setTokenGetter([pLoadedModule] (const std::wstring& token, size_t, const std::wstring&)
            {
                pLoadedModule->setAddrBegin(fromHexStr(getTrimmedString(std::move(token))));
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L'-';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<AddrBeginParser>(m_pLoadedModule));

    struct AddrEndParser : BaseTokenParser
    {
        AddrEndParser(gui::ILoadedModule* pLoadedModule)
        {
            setTokenGetter([pLoadedModule] (const std::wstring& token, size_t, const std::wstring&)
            {
                pLoadedModule->setAddrEnd(fromHexStr(getTrimmedString(std::move(token))));
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L' ';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<AddrEndParser>(m_pLoadedModule));

    struct ModuleParser : BaseTokenParser
    {
        ModuleParser(gui::ILoadedModule* pLoadedModule)
        {
            setTokenGetter([pLoadedModule] (const std::wstring& token, size_t, const std::wstring&)
            {
                pLoadedModule->setModuleName(getTrimmedString(std::move(token)));
            });
            setStartTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L' ';
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L'-';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<ModuleParser>(m_pLoadedModule));

    struct SymbolsLoadedParser : BaseTokenParser
    {
        SymbolsLoadedParser(gui::ILoadedModule* pLoadedModule)
        {
            setTokenGetter([pLoadedModule] (const std::wstring& token, size_t, const std::wstring&)
            {
                pLoadedModule->setIsSymbolsLoaded(getTrimmedString(std::move(token)) != L"no symbols loaded");
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L'\n' || (i == stringData.size() - 1);
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<SymbolsLoadedParser>(m_pLoadedModule));

    struct SymbolsPathParser : BaseTokenParser
    {
        SymbolsPathParser(gui::ILoadedModule* pLoadedModule)
        {
            setTokenGetter([pLoadedModule] (const std::wstring& token, size_t, const std::wstring&)
            {
                pLoadedModule->setSymbolsPath(getTrimmedString(std::move(token)));
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L'\n' || (i == stringData.size() - 1);
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<SymbolsPathParser>(m_pLoadedModule));

    readString(fromString, std::move(m_tokensGetters));
}
