#include "cforwardparser.h"
#include "utils.h"

using namespace datamodelserializer;

void datamodelserializer::readString(const std::wstring &data, std::vector<gui::unique_ptr<ITokenParser>> tokensGetters)
{
    bool token_start_inited = false;
    const std::wstring& string_data = getTrimmedString(data);
    for (size_t current_pos = 0, token_start = 0, token_id = 0;
         (current_pos < string_data.size()) && (token_id < tokensGetters.size());
         ++current_pos)
    {
        auto& tokenGetter = tokensGetters[token_id];
        token_id += tokenGetter->processStep(string_data, current_pos, token_start_inited, token_start);
    }
}

size_t BaseTokenParser::processStep(const std::wstring &stringData, size_t currentPos, bool &tokenStartInited, size_t &tokenStart)
{
    if (!tokenStartInited && m_startTokenCondition(stringData, currentPos))
    {
        tokenStart = currentPos;
        tokenStartInited = true;
    }
    else if (tokenStartInited && m_endTokenCondition(stringData, currentPos))
    {
        tokenStartInited = false;

        int last_elem_fix = 0;
        if (currentPos == stringData.size() - 1)
            last_elem_fix = 1;

        --m_readNtimes;
        m_tokenGetter(stringData.substr(tokenStart, (currentPos + last_elem_fix) - tokenStart), tokenStart, stringData);

        if (m_readNtimes == 0)
            return 1;
    }

    return 0;
}

BaseTokenParser::BaseTokenParser() : m_readNtimes(1)
{
    m_startTokenCondition = [] (const std::wstring &stringData, size_t i)
    {
        return !isblank(stringData[i]);
    };
    m_endTokenCondition = [] (const std::wstring &stringData, size_t i)
    {
        return isblank(stringData[i]);
    };
    m_tokenGetter = [] (const std::wstring&, size_t, const std::wstring&)
    {
        return;
    };
}

void BaseTokenParser::setStartTokenCondition(std::function<bool (const std::wstring&, size_t)> startTokenCondition)
{
    m_startTokenCondition = startTokenCondition;
}

void BaseTokenParser::setEndTokenCondition(std::function<bool (const std::wstring&, size_t)> endTokenCondition)
{
    m_endTokenCondition = endTokenCondition;
}

void BaseTokenParser::setTokenGetter(std::function<void (const std::wstring&, size_t, const std::wstring&)> tokenGetter)
{
    m_tokenGetter = tokenGetter;
}

void BaseTokenParser::setTokenRepeat(long ntimes)
{
    m_readNtimes = ntimes;
}
