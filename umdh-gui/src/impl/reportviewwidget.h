#ifndef REPORTVIEWWIDGET_H
#define REPORTVIEWWIDGET_H

#include "ilogic/ireportmanager.h"
#include "ilogic/ifilterfactory.h"
#include "ilogic/isorterfactory.h"
#include "ilogic/ireportfilter.h"

#include <QWidget>
#include <QProgressDialog>

namespace Ui {
class ReportViewWidget;
}

class ReportViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReportViewWidget(gui::IReportManager*,
                              gui::IFilterFactory*,
                              gui::ISorterFactory*,
                              QWidget *parent = nullptr);
    ~ReportViewWidget();

private slots:
    void on_filterLeakedBytesCheckBox_stateChanged(int);

    void on_filterLeakedTimesCheckBox_stateChanged(int);

    void on_filterWithSymbolsCheckBox_stateChanged(int);

    void on_filterWithSourcesCheckBox_stateChanged(int);

    void on_filterWithPatternCheckBox_stateChanged(int);

    void on_filterWithPatternLineEdit_editingFinished();

    void on_sortingTypeComboBox_currentIndexChanged(int);

    void on_filterLeakedBytesSpinBox_editingFinished();

    void on_filterLeakedTimesSpinBox_editingFinished();

    void on_filterWithoutPatternLineEdit_editingFinished();

    void on_filterHideSystemCheckBox_stateChanged(int);

    void on_filterWithoutPatternCheckBox_stateChanged(int arg1);

private:
    void initDefaultState();
    void initFilters();
    void initSortingCombobox();
    void updateSourceReportsView();
    void updateReportView(bool isInitialization = false);
    void updateFilter();
    gui::unique_ptr<gui::IReportFilter> getActualFilter() const;

private:
    gui::IReportManager *m_pReportManager;
    gui::IFilterFactory *m_pFilterFactory;
    gui::ISorterFactory *m_pSorterFactory;
    Ui::ReportViewWidget *m_pUi;
    QProgressDialog *m_pProgressDialog;
};

#endif // REPORTVIEWWIDGET_H
