#ifndef CREPORTMANAGER_H
#define CREPORTMANAGER_H

#include "idatamodel/iobjectfactory.h"
#include "idatamodelserializer/iserializerfactory.h"
#include "ilogic/ireportmanager.h"

namespace logic
{
    class CReportManager : public gui::IReportManager
    {
    public:
        CReportManager(gui::IObjectFactory*, gui::ISerializerFactory*);

        void setSourceReport(std::wstring pathToReport) override;
        std::wstring getSourceReport() const override;

        void setFilter(gui::unique_ptr<gui::IReportFilter>) override;
        void setSorter(gui::unique_ptr<gui::IReportSorter>) override;

        std::wstring getProcessedReport(gui::ProgressNotifier) override;

        void setUpdateNotifier(gui::ReportManagerObserver) override;

        size_t getUniqueStacksCount() const override;
        size_t getAllocationsCount() const override;
        size_t getAllocatedBytes() const override;

    private:
        void initReport();
        void applyFilter();
        void applySorter();
        void postProcessing();

    private:
        std::wstring m_pathToReport;
        bool m_pathToReportChanged = false;
        gui::unique_ptr<gui::IReport> m_pSourceReport;
        gui::unique_ptr<gui::IReport> m_pReport;
        gui::ReportManagerObserver m_observer;
        gui::unique_ptr<gui::IReportFilter> m_pFilter;
        gui::unique_ptr<gui::IReportSorter> m_pSorter;
        gui::IObjectFactory *m_pObjectFactory = nullptr;
        gui::ISerializerFactory *m_pSerializerFactory = nullptr;

        size_t m_allocationsCount = 0;
        size_t m_allocatedBytes = 0;
        size_t m_uniqueStackTraceCount = 0;
    };
}
#endif // CREPORTMANAGER_H
