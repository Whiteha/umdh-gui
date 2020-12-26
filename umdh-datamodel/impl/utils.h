#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

#include "testtypes.h"

std::wstring toHexStr(size_t integer, bool inUpperCase = false);
size_t fromHexStr(const std::wstring& integer);

std::wstring getTrimmedString(const std::wstring& raw);
std::wstring getTrimmedByLinesString(const std::wstring& raw);
std::wstring getWithoutLinesBeginsFrom(const std::wstring& raw, const std::wstring& pattern = L"//");
std::wstring getWithoutSameBlankSymbolsInARow(const std::wstring& raw);
std::vector<std::wstring> splitByLines(const std::wstring &raw);

namespace utils
{
    TestResults runTests();
}

#endif // UTILS_H
