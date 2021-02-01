#include "ctotalincreaseserializer.h"
#include "cforwardparser.h"
#include "utils.h"
#include "metamodel.h"

#pragma warning(push, 0)
#include <cassert>
#pragma warning(pop)

using namespace datamodelserializer;


CTotalIncreaseSerializer::CTotalIncreaseSerializer(gui::IDataObject *pTotalIncrease,
                                                   const gui::ISerializerFactory *pSerializerFactory,
                                                   const gui::IObjectFactory *pObjectFactory)
    :
      m_pTotalIncrease(dynamic_cast<gui::ITotalIncrease*>(pTotalIncrease)),
      m_pSerializerFactory(pSerializerFactory),
      m_pObjectFactory(pObjectFactory)

{
    assert(m_pTotalIncrease);
    assert(m_pSerializerFactory);
    assert(m_pObjectFactory);
}

std::wstring CTotalIncreaseSerializer::toString() const
{
    std::wstring result;
    result += L"Total increase == " + toHexStr(m_pTotalIncrease->getRequested()) +
            L" requested +   " + toHexStr(m_pTotalIncrease->getOverhead()) +
            L" overhead = " + toHexStr(m_pTotalIncrease->getRequested() + m_pTotalIncrease->getOverhead());
    return result;
}

void CTotalIncreaseSerializer::fromString(std::wstring fromString)
{
    std::vector<gui::unique_ptr<ITokenParser>> m_tokensGetters;

    struct SkeepUntilRequestedStub : BaseTokenParser
    {
        SkeepUntilRequestedStub()
        {
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L'=';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<SkeepUntilRequestedStub>());

    struct RequestedParser : BaseTokenParser
    {
        RequestedParser(gui::ITotalIncrease *pTotalIncrease)
        {
            setTokenGetter([pTotalIncrease] (const std::wstring& token, size_t, const std::wstring&)
            {
                pTotalIncrease->setRequested(fromHexStr(getTrimmedString(std::move(token))));
            });
            setStartTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] != L' ' && stringData[i] != L'=';
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L' ';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<RequestedParser>(m_pTotalIncrease));

    struct SkeepUntilOverheadStub : BaseTokenParser
    {
        SkeepUntilOverheadStub()
        {
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L'+';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<SkeepUntilOverheadStub>());

    struct OverheadParser : BaseTokenParser
    {
        OverheadParser(gui::ITotalIncrease *pTotalIncrease)
        {
            setTokenGetter([pTotalIncrease] (const std::wstring& token, size_t, const std::wstring&)
            {
                pTotalIncrease->setOverhead(fromHexStr(getTrimmedString(std::move(token))));
            });
            setEndTokenCondition([] (const std::wstring& stringData, size_t i)
            {
                return stringData[i] == L' ';
            });
        }
    };
    m_tokensGetters.push_back(gui::make_unique<OverheadParser>(m_pTotalIncrease));

    readString(fromString, std::move(m_tokensGetters));
}
