#include "cstacktraceframeserializer.h"
#include "cforwardparser.h"
#include "utils.h"
#include "metamodel.h"

#pragma warning(push, 0)
#include <cassert>
#pragma warning(pop)

using namespace datamodelserializer;


CStackTraceFrameSerializer::CStackTraceFrameSerializer(gui::IDataObject *pStackTraceFrame,
                                                       const gui::ISerializerFactory *pSerializerFactory,
                                                       const gui::IObjectFactory *pObjectFactory)
    :
      m_pStackTraceFrame(dynamic_cast<gui::IStackTraceFrame*>(pStackTraceFrame)),
      m_pSerializerFactory(pSerializerFactory),
      m_pObjectFactory(pObjectFactory)

{
    assert(m_pStackTraceFrame);
    assert(m_pSerializerFactory);
    assert(m_pObjectFactory);
}

std::wstring CStackTraceFrameSerializer::toString() const
{
    std::wstring result =
            m_pStackTraceFrame->getModule() + L'!' +
            m_pStackTraceFrame->getFunction() + L'+' +
            toHexStr(m_pStackTraceFrame->getOffset(), true);

    if (!m_pStackTraceFrame->getSource().empty())
    {
        result += L" (" +
                m_pStackTraceFrame->getSource() + L", " +
                std::to_wstring(m_pStackTraceFrame->getSourceLine()) + L")";
    }

    if (!m_pStackTraceFrame->getUnknownAddress().empty())
    {
        result += L" : " + m_pStackTraceFrame->getUnknownAddress();
    }

    return result;
}

void CStackTraceFrameSerializer::fromString(std::wstring fromString)
{
    std::vector<gui::unique_ptr<ITokenParser>> m_tokensGetters;

    struct ModuleParser : BaseTokenParser
    {
        ModuleParser(gui::IStackTraceFrame *pStackTraceFrame)
        {
            setTokenGetter([pStackTraceFrame] (const std::wstring& token, size_t, const std::wstring&)
            {
                pStackTraceFrame->setModule(getTrimmedString(std::move(token)));
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L'!';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<ModuleParser>(m_pStackTraceFrame));

    struct FunctionParser : BaseTokenParser
    {
        FunctionParser(gui::IStackTraceFrame *pStackTraceFrame)
        {
            setTokenGetter([pStackTraceFrame] (const std::wstring& token, size_t, const std::wstring&)
            {
                pStackTraceFrame->setFunction(getTrimmedString(std::move(token)));
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L'+' || (i < stringData.size() - 1 && stringData[i] == L':' && stringData[i + 1] == L' ');
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<FunctionParser>(m_pStackTraceFrame));

    struct OffsetParser : BaseTokenParser
    {
        OffsetParser(gui::IStackTraceFrame *pStackTraceFrame)
        {
            setTokenGetter([pStackTraceFrame] (const std::wstring& token, size_t, const std::wstring&)
            {
                pStackTraceFrame->setOffset(fromHexStr(getTrimmedString(std::move(token))));
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L' ' || stringData[i] == L'\n' || i == stringData.size() - 1;
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<OffsetParser>(m_pStackTraceFrame));

    struct SourceOrUnknownModuleParser : BaseTokenParser
    {
        SourceOrUnknownModuleParser(gui::IStackTraceFrame *pStackTraceFrame)
        {
            setTokenGetter([pStackTraceFrame] (const std::wstring& token, size_t from, const std::wstring& stringData)
            {
                if (from + token.size() == stringData.size())
                    pStackTraceFrame->setUnknownAddress(getTrimmedString(token.substr(1, token.size() - 1)));
                else
                    pStackTraceFrame->setSource(getTrimmedString(token.substr(1, token.size() - 1)));
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L',' || i == stringData.size() - 1;
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<SourceOrUnknownModuleParser>(m_pStackTraceFrame));

    struct SourceLineParser : BaseTokenParser
    {
        SourceLineParser(gui::IStackTraceFrame *pStackTraceFrame)
        {
            setTokenGetter([pStackTraceFrame] (const std::wstring& token, size_t, const std::wstring&)
            {
                pStackTraceFrame->setSourceLine(_wtol(getTrimmedString(std::move(token)).c_str()));
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L')';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<SourceLineParser>(m_pStackTraceFrame));

    readString(fromString, std::move(m_tokensGetters));
}
