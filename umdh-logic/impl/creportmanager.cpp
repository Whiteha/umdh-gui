#include "creportmanager.h"
#include "idatamodel/iclonable.h"
#include "metamodel.h"

#pragma warning(push, 0)
#include <cassert>
#include <fstream>
#include <codecvt>
#include <locale>
#pragma warning(pop)

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

CReportManager::CReportManager(const gui::IObjectFactory *pObjectFactory,
	const gui::ISerializerFactory *pSerializerFactory,
	gui::unique_ptr<IHotPathCalculator> pHotPathCalculator)
	:
	m_pObjectFactory(pObjectFactory),
	m_pSerializerFactory(pSerializerFactory),
	m_pHotPathCalculator(std::move(pHotPathCalculator))
{
	assert(m_pObjectFactory);
	assert(m_pSerializerFactory);
	assert(m_pHotPathCalculator);
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
	m_reportFilterChanged = true;
	m_pFilter = std::move(filter);
	if (m_observer)
		m_observer();
}

void CReportManager::setSorter(gui::unique_ptr<gui::IReportSorter> sorter)
{
	m_reportSorterChanged = true;
	m_pSorter = std::move(sorter);
	if (m_observer)
		m_observer();
}

std::wstring CReportManager::getProcessedReport(gui::ProgressNotifier progressNotifier)
{
	processReport(progressNotifier);
	auto pReportSerializer = m_pSerializerFactory->get(dynamic_cast<gui::IDataObject *>(m_pReport.get()));
	auto result = pReportSerializer->toString();
	progressNotifier(100);
	return result;
}

gui::StackFrameDisplayDataList CReportManager::getProcessedReportList(gui::ProgressNotifier progressNotifier)
{
	processReport(progressNotifier);

	auto logEntries = m_pReport->getLogEntries();
	progressNotifier(95);
	gui::StackFrameDisplayDataList result;
	for (const auto &pLogEntry : logEntries)
	{
		auto pLogEntrySerializer = m_pSerializerFactory->get(dynamic_cast<gui::IDataObject *>(pLogEntry.get()));
		const std::ptrdiff_t allocatedBytes = pLogEntry->getNewBytes() - pLogEntry->getOldBytes();
		const std::ptrdiff_t allocationsCount = pLogEntry->getNewCount() - pLogEntry->getOldCount();
		std::wstring title = L"Allocated bytes: " + std::to_wstring(allocatedBytes) + L"; Allocation count: " + std::to_wstring(allocationsCount);
		result.push_back({ title, pLogEntrySerializer->toString() });
	}
	progressNotifier(100);
	return result;
}

namespace
{
	std::wstring getCallTreeItemReprSimple(const std::vector<gui::unique_ptr<gui::IStackTraceFrame>> &pStackTrace, const ptrdiff_t &i)
	{
		if (i < 0)
			return L"";
		return pStackTrace[i]->getModule() + L'.' + pStackTrace[i]->getFunction() + L'+' + std::to_wstring(pStackTrace[i]->getOffset());
	}

	std::wstring getCallTreeItemReprId(const std::vector<gui::unique_ptr<gui::IStackTraceFrame>> &pStackTrace, const ptrdiff_t &i)
	{
		if (i < 0)
			return L"";
		return getCallTreeItemReprSimple(pStackTrace, i);
	}
}

std::wstring CReportManager::getCallTreeItemReprFull(const std::vector<gui::unique_ptr<gui::IStackTraceFrame>> &pStackTrace, const ptrdiff_t &i) const
{
	if (i < 0)
		return L"";
	auto pReportSerializer = m_pSerializerFactory->get(dynamic_cast<gui::IDataObject *>(pStackTrace[i].get()));
	return pReportSerializer->toString();
}

gui::CallTree CReportManager::getProcessedReportCallTree(gui::ProgressNotifier progressNotifier)
{
	processReport(progressNotifier);
	auto logEntries = m_pReport->getLogEntries();
	progressNotifier(92);

	gui::CallTree result;
	gui::CallTreePathId currentLogEntryId = 0;
	for (const auto &pLogEntry : logEntries)
	{
		addLogEntryToCallTree(pLogEntry, currentLogEntryId, result);
		++currentLogEntryId;
	}
	progressNotifier(100);
	return result;
}

void logic::CReportManager::addLogEntryToCallTree(const gui::unique_ptr<gui::ILogEntry> &pLogEntry,
	                                              const gui::CallTreePathId &currentLogEntryId,
	                                              gui::CallTree &callTree) const
{
	const auto pStackTrace = pLogEntry->getStackTrace();
	const std::ptrdiff_t bytesDiff = pLogEntry->getNewBytes() - pLogEntry->getOldBytes();
	const std::ptrdiff_t totalCountDiff = pLogEntry->getNewCount() - pLogEntry->getOldCount();

	std::wstring functionRepresentation;
	std::wstring functionRepresentationNext;

	const auto startIndex = static_cast<std::ptrdiff_t>(pStackTrace.size()) - 1;
	if (startIndex >= 0)
		functionRepresentationNext = getCallTreeItemReprId(pStackTrace, startIndex);

	for (auto i = startIndex; i >= 0; --i)
	{
		functionRepresentation = functionRepresentationNext;
		functionRepresentationNext = getCallTreeItemReprId(pStackTrace, i - 1);

		updatedRecord(callTree,
			pStackTrace,
			i,
			currentLogEntryId,
			functionRepresentation,
			functionRepresentationNext,
			bytesDiff,
			totalCountDiff);
	}
}

