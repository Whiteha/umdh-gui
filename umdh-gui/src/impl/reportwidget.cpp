#include "reportwidget.h"

#include "reportviewwidget.h"

#pragma warning(push, 0)
#include <QFileDialog>
#include <QFileInfo>
#include "ui_reportwidget.h"
#pragma warning(pop)

ReportWidget::ReportWidget(gui::IObjectFactory *pObjectFactory,
                           gui::ISerializerFactory *pSerializerFactory,
                           gui::IReportManagerCreator *pReportManagerCreator,
                           gui::IFilterFactory *pFilterFactory,
                           gui::ISorterFactory *pSorterFactory,
                           QWidget *parent) :
    QWidget(parent),
    m_pObjectFactory(pObjectFactory),
    m_pSerializerFactory(pSerializerFactory),
    m_pReportManagerCreator(pReportManagerCreator),
    m_pFilterFactory(pFilterFactory),
    m_pSorterFactory(pSorterFactory),
    m_pUi(new Ui::ReportWidget)
{
    assert(m_pObjectFactory);
    assert(m_pSerializerFactory);
    assert(m_pReportManagerCreator);
    assert(m_pFilterFactory);
    assert(m_pSorterFactory);

    m_pUi->setupUi(this);

    updateSourceReportsView();
}

ReportWidget::~ReportWidget()
{
    delete m_pUi;
}

void ReportWidget::updateSourceReportsView()
{
    m_pUi->reportForAnalyzeListWidget->clear();
    for (const auto &pReportManager : m_pReportManagers)
    {
        m_pUi->reportForAnalyzeListWidget->addItem(QString::fromStdWString(pReportManager->getSourceReport()));
    }
    m_pUi->removeReportPushButton->setEnabled(!m_pReportManagers.empty());
    m_pUi->reportForAnalyzeListWidget->setEnabled(!m_pReportManagers.empty());
    if (m_pUi->reportForAnalyzeListWidget->count() > 0)
        m_pUi->reportForAnalyzeListWidget->setCurrentRow(m_pUi->reportForAnalyzeListWidget->count() - 1);
}

void ReportWidget::on_addReportPushButton_clicked()
{
    const auto &path = QFileDialog::getOpenFileName(this, tr("Choose path to UMDH report"), "", "UMDH report (*.*)");
    if (path.isEmpty() || !QFileInfo::exists(path) || !QFileInfo(path).isFile())
        return;

    m_pReportManagers.push_back(m_pReportManagerCreator->create(m_pObjectFactory, m_pSerializerFactory));
    auto pReportManager = m_pReportManagers.back().get();

    pReportManager->setSourceReport( path.toStdWString() );
    m_pUi->reportView->addTab(new ReportViewWidget(pReportManager, m_pFilterFactory, m_pSorterFactory), QFileInfo(path).fileName());
    m_pUi->reportView->setTabToolTip(m_pUi->reportView->count() - 1, path);
    updateSourceReportsView();
}

void ReportWidget::on_removeReportPushButton_clicked()
{
    const auto currentRow = m_pUi->reportForAnalyzeListWidget->currentRow();
    if (currentRow < 0)
        return;

    removeReportViewTab(currentRow);
}

void ReportWidget::on_reportView_tabCloseRequested(int index)
{
    removeReportViewTab(index);
}

void ReportWidget::removeReportViewTab(int index)
{
    for (int i = 0; static_cast<size_t>(i) < m_pReportManagers.size(); ++i)
    {
        if (i == index)
        {
            m_pUi->reportView->removeTab(i);
            m_pReportManagers[i].reset();
            continue;
        }

        m_pUi->reportForAnalyzeListWidget->addItem(QString::fromStdWString(m_pReportManagers[i]->getSourceReport()));
    }
    m_pReportManagers.erase(m_pReportManagers.begin() + index);
    updateSourceReportsView();
}
