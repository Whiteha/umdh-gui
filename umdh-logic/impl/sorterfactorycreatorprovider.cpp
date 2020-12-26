#include "interface/sorterfactorycreatorprovider.h"
#include "impl/csorterfactorycreator.h"

gui::unique_ptr<gui::ISorterFactoryCreator> logic::getSorterFactoryCreator()
{
    return gui::make_unique<CSorterFactoryCreator>();
}
