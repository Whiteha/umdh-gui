#ifndef IUMDH_H
#define IUMDH_H

#include <string>

namespace gui
{
    struct IUmdh
    {
        virtual ~IUmdh() = default;

        virtual void createSnapshot(std::wstring pathToSnapshot) = 0;
        virtual void createReport(std::wstring reportPath, std::wstring pathToSnapshotOld, std::wstring pathToSnapshotNew) = 0;
    };
}

#endif // IUMDH_H
