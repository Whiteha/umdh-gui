#ifndef GUIUTILS_H
#define GUIUTILS_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

namespace gui
{
    inline std::vector<std::wstring> splitByLines(const std::wstring &raw)
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
}

#endif // GUIUTILS_H
