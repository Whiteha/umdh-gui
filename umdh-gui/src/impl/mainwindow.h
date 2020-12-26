#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>

#include "idatamodel/iobjectfactory.h"
#include "idatamodelserializer/iserializerfactory.h"

#include "ilogic/ireportmanagercreator.h"
#include "ilogic/ifilterfactorycreator.h"
#include "ilogic/isorterfactorycreator.h"
#include "ilogic/iumdhcreator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct MainWindowArgs
{
    gui::IObjectFactory *pObjectFactory;
    gui::ISerializerFactory *pSerializerFactory;
    gui::IReportManagerCreator *pReportManagerCreator;
    gui::IFilterFactoryCreator *pFilterFactoryCreator;
    gui::ISorterFactoryCreator *pSorterFactoryCreator;
    gui::IUmdhCreator *pUmdhCreator;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const MainWindowArgs&,
               QWidget *parent = nullptr);
    ~MainWindow();



private:
    void init(const MainWindowArgs&);
    void createTabUmdh(const MainWindowArgs& mainWindowArgs);
    void createTabReport(const MainWindowArgs& mainWindowArgs);

private:
    Ui::MainWindow *m_pUi;

    gui::unique_ptr<gui::ISettings> m_pSettings;
    gui::unique_ptr<gui::IFilterFactory> m_pFilterFactory;
    gui::unique_ptr<gui::ISorterFactory> m_pSorterFactory;
    gui::unique_ptr<gui::IUmdh> m_pUmdh;
};
#endif // MAINWINDOW_H
