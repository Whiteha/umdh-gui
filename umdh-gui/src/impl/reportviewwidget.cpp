#include "reportviewwidget.h"

#include "metamodel.h"

#include "ilogic/ireportfilterwithpattern.h"
#include "ilogic/ireportfilterwithoutpattern.h"
#include "ilogic/ileakedmorethanbytesfilter.h"
#include "ilogic/ileakedmorethancountfilter.h"

#pragma warning(push, 0)
#include "ui_reportviewwidget.h"
#pragma warning(pop)

namespace
{
    const int cMaxCallTreeDeep = 64;
}

using namespace gui;

ReportViewWidget::ReportViewWidget(gui::IReportManager *pReportManager,
                                   gui::IFilterFactory *pFilterFactory,
                                   gui::ISorterFactory *pSorterFactory,
                                   QWidget *parent) :
    QWidget(parent),
    m_pReportManager(pReportManager),
    m_pFilterFactory(pFilterFactory),
    m_pSorterFactory(pSorterFactory),
    m_pUi(new Ui::ReportViewWidget)
{
    assert(m_pReportManager);
    assert(m_pFilterFactory);
    assert(m_pSorterFactory);

    m_pUi->setupUi(this);
    m_pProgressDialog = new QProgressDialog(this);
    m_pProgressDialog->setWindowFlags(m_pProgressDialog->windowFlags()
                                      & ~Qt::WindowContextHelpButtonHint
                                      & ~Qt::WindowCloseButtonHint);
    m_pProgressDialog->setWindowTitle(tr("Processing..."));
    m_pProgressDialog->setCancelButton(0);

    initDefaultState();
}

ReportViewWidget::~ReportViewWidget()
{
    delete m_pUi;
}

void ReportViewWidget::initDefaultState()
{
    m_pUi->plainTextEdit->setWordWrapMode(QTextOption::NoWrap);
    m_pUi->treeWidget->setVerticalScrollMode(QHeaderView::ScrollPerPixel);
    m_pUi->treeWidget->setColumnWidth(0, 300);

    m_pUi->reportViewPlainTextRadioButton->setChecked(true);

    initSortingCombobox();
    initFilters();
    updateFilter();

    m_pReportManager->setUpdateNotifier([&] { updateReportView(); });

    updateReportView(true);
}

void ReportViewWidget::initFilters()
{
    m_pUi->filterLeakedBytesCheckBox->setChecked(true);
    m_pUi->filterLeakedTimesCheckBox->setChecked(true);
    m_pUi->filterHideSystemCheckBox->setChecked(true);
    m_pUi->filterWithSymbolsCheckBox->setChecked(true);
    m_pUi->filterWithSourcesCheckBox->setChecked(true);
    m_pUi->filterWithPatternLineEdit->setEnabled(false);
    m_pUi->filterWithoutPatternLineEdit->setEnabled(false);
}

void ReportViewWidget::initSortingCombobox()
{
    m_pUi->sortingTypeComboBox->addItem(tr("By allocated bytes"), QVariant(QString::fromStdWString(cReportSorterByAllocatedBytesType)));
    m_pUi->sortingTypeComboBox->addItem(tr("By allocations count"), QVariant(QString::fromStdWString(cReportSorterByAllocationCountType)));
}

void ReportViewWidget::on_sortingTypeComboBox_currentIndexChanged(int pos)
{
    m_pUi->sortingTypeComboBox->itemData(pos);
    m_pReportManager->setSorter(m_pSorterFactory->get(m_pUi->sortingTypeComboBox->itemData(pos).toString().toStdWString()));
}

void ReportViewWidget::updateReportView(bool isInitialization)
{
    if (isInitialization)
        m_pProgressDialog->show();

    auto progressStateCallback = [&] (int progress)
    {
        if (isInitialization)
        {
            m_pProgressDialog->setValue(progress);
            QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        }
    };

    updateActualReportRepresentation(progressStateCallback);

    if (isInitialization)
        m_pProgressDialog->hide();
}

void ReportViewWidget::updateCommonInfo()
{
    m_pUi->uniqueStacksCountLabel->setText(tr("Unique stacks count") + ": " + QString::fromStdString(std::to_string(m_pReportManager->getUniqueStacksCount())));
    m_pUi->allocationsEventsLabel->setText(tr("Allocation events") + ": " + QString::fromStdString(std::to_string(m_pReportManager->getAllocationsCount())));
    m_pUi->bytesAllocatedLabel->setText(tr("Bytes allocated") + ": " + QString::fromStdString(std::to_string(m_pReportManager->getAllocatedBytes())));
}

