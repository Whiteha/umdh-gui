#ifndef REPORTWIDGET_H
#define REPORTWIDGET_H

#include "idatamodel/iobjectfactory.h"
#include "idatamodelserializer/iserializerfactory.h"

#include "ilogic/ireportmanagercreator.h"
#include "ilogic/ireportmanager.h"
#include "ilogic/ifilterfactory.h"
#include "ilogic/isorterfactory.h"

#include <QWidget>

namespace Ui {
class ReportWidget;
}

class ReportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReportWidget(gui::IObjectFactory *pObjectFactory,
                          gui::ISerializerFactory *pSerializerFactory,
                          gui::IReportManagerCreator*,
                          gui::IFilterFactory*,
                          gui::ISorterFactory*,
                          QWidget *parent = nullptr);
    ~ReportWidget();

private slots:
    void on_addReportPushButton_clicked();

    void on_removeReportPushButton_clicked();

    void on_reportView_tabCloseRequested(int index);

private:
    void updateSourceReportsView();
    void removeReportViewTab(int index);

private:
    gui::IObjectFactory *m_pObjectFactory;
    gui::ISerializerFactory *m_pSerializerFactory;
    gui::IReportManagerCreator* m_pReportManagerCreator;
    gui::IFilterFactory *m_pFilterFactory;
    gui::ISorterFactory *m_pSorterFactory;
    Ui::ReportWidget *m_pUi;

    std::vector<gui::unique_ptr<gui::IReportManager>> m_pReportManagers;
};

#endif // REPORTWIDGET_H
