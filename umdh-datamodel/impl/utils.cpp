#include "utils.h"

#include <iomanip>
#include <sstream>

template <class IntegerType>
std::wstring toHexStrImpl(IntegerType integer, bool inUpperCase)
{
    std::wstringstream stream;
    if (inUpperCase)
        stream << std::uppercase;

    stream << std::hex <<  integer;
    return stream.str();
}

std::wstring toHexStr(size_t integer, bool inUpperCase)
{
    return toHexStrImpl(integer, inUpperCase);
};

std::wstring toHexStrSigned(long long integer, bool inUpperCase)
{
    return toHexStrImpl(integer, inUpperCase);
}

size_t fromHexStr(const std::wstring& integer)
{
    std::wstringstream ss;
    ss << std::hex << integer;
    size_t result = 0;
    ss >> result;
    return result;
};

const auto ws = L" \t\n\r\f\v";

// trim from end of string (right)
std::wstring& rtrim(std::wstring& s, const wchar_t* t = ws)
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from beginning of string (left)
std::wstring& ltrim(std::wstring& s, const wchar_t* t = ws)
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from both ends of string (right then left)
std::wstring& trim(std::wstring& s, const wchar_t* t = ws)
{
    return ltrim(rtrim(s, t), t);
}

std::wstring getTrimmedString(const std::wstring& raw)
{
    std::wstring result(raw);
    trim(result);
    return result;
}

std::wstring getTrimmedByLinesString(const std::wstring& raw)
{
    std::wstring result;
    std::wistringstream input;
    input.str(raw.c_str());

    for (std::wstring line; std::getline(input, line); )
    {
        const std::wstring& trimmed = getTrimmedString(line);
        result += trimmed + L'\n';
    }
    result.pop_back();

    return result;
}

std::vector<std::wstring> splitByLines(const std::wstring &raw)
{
    std::vector<std::wstring> result;

    std::wistringstream input;
    input.str(raw.c_str());

    for (std::wstring line; std::getline(input, line); )
    {
        result.push_back(line);
    }

    return result;
}

std::wstring getWithoutLinesBeginsFrom(const std::wstring &raw, const std::wstring &pattern)
{
    std::wstring result;
    std::wistringstream input;
    input.str(raw.c_str());

    for (std::wstring line; std::getline(input, line); )
    {
        const std::wstring& trimmed = getTrimmedString(line);
        if (pattern.size() <= trimmed.size())
        {
            if (pattern == trimmed.substr(0, pattern.size()))
                continue;
        }

        result += line + L'\n';
    }
    result.pop_back();

    return result;
}

std::wstring getWithoutSameBlankSymbolsInARow(const std::wstring &raw)
{
    std::wstring result;
    result.reserve(raw.size());

    for (size_t i = 0; i < raw.size(); ++i)
    {
        if (result.size() < raw.size())
        {
            if ((result.back() == raw[i]) && ((raw[i] == L' ') || (raw[i] == L'\t') || (raw[i] == L'\n')))
                continue;
        }

        result.push_back(raw[i]);
    }

    return result;
}
