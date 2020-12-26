#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace datamodelserializer
{
    std::wstring toHexStr(size_t integer, bool inUpperCase = false);
    size_t fromHexStr(std::wstring integer);

    std::wstring getTrimmedString(std::wstring raw);
    std::wstring getTrimmedByLinesString(std::wstring raw);

    std::wstring getWithoutLinesBeginsFrom(std::wstring raw, std::wstring pattern = L"//");
    std::wstring getWithoutSameBlankSymbolsInARow(std::wstring raw);

    std::vector<std::wstring> splitByLines(std::wstring raw);
}

#endif // UTILS_H