void CReportManager::updatedRecord(gui::CallTree &callTreeToUpdate,
	                               const std::vector<gui::unique_ptr<gui::IStackTraceFrame>> &pStackTrace,
	                               const ptrdiff_t &i,
	                               const gui::CallTreePathId &currentLogEntryId,
	                               const std::wstring &functionRepresentation,
	                               const std::wstring &functionRepresentationNext,
	                               const ptrdiff_t &bytesDiff,
	                               const ptrdiff_t &totalCountDiff) const
{
	const bool isStart = (static_cast<std::ptrdiff_t>(i) == static_cast<std::ptrdiff_t>(pStackTrace.size()) - 1);
	const bool isEnd = (i == 0);
	const auto it = callTreeToUpdate.find(gui::CallTreeItem{ functionRepresentation });

	gui::CallTreeItem currrentItem;
	gui::CallTreeNextItemPath currentItemState;
	if (it == callTreeToUpdate.cend())
	{
		currrentItem = gui::CallTreeItem{ functionRepresentation,
										  getCallTreeItemReprSimple(pStackTrace, i),
										  getCallTreeItemReprFull(pStackTrace, i),
										  bytesDiff,
										  totalCountDiff };
		currrentItem.allocatedBytes = bytesDiff;
		currrentItem.allocationCount = totalCountDiff;
	}
	else
	{
		currrentItem = it->first;
		currentItemState = it->second;
		currrentItem.allocatedBytes += bytesDiff;
		currrentItem.allocationCount += totalCountDiff;
		callTreeToUpdate.erase(it);
	}

	if (isStart)
	{
		currrentItem.startForPathId.insert(currentLogEntryId);
	}
	else if (isEnd)
	{
		currrentItem.endForPathId.insert(currentLogEntryId);
	}
	else
	{
		currrentItem.partOfPathId.insert(currentLogEntryId);
	}

	currentItemState[currentLogEntryId] = functionRepresentationNext;
	callTreeToUpdate[currrentItem] = std::move(currentItemState);
}

void CReportManager::processReport(gui::ProgressNotifier progressNotifier)
{
	progressNotifier(1);
	const bool needPostProcessing = m_pathToReportChanged || m_reportFilterChanged || m_reportSorterChanged;
	reloadReportIfNeeded();
	resetReportChangeTriggers();
	progressNotifier(25);
	processReportBase(progressNotifier, needPostProcessing);
}

void CReportManager::resetReportChangeTriggers()
{
	m_pathToReportChanged = false;
	m_reportFilterChanged = false;
	m_reportSorterChanged = false;
}

void CReportManager::processReportBase(gui::ProgressNotifier progressNotifier, bool needPostProcessing)
{
	applyFilter();
	progressNotifier(50);
	applySorter();
	progressNotifier(75);

	if (needPostProcessing)
		postProcessing();

	progressNotifier(90);
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

gui::CallsByCountMap CReportManager::calculateCallsCount(gui::ProgressNotifier progressNotifier)
{
	if (!m_pReport)
		return {};

	processReport(progressNotifier);
	auto result = m_pHotPathCalculator->calculateCallsCount(m_pReport.get());
	progressNotifier(100);
	return result;
}

void CReportManager::reloadReportIfNeeded()
{
	if (m_pathToReport.empty())
		return;

	if (m_pathToReportChanged)
	{
		m_pSourceReport = readReportFromFile();
	}

	if (m_pathToReportChanged || m_reportFilterChanged || m_reportSorterChanged)
	{
		m_pReport = gui::dynamic_unique_cast<gui::IReport>(dynamic_cast<gui::IClonable *>(m_pSourceReport.get())->clone());
	}
}

gui::unique_ptr<gui::IReport> CReportManager::readReportFromFile() const
{
	auto pReport = m_pObjectFactory->get(cReportType);
	auto pReportSerializer = m_pSerializerFactory->get(pReport.get());
	gui::readSerializedString(m_pSerializerFactory, pReport.get(), getFileContent(m_pathToReport));
	return gui::dynamic_unique_cast<gui::IReport>(std::move(pReport));
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
		const std::ptrdiff_t bytesDiff = static_cast<std::ptrdiff_t>(pLogEntry->getNewBytes()) - static_cast<std::ptrdiff_t>(pLogEntry->getOldBytes());
		if (bytesDiff > 0)
			m_allocatedBytes += bytesDiff;

		const std::ptrdiff_t allocationsCountDiff = static_cast<std::ptrdiff_t>(pLogEntry->getNewCount()) - static_cast<std::ptrdiff_t>(pLogEntry->getOldCount());
		if (allocationsCountDiff > 0)
			m_allocationsCount += pLogEntry->getNewCount() - pLogEntry->getOldCount();
	}
}
