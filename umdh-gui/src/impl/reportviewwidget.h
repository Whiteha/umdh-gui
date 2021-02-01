#ifndef REPORTVIEWWIDGET_H
#define REPORTVIEWWIDGET_H

#include "ilogic/ireportmanager.h"
#include "ilogic/ifilterfactory.h"
#include "ilogic/isorterfactory.h"
#include "ilogic/ireportfilter.h"

#pragma warning(push, 0)
#include <QWidget>
#include <QProgressDialog>
#include <QTreeWidgetItem>
#pragma warning(pop)

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

    void on_reportViewPlainTextRadioButton_toggled(bool checked);

    void on_reportViewListRadioButton_toggled(bool checked);

    void on_reportViewCallsCountRadioButton_toggled(bool checked);

    void on_reportViewCallStackTreeRadioButton_toggled(bool checked);

private:
    void initDefaultState();
    void initFilters();
    void initSortingCombobox();
    void updateReportView(bool isInitialization = false);
    void updateFilter();
    gui::unique_ptr<gui::IReportFilter> getFilter() const;
    void updateActualReportRepresentation(std::function<void(int)> progressStateCallback);
    void setCallTreeRepresentation(gui::ProgressNotifier &progressStateCallback);
    void buildCallTreeRepresentation(const gui::CallTree &callTree,
                                    const gui::CallTreeItem &current, 
                                    QTreeWidgetItem *pParent, 
                                    const int deep, 
                                    const gui::CallTreePathId pathId);

    void setPlainTextRepresentation(QString str);
    void setListRepresentation(const gui::StackFrameDisplayDataList&);

    QString getCallsCountData(const gui::CallsByCountMap&) const;
    void updateCommonInfo();

private:
    gui::IReportManager *m_pReportManager = nullptr;
    gui::IFilterFactory *m_pFilterFactory = nullptr;
    gui::ISorterFactory *m_pSorterFactory = nullptr;
    Ui::ReportViewWidget *m_pUi = nullptr;
    QProgressDialog *m_pProgressDialog = nullptr;
};

#endif // REPORTVIEWWIDGET_H
