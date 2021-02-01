#include "clogentryserializer.h"
#include "cforwardparser.h"
#include "utils.h"
#include "metamodel.h"

#pragma warning(push, 0)
#include <cassert>
#pragma warning(pop)

using namespace datamodelserializer;


CLogEntrySerializer::CLogEntrySerializer(gui::IDataObject *pLogEntry,
                                         const gui::ISerializerFactory *pSerializerFactory,
                                         const gui::IObjectFactory *pObjectFactory)
    :
      m_pLogEntry(dynamic_cast<gui::ILogEntry*>(pLogEntry)),
      m_pSerializerFactory(pSerializerFactory),
      m_pObjectFactory(pObjectFactory)
{
    assert(m_pLogEntry);
    assert(m_pSerializerFactory);
    assert(m_pObjectFactory);
}

std::wstring CLogEntrySerializer::toString() const
{
    std::wstring result;

    const long long bytes_delta = m_pLogEntry->getNewBytes() - m_pLogEntry->getOldBytes();
    if (bytes_delta >= 0)
        result += L"+\t";
    else
        result += L"-\t";
    result += toHexStr(std::abs(bytes_delta)) + L" (\t" +
              toHexStr(m_pLogEntry->getNewBytes()) + L" -\t" +
              toHexStr(m_pLogEntry->getOldBytes()) + L")\t" +
              toHexStr(m_pLogEntry->getNewCount()) + L" allocs\tBackTrace" +
              m_pLogEntry->getTraceId() + L'\n';

    const long long count_delta = m_pLogEntry->getNewCount() - m_pLogEntry->getOldCount();
    if (count_delta >= 0)
        result += L"+\t";
    else
        result += L"-\t";
    result += toHexStr(std::abs(count_delta)) + L" (\t" +
              toHexStr(m_pLogEntry->getNewCount()) + L" -\t" +
              toHexStr(m_pLogEntry->getOldCount()) + L")\t" +
              L"BackTrace" + m_pLogEntry->getTraceId() + L'\t'
              + L"allocations\n\n";

    for (const auto& stackTraceFrame : m_pLogEntry->getStackTrace())
    {
        auto pSerializer = m_pSerializerFactory->get(dynamic_cast<gui::IDataObject*>(stackTraceFrame.get()));
        result += L'\t' + pSerializer->toString() + L"\n";
    }

    return result;
}

void CLogEntrySerializer::fromString(std::wstring logEntryString)
{
    std::vector<gui::unique_ptr<ITokenParser>> m_tokensGetters;

    struct BytesDeltaStub : BaseTokenParser
    {
        BytesDeltaStub()
        {
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L'(';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<BytesDeltaStub>());

    struct NewBytesParser : BaseTokenParser
    {
        NewBytesParser(gui::ILogEntry *pLogEntry)
        {
            setTokenGetter([pLogEntry] (const std::wstring& token, size_t, const std::wstring&)
            {
                pLogEntry->setNewBytes(fromHexStr(getTrimmedString(std::move(token))));
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L'-' || stringData[i] == L'+';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<NewBytesParser>(m_pLogEntry));

    struct OldBytesParser : BaseTokenParser
    {
        OldBytesParser(gui::ILogEntry *pLogEntry)
        {
            setTokenGetter([pLogEntry] (const std::wstring& token, size_t, const std::wstring&)
            {
                pLogEntry->setOldBytes(fromHexStr(getTrimmedString(std::move(token))));
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L')';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<OldBytesParser>(m_pLogEntry));

    struct NewCountParser : BaseTokenParser
    {
        NewCountParser(gui::ILogEntry *pLogEntry)
        {
            setTokenGetter([pLogEntry] (const std::wstring& token, size_t, const std::wstring&)
            {
                pLogEntry->setNewCount(fromHexStr(getTrimmedString(std::move(token))));
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L' ';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<NewCountParser>(m_pLogEntry));

    struct TraceIdParser : BaseTokenParser
    {
        TraceIdParser(gui::ILogEntry *pLogEntry)
        {
            setTokenGetter([pLogEntry] (const std::wstring& token, size_t, const std::wstring&)
            {
                const std::ptrdiff_t offset = std::wstring(L" allocs	BackTrace").size() - 1;
                pLogEntry->setTraceId(getTrimmedString(token.substr(offset, token.size() - offset)));
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L'\n';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<TraceIdParser>(m_pLogEntry));

    struct CountDeltaStub : BaseTokenParser
    {
        CountDeltaStub()
        {
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L'(';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<CountDeltaStub>());

    struct NewCountStub : BaseTokenParser
    {
        NewCountStub()
        {
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L'-' || stringData[i] == L'+';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<NewCountStub>());

    struct OldCountParser : BaseTokenParser
    {
        OldCountParser(gui::ILogEntry *pLogEntry)
        {
            setTokenGetter([pLogEntry] (const std::wstring& token, size_t, const std::wstring&)
            {
                pLogEntry->setOldCount(fromHexStr(getTrimmedString(std::move(token))));
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L')';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<OldCountParser>(m_pLogEntry));

    struct TraceIdStub : BaseTokenParser
    {
        TraceIdStub()
        {
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L'\n';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<TraceIdStub>());

    auto stackTrace = m_pLogEntry->getStackTrace();
    struct BackTraceParser : BaseTokenParser
    {
        BackTraceParser(const gui::IObjectFactory *pObjectFactory, const gui::ISerializerFactory *pSerializerFactory, std::vector<gui::unique_ptr<gui::IStackTraceFrame>> &stackTrace)
        {
            setTokenGetter([this, pObjectFactory, pSerializerFactory, &stackTrace = stackTrace] (const std::wstring& token, size_t, const std::wstring&)
            {
                auto pStackTraceFrame = pObjectFactory->get(cStackFrameType);
                auto pStackTraceFrameSerializer = pSerializerFactory->get(pStackTraceFrame.get());
                pStackTraceFrameSerializer->fromString(getTrimmedString(std::move(token)));
                stackTrace.push_back(gui::dynamic_unique_cast<gui::IStackTraceFrame>(std::move(pStackTraceFrame)));
                setTokenRepeat(1);
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L'\n' || (i == stringData.size() - 1);
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<BackTraceParser>(m_pObjectFactory, m_pSerializerFactory, stackTrace));

    readString(logEntryString, std::move(m_tokensGetters));
    m_pLogEntry->setStackTrace(std::move(stackTrace));
}
