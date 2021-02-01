#ifndef IREPORTMANAGER_H
#define IREPORTMANAGER_H

#include "ireportfilter.h"
#include "ireportsorter.h"
#include "idatamodel/ireport.h"

#pragma warning(push, 0)
#include <vector>
#include <map>
#include <set>
#include <list>
#include <string>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#pragma warning(pop)

namespace gui
{
    using ReportManagerObserver = std::function<void()>;
    using ProgressNotifier = std::function<void(int)>;
    using CallsByCountMap = std::multimap<std::ptrdiff_t, std::wstring>;

    struct StackFrameDisplayData
    {
        std::wstring title;
        std::wstring body;
    };
    using StackFrameDisplayDataList = std::list<StackFrameDisplayData>;

    using CallTreePathId = int;
    static const CallTreePathId CallTreeRootPathId = -1;
    using NextFunction = std::wstring;

    struct CallTreeItem
    {
        std::wstring functionRepresentation;
        std::wstring shortFunctionRepresentation;
        std::wstring fullFunctionRepresentation;

        std::ptrdiff_t allocatedBytes = 0;
        std::ptrdiff_t allocationCount = 0;

        std::unordered_set<CallTreePathId> startForPathId;
        std::unordered_set<CallTreePathId> endForPathId;
        std::unordered_set<CallTreePathId> partOfPathId;
    };
    inline bool operator<(const CallTreeItem& left, const CallTreeItem& right)
    {
        return left.functionRepresentation < right.functionRepresentation;
    }
    inline bool operator!=(const CallTreeItem& left, const CallTreeItem& right)
    {
        return left.functionRepresentation != right.functionRepresentation;
    }
    using CallTreeNextItemPath = std::unordered_map<CallTreePathId, NextFunction>;
    using CallTree = std::map<CallTreeItem, CallTreeNextItemPath>;

    struct IReportManager
    {
        virtual ~IReportManager() = default;

        virtual void setSourceReport(std::wstring pathToReport) = 0;
        virtual std::wstring getSourceReport() const = 0;

        virtual void setFilter(gui::unique_ptr<IReportFilter>) = 0;
        virtual void setSorter(gui::unique_ptr<IReportSorter>) = 0;

        virtual std::wstring getProcessedReport(ProgressNotifier) = 0;
        virtual StackFrameDisplayDataList getProcessedReportList(ProgressNotifier) = 0;
        virtual CallTree getProcessedReportCallTree(ProgressNotifier) = 0;

        virtual size_t getUniqueStacksCount() const = 0;
        virtual size_t getAllocationsCount() const = 0;
        virtual size_t getAllocatedBytes() const = 0;

        virtual CallsByCountMap calculateCallsCount(gui::ProgressNotifier progressNotifier) = 0;

        virtual void setUpdateNotifier(ReportManagerObserver) = 0;
    };
}

#endif // IREPORTMANAGER_H
