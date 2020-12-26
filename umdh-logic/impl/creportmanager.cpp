#include "creportmanager.h"
#include "idatamodel/iclonable.h"
#include "metamodel.h"

#include <cassert>
#include <fstream>

#include <codecvt>
#include <locale>

using namespace logic;

namespace
{
    std::string ws2s(const std::wstring &wstr)
    {
        using convert_type = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_type, wchar_t> converter;
        return converter.to_bytes(wstr);
    }

    std::wstring getFileContent(std::wstring filename)
    {
        std::wifstream file(ws2s(filename));
        std::wstring fileData((std::istreambuf_iterator<wchar_t>(file)),
                               std::istreambuf_iterator<wchar_t>());
        return fileData;
    }
}

CReportManager::CReportManager(gui::IObjectFactory *pObjectFactory, gui::ISerializerFactory *pSerializerFactory)
    :
      m_pObjectFactory(pObjectFactory),
      m_pSerializerFactory(pSerializerFactory)
{
    assert(m_pObjectFactory);
    assert(m_pSerializerFactory);
}

void CReportManager::setSourceReport(std::wstring pathToReport)
{
    if (m_pathToReport == pathToReport)
        return;

    m_pathToReport = std::move(pathToReport);
    m_pathToReportChanged = true;
    if (m_observer)
        m_observer();
}

std::wstring CReportManager::getSourceReport() const
{
    return m_pathToReport;
}

void CReportManager::setFilter(gui::unique_ptr<gui::IReportFilter> filter)
{
    m_pFilter = std::move(filter);
    if (m_observer)
        m_observer();
}

void CReportManager::setSorter(gui::unique_ptr<gui::IReportSorter> sorter)
{
    m_pSorter = std::move(sorter);
    if (m_observer)
        m_observer();
}

std::wstring CReportManager::getProcessedReport(gui::ProgressNotifier progressNotifier)
{
    progressNotifier(1);
    initReport();
    progressNotifier(25);
    applyFilter();
    progressNotifier(50);
    applySorter();
    progressNotifier(75);
    postProcessing();
    progressNotifier(90);
    auto pReportSerializer = m_pSerializerFactory->get(dynamic_cast<gui::IDataObject*>(m_pReport.get()));
    auto result = pReportSerializer->toString();
    progressNotifier(100);
    return result;
}

void CReportManager::setUpdateNotifier(gui::ReportManagerObserver observer)
{
    m_observer = std::move(observer);
}

size_t CReportManager::getUniqueStacksCount() const
{
    return m_uniqueStackTraceCount;
}

size_t CReportManager::getAllocationsCount() const
{
    return m_allocationsCount;
}

size_t CReportManager::getAllocatedBytes() const
{
    return m_allocatedBytes;
}

void CReportManager::initReport()
{
    if (m_pathToReport.empty())
        return;

    if (m_pathToReportChanged)
    {
        auto pReport = m_pObjectFactory->get(cReportType);
        auto pReportSerializer = m_pSerializerFactory->get(pReport.get());
        gui::readSerializedString(m_pSerializerFactory, pReport.get(), getFileContent(m_pathToReport));
        m_pSourceReport = gui::dynamic_unique_cast<gui::IReport>(std::move(pReport));
        m_pathToReportChanged = false;
    }

    m_pReport = gui::dynamic_unique_cast<gui::IReport>(dynamic_cast<gui::IClonable*>(m_pSourceReport.get())->clone());
}

void CReportManager::applyFilter()
{
    if (!m_pFilter)
        return;

    m_pReport = m_pFilter->get(std::move(m_pReport));
}

void CReportManager::applySorter()
{
    if (!m_pSorter)
        return;

    m_pReport = m_pSorter->sort(std::move(m_pReport));
}

void CReportManager::postProcessing()
{
    auto modules = m_pReport->getLoadedModules();
    modules.clear();
    m_pReport->setLoadedModules(std::move(modules));

    auto logEntries = m_pReport->getLogEntries();
    m_pReport->setTotalIncrease(nullptr);

    m_allocationsCount = 0;
    m_allocatedBytes = 0;
    m_uniqueStackTraceCount = logEntries.size();

    for (size_t i = 0; i < logEntries.size(); ++i)
    {
        const auto &pLogEntry = logEntries[i];
        m_allocatedBytes += pLogEntry->getNewBytes() - pLogEntry->getOldBytes();
        m_allocationsCount += pLogEntry->getNewCount() - pLogEntry->getOldCount();
    }
}
