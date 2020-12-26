#include "reportviewwidget.h"
#include "ui_reportviewwidget.h"

#include "metamodel.h"

#include "ilogic/ireportfilterwithpattern.h"
#include "ilogic/ireportfilterwithoutpattern.h"
#include "ilogic/ileakedmorethanbytesfilter.h"
#include "ilogic/ileakedmorethancountfilter.h"

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
    m_pProgressDialog->setCancelButton(0);

    initDefaultState();
}

ReportViewWidget::~ReportViewWidget()
{
    delete m_pUi;
}

void ReportViewWidget::initDefaultState()
{
    m_pUi->reportViewPlainTextEdit->setWordWrapMode(QTextOption::NoWrap);

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
    m_pUi->reportViewPlainTextEdit->setPlainText(QString::fromStdWString(m_pReportManager->getProcessedReport(progressStateCallback)));
    m_pUi->uniqueStacksCountLabel->setText(tr("Unique stacks count") + ": " + QString::fromStdString(std::to_string(m_pReportManager->getUniqueStacksCount())));
    m_pUi->allocationsEventsLabel->setText(tr("Allocation events") + ": " + QString::fromStdString(std::to_string(m_pReportManager->getAllocationsCount())));
    m_pUi->bytesAllocatedLabel->setText(tr("Bytes allocated") + ": " + QString::fromStdString(std::to_string(m_pReportManager->getAllocatedBytes())));

    if (isInitialization)
        m_pProgressDialog->hide();
}

void ReportViewWidget::updateFilter()
{
    m_pReportManager->setFilter(getActualFilter());
}

gui::unique_ptr<IReportFilter> ReportViewWidget::getActualFilter() const
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