void ReportViewWidget::updateActualReportRepresentation(std::function<void(int)> progressStateCallback)
{
    if (m_pUi->reportViewPlainTextRadioButton->isChecked())
    {
        setPlainTextRepresentation(QString::fromStdWString(m_pReportManager->getProcessedReport(progressStateCallback)));
    }
    else if (m_pUi->reportViewListRadioButton->isChecked())
    {
        auto list = m_pReportManager->getProcessedReportList(progressStateCallback);
        setListRepresentation(std::move(list));
    }
    else if (m_pUi->reportViewCallsCountRadioButton->isChecked())
    {
        const auto callsCount = m_pReportManager->calculateCallsCount(progressStateCallback);
        auto text = getCallsCountData(callsCount);
        setPlainTextRepresentation(std::move(text));
    }
    else if (m_pUi->reportViewCallStackTreeRadioButton->isChecked())
    {
        setCallTreeRepresentation(progressStateCallback);
    }

    updateCommonInfo();
}

void ReportViewWidget::setCallTreeRepresentation(gui::ProgressNotifier &progressStateCallback)
{
    m_pUi->treeWidget->clear();
    m_pUi->treeWidget->show();
    m_pUi->plainTextEdit->hide();
    m_pUi->treeWidget->setHeaderHidden(false);
    m_pUi->treeWidget->setSortingEnabled(true);
    m_pUi->treeWidget->setColumnCount(3);

    const auto &callTree = m_pReportManager->getProcessedReportCallTree(progressStateCallback);
    for (const auto &item : callTree)
    {
        if (!item.first.startForPathId.empty())
        {
            buildCallTreeRepresentation(callTree, item.first, nullptr, 0, gui::CallTreeRootPathId);
        }
    }
}

namespace
{
    class TreeWidgetItemWithNumberSorting : public QTreeWidgetItem
    {
    public:
        using QTreeWidgetItem::QTreeWidgetItem;

    private:
          bool operator<(const QTreeWidgetItem &other) const
          {
              const auto currentSortingColumn = treeWidget()->sortColumn();
              if (currentSortingColumn == 0)
                  return QTreeWidgetItem::operator<(other);

              return text(currentSortingColumn).toLongLong() < other.text(currentSortingColumn).toLongLong();
          }
    };
}

void ReportViewWidget::buildCallTreeRepresentation(const CallTree &callTree,
                                                   const gui::CallTreeItem &current, 
                                                   QTreeWidgetItem *pParent, 
                                                   const int deep, 
                                                   const gui::CallTreePathId pathId)
{
    if (deep == cMaxCallTreeDeep)
        return;

    auto it = callTree.find(current);
    if (it == callTree.cend())
        return;

    if (pathId != gui::CallTreeRootPathId)
    {
        const bool notPresentInPath = (it->first.startForPathId.find(pathId) == it->first.startForPathId.cend())
                                   && (it->first.partOfPathId.find(pathId) == it->first.partOfPathId.cend())
                                   && (it->first.endForPathId.find(pathId) == it->first.endForPathId.cend());
        if (notPresentInPath)
            return;
    }

    auto* pQTreeWidgetItem = new TreeWidgetItemWithNumberSorting;
    bool childFound = false;
    if (pParent)
    {
        const auto childrenCount = pParent->childCount();
        for (int i = 0; i < childrenCount; ++i)
        {
            if (pParent->child(i)->data(0, Qt::UserRole) == QString::fromStdWString(it->first.functionRepresentation))
            {
                pQTreeWidgetItem = (TreeWidgetItemWithNumberSorting*)pParent->child(i);
                childFound = true;
                break;
            }
        }
        if (!childFound)
        {
            pParent->addChild(pQTreeWidgetItem);
        }
    }        
    else
    {
        m_pUi->treeWidget->addTopLevelItem(pQTreeWidgetItem);
    }

    if (!childFound)
    {
        pQTreeWidgetItem->setData(0, Qt::UserRole, QString::fromStdWString(it->first.functionRepresentation));
        pQTreeWidgetItem->setData(0, Qt::DisplayRole, QString::fromStdWString(it->first.shortFunctionRepresentation));
        pQTreeWidgetItem->setData(1, Qt::DisplayRole, QString::fromStdWString(std::to_wstring(pQTreeWidgetItem->data(1, Qt::DisplayRole).toInt() + it->first.allocatedBytes)));
        pQTreeWidgetItem->setData(2, Qt::DisplayRole, QString::fromStdWString(std::to_wstring(pQTreeWidgetItem->data(2, Qt::DisplayRole).toInt() + it->first.allocationCount)));
        pQTreeWidgetItem->setToolTip(0, QString::fromStdWString(it->first.fullFunctionRepresentation));
    }

    const bool isEnd = (it->first.endForPathId.find(pathId) != it->first.endForPathId.cend());
    if (isEnd)
        return;

    if (pathId == gui::CallTreeRootPathId)
    {
        for (const auto& itemNext : (*it).second)
        {
            buildCallTreeRepresentation(callTree, gui::CallTreeItem{ itemNext.second }, pQTreeWidgetItem, deep + 1, itemNext.first);
        }
    }
    else
    {
        auto nextIt = (*it).second.find(pathId);
        if (nextIt != (*it).second.end())
        {
            buildCallTreeRepresentation(callTree, gui::CallTreeItem{ nextIt->second }, pQTreeWidgetItem, deep + 1, nextIt->first);
        }
    }
}

