#ifndef CREPORTMANAGER_H
#define CREPORTMANAGER_H

#include "idatamodel/iobjectfactory.h"
#include "idatamodelserializer/iserializerfactory.h"
#include "ilogic/ireportmanager.h"

#include "ihotpathcalculator.h"

namespace logic
{
    class CReportManager : public gui::IReportManager
    {
    public:
        CReportManager(const gui::IObjectFactory*,
                       const gui::ISerializerFactory*,
                       gui::unique_ptr<IHotPathCalculator>);

        void setSourceReport(std::wstring pathToReport) override;
        std::wstring getSourceReport() const override;

        void setFilter(gui::unique_ptr<gui::IReportFilter>) override;
        void setSorter(gui::unique_ptr<gui::IReportSorter>) override;

        std::wstring getProcessedReport(gui::ProgressNotifier) override;
        gui::StackFrameDisplayDataList getProcessedReportList(gui::ProgressNotifier) override;
        gui::CallTree getProcessedReportCallTree(gui::ProgressNotifier) override;

        void setUpdateNotifier(gui::ReportManagerObserver) override;

        size_t getUniqueStacksCount() const override;
        size_t getAllocationsCount() const override;
        size_t getAllocatedBytes() const override;

        gui::CallsByCountMap calculateCallsCount(gui::ProgressNotifier progressNotifier) override;

    private:
        void reloadReportIfNeeded();
        void resetReportChangeTriggers();
        gui::unique_ptr<gui::IReport> readReportFromFile() const;
        void applyFilter();
        void applySorter();
        void postProcessing();
        void processReport(gui::ProgressNotifier progressNotifier);
        void processReportBase(gui::ProgressNotifier progressNotifier, bool needPostProcessing);
        
        void addLogEntryToCallTree(const gui::unique_ptr<gui::ILogEntry> &pLogEntry,
                                   const gui::CallTreePathId &currentLogEntryId, 
                                   gui::CallTree &callTree) const;

        std::wstring getCallTreeItemReprFull(const std::vector<gui::unique_ptr<gui::IStackTraceFrame>> &pStackTrace, const ptrdiff_t &i) const;
        void updatedRecord(gui::CallTree &callTreeToUpdate,                           
                           const std::vector<gui::unique_ptr<gui::IStackTraceFrame>> &pStackTrace,
                           const ptrdiff_t &i,
                           const gui::CallTreePathId &currentLogEntryId,
                           const std::wstring &functionRepresentation,
                           const std::wstring &functionRepresentationNext,
                           const ptrdiff_t &bytesDiff,
                           const ptrdiff_t &totalCountDiff) const;
    private:
        std::wstring m_pathToReport;
        bool m_pathToReportChanged = false;
        gui::unique_ptr<gui::IReport> m_pSourceReport;
        gui::unique_ptr<gui::IReport> m_pReport;
        gui::ReportManagerObserver m_observer;
        gui::unique_ptr<gui::IReportFilter> m_pFilter;
        gui::unique_ptr<gui::IReportSorter> m_pSorter;
        const gui::IObjectFactory *m_pObjectFactory = nullptr;
        const gui::ISerializerFactory *m_pSerializerFactory = nullptr;

        size_t m_allocationsCount = 0;
        size_t m_allocatedBytes = 0;
        size_t m_uniqueStackTraceCount = 0;

        gui::unique_ptr<IHotPathCalculator> m_pHotPathCalculator;

        bool m_reportFilterChanged = false;
        bool m_reportSorterChanged = false;
    };
}
#endif // CREPORTMANAGER_H
