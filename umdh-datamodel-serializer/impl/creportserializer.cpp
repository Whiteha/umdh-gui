#include "creportserializer.h"
#include "cforwardparser.h"
#include "utils.h"
#include "metamodel.h"

#pragma warning(push, 0)
#include <cassert>
#pragma warning(pop)

namespace
{
bool isEndedWithPattern(size_t position, const std::wstring& stringData, const std::wstring& pattern)
{
    bool result = false;
    if (position + pattern.size() < stringData.size())
    {
        result = true;
        result &= pattern == stringData.substr(position, pattern.size());
    }
    return result;
}
}

using namespace datamodelserializer;


CReportSerializer::CReportSerializer(gui::IDataObject *pReport,
                                     const gui::ISerializerFactory *pSerializerFactory,
                                     const gui::IObjectFactory *pObjectFactory)
    :
      m_pReport(dynamic_cast<gui::IReport*>(pReport)),
      m_pSerializerFactory(pSerializerFactory),
      m_pObjectFactory(pObjectFactory)

{
    assert(m_pReport);
    assert(m_pSerializerFactory);
    assert(m_pObjectFactory);
}

std::wstring CReportSerializer::toString() const
{
    std::wstring result;
    const auto& pLoadedModules = m_pReport->getLoadedModules();
    for (const auto& pLoadedModule : pLoadedModules)
    {
        const auto& pLoadedModuleSerializer = m_pSerializerFactory->get(dynamic_cast<gui::IDataObject*>(pLoadedModule.get()));
        result += pLoadedModuleSerializer->toString() + L'\n';
    }

    const auto& pLogEntries = m_pReport->getLogEntries();
    for (const auto& pLogEntry : pLogEntries)
    {
        const auto& pLogEntrySerializer = m_pSerializerFactory->get(dynamic_cast<gui::IDataObject*>(pLogEntry.get()));
        result += pLogEntrySerializer->toString() + L'\n';
    }

    const auto& pTotalIncrease = m_pReport->getTotalIncrease();
    if (pTotalIncrease)
    {
        const auto& pTotalIncreaseSerializer = m_pSerializerFactory->get(dynamic_cast<gui::IDataObject*>(pTotalIncrease.get()));
        result += pTotalIncreaseSerializer->toString() + L'\n';
    }
    return result;
}

void CReportSerializer::fromString(std::wstring fromString)
{
    std::vector<gui::unique_ptr<ITokenParser>> m_tokensGetters;

    struct LoadedModulesParser : BaseTokenParser
    {
        LoadedModulesParser(gui::IReport* pReport, const gui::ISerializerFactory *pSerializerFactory, const gui::IObjectFactory *pObjectFactory)
        {
            setTokenGetter([this, pReport, pSerializerFactory, pObjectFactory] (const std::wstring& token, size_t token_start, const std::wstring& stringData)
            {
                auto pLoadedModules = pReport->getLoadedModules();
                auto pNewLoadedModule = pObjectFactory->get(cLoadedModuleType);
                gui::readSerializedString(pSerializerFactory, pNewLoadedModule.get(), getTrimmedString(token));
                pLoadedModules.push_back(gui::dynamic_unique_cast<gui::ILoadedModule>(std::move(pNewLoadedModule)));
                pReport->setLoadedModules(std::move(pLoadedModules));

                const bool isEndedWithLogEntryToken = isEndedWithPattern(token_start + token.length(), stringData, L"\n+")
                        || isEndedWithPattern(token_start + token.length(), stringData, L"\n-");

                if (!isEndedWithLogEntryToken)
                    setTokenRepeat(1);
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L'\n';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<LoadedModulesParser>(m_pReport, m_pSerializerFactory, m_pObjectFactory));

    auto logEntries = m_pReport->getLogEntries();
    struct LogEntryParser : BaseTokenParser
    {
        LogEntryParser(const gui::ISerializerFactory *pSerializerFactory, const gui::IObjectFactory *pObjectFactory, std::vector<gui::unique_ptr<gui::ILogEntry>> &logEntries)
        {
            setTokenGetter([this, pSerializerFactory, pObjectFactory, &logEntries = logEntries] (const std::wstring& token, size_t token_start, const std::wstring& stringData)
            {
                auto pNewLogEntry = pObjectFactory->get(cLogEntryType);
                gui::readSerializedString(pSerializerFactory, pNewLogEntry.get(), getTrimmedString(token));
                logEntries.push_back(gui::dynamic_unique_cast<gui::ILogEntry>(std::move(pNewLogEntry)));

                const bool isTotalIncreaseNextToken = isEndedWithPattern(token_start + token.length(), stringData, L"\nTotal increase");
                if (!isTotalIncreaseNextToken)
                    setTokenRepeat(1);
            });
            setEndTokenCondition([this] (const std::wstring& stringData, size_t i)
            {
                const bool candidate = (isEndedWithPattern(i, stringData, L"\n+")
                                       || isEndedWithPattern(i, stringData, L"\n-"));
                if (candidate)
                {
                    m_isOdd = !m_isOdd;
                }
                return (candidate && !m_isOdd)
                        || isEndedWithPattern(i, stringData, L"\nTotal increase");
            });
        }
    private:
        bool m_isOdd = false;
    };
    m_tokensGetters.push_back(gui::make_unique<LogEntryParser>(m_pSerializerFactory, m_pObjectFactory, logEntries));

    struct TotalIncreaseParser : BaseTokenParser
    {
        TotalIncreaseParser(gui::IReport* pReport, const gui::ISerializerFactory *pSerializerFactory, const gui::IObjectFactory *pObjectFactory)
        {
            setTokenGetter([pReport, pSerializerFactory, pObjectFactory] (const std::wstring& token, size_t, const std::wstring&)
            {
                auto pTotalIncrease = pObjectFactory->get(cTotalIncreaseType);
                gui::readSerializedString(pSerializerFactory, pTotalIncrease.get(), getTrimmedString(token));
                pReport->setTotalIncrease(gui::dynamic_unique_cast<gui::ITotalIncrease>(std::move(pTotalIncrease)));
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return i == stringData.length() - 1;
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<TotalIncreaseParser>(m_pReport, m_pSerializerFactory, m_pObjectFactory));

    readString(getWithoutSameBlankSymbolsInARow(getWithoutLinesBeginsFrom(getTrimmedByLinesString(fromString))), std::move(m_tokensGetters));
    m_pReport->setLogEntries(std::move(logEntries));
}