void ReportViewWidget::setPlainTextRepresentation(QString str)
{
    m_pUi->plainTextEdit->clear();
    m_pUi->plainTextEdit->show();
    m_pUi->treeWidget->hide();

    m_pUi->plainTextEdit->setPlainText(str);
}

void ReportViewWidget::setListRepresentation(const gui::StackFrameDisplayDataList &list)
{
    m_pUi->treeWidget->clear();
    m_pUi->treeWidget->show();
    m_pUi->plainTextEdit->hide();
    m_pUi->treeWidget->setHeaderHidden(false);
    m_pUi->treeWidget->setSortingEnabled(false);
    m_pUi->treeWidget->setColumnCount(1);

    for (const auto &item : list)
    {
        auto pQTreeWidgetItem = new QTreeWidgetItem;
        pQTreeWidgetItem->setData(0, Qt::DisplayRole, QString::fromStdWString(item.title));
        auto pQTreeWidgetItemChild = new QTreeWidgetItem(pQTreeWidgetItem);
        pQTreeWidgetItemChild->setData(0, Qt::DisplayRole, QString::fromStdWString(item.body));
        m_pUi->treeWidget->addTopLevelItem(pQTreeWidgetItem);
    }
}

QString ReportViewWidget::getCallsCountData(const gui::CallsByCountMap &callsCount) const
{
    QString result;
    std::ptrdiff_t prevCount = 0;
    bool isFirst = true;
    for (auto it = callsCount.crbegin(); it != callsCount.crend(); ++it)
    {
        if (prevCount != (*it).first)
        {
            prevCount = (*it).first;
            if (isFirst)
            {
                isFirst = false;
            }
            else
            {
                result += L"\n";
            }
        }

        result += L"Calls count: " + std::to_wstring((*it).first) + L";\t" + (*it).second + L"\n";
    }

    return result;
}

void ReportViewWidget::updateFilter()
{
    m_pReportManager->setFilter(getFilter());
}

