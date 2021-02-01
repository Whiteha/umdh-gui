#include "cwithoutsystemmodulesfilter.h"
#include "guiutils.h"
#include "customuniqueptr.h"

#pragma warning(push, 0)
#include <string>
#include <vector>
#include <fstream>
#include <streambuf>
#include <Windows.h>
#pragma warning(pop)


using namespace logic;


namespace
{
    std::vector<std::wstring> getSystemModuleList()
    {
        char szModule[MAX_PATH];
        memset(&szModule[0], 0, (sizeof szModule));
        GetModuleFileNameA(NULL, szModule, sizeof szModule);
        std::string currentModuleFilename(szModule);

        if (currentModuleFilename.empty())
            return {};

        const auto& posToFilenameStart = currentModuleFilename.find_last_of('\\');
        const auto& pathToModule = currentModuleFilename.substr(0, currentModuleFilename.length() - (currentModuleFilename.length() - posToFilenameStart));

        std::wifstream systemModuleList(pathToModule + "/systemModulesToHide.txt");
        if (!systemModuleList)
            return {};

        std::wstring fileData((std::istreambuf_iterator<wchar_t>(systemModuleList)),
                              std::istreambuf_iterator<wchar_t>());

        return gui::splitByLines(fileData);
    }
}

CWithoutSystemModulesFilter::CWithoutSystemModulesFilter()
{
    auto systemModulesToIgnore = getSystemModuleList();
    auto filter = [systemModulesToIgnore = std::move(systemModulesToIgnore)] (gui::ILogEntry *pLogEntry)
    {
        auto stackTrace = pLogEntry->getStackTrace();
        std::vector<gui::unique_ptr<gui::IStackTraceFrame>> stackTraceFiltered;
        for (auto &pFrame : stackTrace)
        {
            bool isIgnoredModule = false;
            for (auto &ignoredModule : systemModulesToIgnore)
            {
                if (pFrame->getModule().find(ignoredModule) != std::wstring::npos)
                {
                    isIgnoredModule = true;
                    break;
                }
            }

            if (!isIgnoredModule)
                stackTraceFiltered.push_back(std::move(pFrame));
        }
        if (!stackTraceFiltered.empty())
        {
            pLogEntry->setStackTrace(std::move(stackTraceFiltered));
            return true;
        }
        return false;
    };
    setLogEntryFilters({filter});
}
