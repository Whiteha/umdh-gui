#include "src/impl/mainwindow.h"

#include <QApplication>

#include "serializerfactorycreatorprovider.h"
#include "objectfactorycreator.h"

#include "umdhcreatorprovider.h"
#include "reportmanagercreatorprovider.h"
#include "filterfactorycreatorprovider.h"
#include "sorterfactorycreatorprovider.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("UMDH-GUI");
    QCoreApplication::setApplicationVersion("1.0.2");

    auto pObjectFactory = datamodel::getObjectFactory();
    auto pSerializerFactoryCreator = datamodelserializer::getSerializerFactoryCreator();

    auto pSerializerFactory = pSerializerFactoryCreator->create(pObjectFactory.get());

    auto reportManagerCreator = logic::getReportManagerCreator();
    auto filterFactoryCreator = logic::getFilterFactoryCreator();
    auto sorterFactoryCreator = logic::getSorterFactoryCreator();
    auto umdhCreator = logic::getUmdhCreator();

    MainWindow w({pObjectFactory.get(),
                 pSerializerFactory.get(),
                 reportManagerCreator.get(),
                 filterFactoryCreator.get(),
                 sorterFactoryCreator.get(),
                 umdhCreator.get()});
    w.show();

    return a.exec();
}
