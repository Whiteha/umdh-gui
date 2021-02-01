#include "cumdh.h"

#pragma warning(push, 0)
#include <cassert>
#include <windows.h>
#pragma warning(pop)


using namespace logic;

namespace
{
    void executeUmdhCommand(std::wstring pathToApp, std::wstring args, std::wstring ntSymbolsPath = L"")
    {
        STARTUPINFO si;
        LPCTSTR lpAppName = pathToApp.c_str();
        args = L" " + args;
        LPTSTR lpArgs = _wcsdup(args.c_str());

        PROCESS_INFORMATION pi;
        DWORD exitCode = 0;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        ntSymbolsPath = L"_NT_SYMBOL_PATH=" + ntSymbolsPath;
        if (!CreateProcess(lpAppName,
                           lpArgs,
                           NULL,
                           NULL,
                           FALSE,
                           CREATE_NEW_CONSOLE,
                           NULL,
                           NULL,
                           &si,
                           &pi)
            )
        {
            return;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        free(lpArgs);

        if (!GetExitCodeProcess(pi.hProcess, &exitCode)) {
            return;
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    std::wstring wrap(std::wstring src)
    {
        return L"\"" + src + L"\"";
    }
}

CUmdh::CUmdh(gui::ISettings *pSettings) : m_pSettings(pSettings)
{
    assert(pSettings);
}

void CUmdh::createSnapshot(std::wstring pathToSnapshot)
{
    if (pathToSnapshot.empty())
    {
        return;
    }

    std::wstring command;

    if (m_pSettings->getPID().empty())
        command = L"-pn:\"" + m_pSettings->getTargetProgramm() + L"\" -f:" + wrap(pathToSnapshot);
    else
        command = L"-p:" + m_pSettings->getPID() + L" -f:" +  wrap(pathToSnapshot);

    executeUmdhCommand(m_pSettings->getUmdhPath(), command, m_pSettings->getNtSymbolPath());
}

void CUmdh::createReport(std::wstring reportPath, std::wstring pathToSnapshotOld, std::wstring pathToSnapshotNew)
{
    if (reportPath.empty() || pathToSnapshotOld.empty() || pathToSnapshotNew.empty())
    {
        return;
    }

    executeUmdhCommand(m_pSettings->getUmdhPath(),
                       wrap(pathToSnapshotOld) + L" " +
                       wrap(pathToSnapshotNew) + L" -f:" +
                       reportPath);
}
