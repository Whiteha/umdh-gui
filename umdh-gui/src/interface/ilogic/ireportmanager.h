#ifndef IREPORTMANAGER_H
#define IREPORTMANAGER_H

#include "ireportfilter.h"
#include "ireportsorter.h"
#include "idatamodel/ireport.h"

#include <vector>
#include <string>
#include <functional>

namespace gui
{
    using ReportManagerObserver = std::function<void()>;
    using ProgressNotifier = std::function<void(int)>;

    struct IReportManager
    {
        virtual ~IReportManager() = default;

        virtual void setSourceReport(std::wstring pathToReport) = 0;
        virtual std::wstring getSourceReport() const = 0;

        virtual void setFilter(gui::unique_ptr<IReportFilter>) = 0;
        virtual void setSorter(gui::unique_ptr<IReportSorter>) = 0;

        virtual std::wstring getProcessedReport(ProgressNotifier) = 0;

        virtual size_t getUniqueStacksCount() const = 0;
        virtual size_t getAllocationsCount() const = 0;
        virtual size_t getAllocatedBytes() const = 0;

        virtual void setUpdateNotifier(ReportManagerObserver) = 0;
    };
}

#endif // IREPORTMANAGER_H