gui::unique_ptr<IReportFilter> ReportViewWidget::getFilter() const
{
    auto pNewfilter = m_pFilterFactory->get(cDefaultFilterType);
    auto pSubFilters = pNewfilter->getLogEntryFilters();

    if (m_pUi->filterWithSymbolsCheckBox->isChecked())
    {
        auto pFilter = m_pFilterFactory->get(cReportFilterWithSymbolsType);
        auto logEntryFilters = pFilter->getLogEntryFilters();
        for (auto &pLogEntryFilter : logEntryFilters)
        {
            pSubFilters.push_back(std::move(pLogEntryFilter));
        }
    }

    if (m_pUi->filterWithSourcesCheckBox->isChecked())
    {
        auto pFilter = m_pFilterFactory->get(cReportFilterWithSourcesType);
        auto logEntryFilters = pFilter->getLogEntryFilters();
        for (auto &pLogEntryFilter : logEntryFilters)
        {
            pSubFilters.push_back(std::move(pLogEntryFilter));
        }
    }

    if (m_pUi->filterHideSystemCheckBox->isChecked())
    {
        auto pFilter = m_pFilterFactory->get(cReportFilterWithoutSystemModulesType);
        auto logEntryFilters = pFilter->getLogEntryFilters();
        for (auto &pLogEntryFilter : logEntryFilters)
        {
            pSubFilters.push_back(std::move(pLogEntryFilter));
        }
    }

    if (m_pUi->filterLeakedBytesCheckBox->isChecked())
    {
        const size_t kbytes = m_pUi->filterLeakedBytesSpinBox->value();
        auto pFilter = m_pFilterFactory->get(cReportFilterLeakedBytesType);
        auto pLeakedMoreThanBytesFilter = dynamic_cast<ILeakedMoreThanBytesFilter*>(pFilter.get());
        pLeakedMoreThanBytesFilter->setBytes(kbytes * 1000);
        auto logEntryFilters = pFilter->getLogEntryFilters();
        for (auto &pLogEntryFilter : logEntryFilters)
        {
            pSubFilters.push_back(std::move(pLogEntryFilter));
        }
    }

    if (m_pUi->filterLeakedTimesCheckBox->isChecked())
    {
        const size_t ntimes = m_pUi->filterLeakedTimesSpinBox->value();
        auto pFilter = m_pFilterFactory->get(cReportFilterLeakedNTimesType);
        auto pLeakedMoreThanCountFilter = dynamic_cast<ILeakedMoreThanCountFilter*>(pFilter.get());
        pLeakedMoreThanCountFilter->setCount(ntimes);
        auto logEntryFilters = pFilter->getLogEntryFilters();
        for (auto &pLogEntryFilter : logEntryFilters)
        {
            pSubFilters.push_back(std::move(pLogEntryFilter));
        }
    }

    if (m_pUi->filterWithPatternCheckBox->isChecked() && !m_pUi->filterWithPatternLineEdit->text().isEmpty())
    {
        const auto &patterns = m_pUi->filterWithPatternLineEdit->text().split(",");
        std::list<std::wstring> convertedPatterns;
        for (const auto &pattern : patterns)
        {
            convertedPatterns.push_back(pattern.trimmed().toStdWString());
        }
        auto pFilter = m_pFilterFactory->get(cReportFilterWithPatternType);
        auto pReportFilterWithPattern = dynamic_cast<IReportFilterWithPattern*>(pFilter.get());
        pReportFilterWithPattern->setPattern(convertedPatterns);
        auto logEntryFilters = pFilter->getLogEntryFilters();
        for (auto &pLogEntryFilter : logEntryFilters)
        {
            pSubFilters.push_back(std::move(pLogEntryFilter));
        }
    }

    if (m_pUi->filterWithoutPatternCheckBox->isChecked() && !m_pUi->filterWithoutPatternLineEdit->text().isEmpty())
    {
        const auto &patterns = m_pUi->filterWithoutPatternLineEdit->text().split(",");
        std::list<std::wstring> convertedPatterns;
        for (const auto &pattern : patterns)
        {
            convertedPatterns.push_back(pattern.trimmed().toStdWString());
        }
        auto pFilter = m_pFilterFactory->get(cReportFilterWithoutPatternType);
        auto pReportFilterWithPattern = dynamic_cast<IReportFilterWithoutPattern*>(pFilter.get());
        pReportFilterWithPattern->setBadPattern(convertedPatterns);
        auto logEntryFilters = pFilter->getLogEntryFilters();
        for (auto &pLogEntryFilter : logEntryFilters)
        {
            pSubFilters.push_back(std::move(pLogEntryFilter));
        }
    }
    pNewfilter->setLogEntryFilters(pSubFilters);
    return pNewfilter;
}

void ReportViewWidget::on_filterLeakedBytesCheckBox_stateChanged(int isActive)
{
    m_pUi->filterLeakedBytesSpinBox->setEnabled(isActive);
    updateFilter();
}

void ReportViewWidget::on_filterLeakedTimesCheckBox_stateChanged(int isActive)
{
    m_pUi->filterLeakedTimesSpinBox->setEnabled(isActive);
    updateFilter();
}

void ReportViewWidget::on_filterWithSymbolsCheckBox_stateChanged(int)
{
    updateFilter();
}

void ReportViewWidget::on_filterWithSourcesCheckBox_stateChanged(int)
{
    updateFilter();
}

void ReportViewWidget::on_filterWithPatternCheckBox_stateChanged(int isActive)
{
    m_pUi->filterWithPatternLineEdit->setEnabled(isActive);
    if (m_pUi->filterWithPatternLineEdit->text().isEmpty())
        return;
    updateFilter();
}

void ReportViewWidget::on_filterWithPatternLineEdit_editingFinished()
{
    updateFilter();
}

void ReportViewWidget::on_filterLeakedBytesSpinBox_editingFinished()
{
    updateFilter();
}

void ReportViewWidget::on_filterLeakedTimesSpinBox_editingFinished()
{
    updateFilter();
}

void ReportViewWidget::on_filterWithoutPatternLineEdit_editingFinished()
{
    updateFilter();
}

void ReportViewWidget::on_filterHideSystemCheckBox_stateChanged(int)
{
    updateFilter();
}

void ReportViewWidget::on_filterWithoutPatternCheckBox_stateChanged(int isActive)
{
    m_pUi->filterWithoutPatternLineEdit->setEnabled(isActive);
    if (m_pUi->filterWithoutPatternLineEdit->text().isEmpty())
        return;
    updateFilter();
}

void ReportViewWidget::on_reportViewPlainTextRadioButton_toggled(bool checked)
{
    if (checked)
        updateReportView();
}

void ReportViewWidget::on_reportViewListRadioButton_toggled(bool checked)
{
    if (checked)
        updateReportView();
}

void ReportViewWidget::on_reportViewCallsCountRadioButton_toggled(bool checked)
{
    if (checked)
        updateReportView();
}

void ReportViewWidget::on_reportViewCallStackTreeRadioButton_toggled(bool checked)
{
    if (checked)
        updateReportView();
}
