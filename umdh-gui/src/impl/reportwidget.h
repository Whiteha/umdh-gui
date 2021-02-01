#ifndef REPORTWIDGET_H
#define REPORTWIDGET_H

#include "idatamodel/iobjectfactory.h"
#include "idatamodelserializer/iserializerfactory.h"

#include "ilogic/ireportmanagercreator.h"
#include "ilogic/ireportmanager.h"
#include "ilogic/ifilterfactory.h"
#include "ilogic/isorterfactory.h"

#pragma warning(push, 0)
#include <QWidget>
#pragma warning(pop)

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
    gui::IObjectFactory *m_pObjectFactory = nullptr;
    gui::ISerializerFactory *m_pSerializerFactory = nullptr;
    gui::IReportManagerCreator* m_pReportManagerCreator = nullptr;
    gui::IFilterFactory *m_pFilterFactory = nullptr;
    gui::ISorterFactory *m_pSorterFactory = nullptr;
    Ui::ReportWidget *m_pUi = nullptr;

    std::vector<gui::unique_ptr<gui::IReportManager>> m_pReportManagers;
};

#endif // REPORTWIDGET_H
