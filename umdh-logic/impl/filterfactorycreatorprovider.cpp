#include "interface/filterfactorycreatorprovider.h"
#include "impl/cfilterfactorycreator.h"

gui::unique_ptr<gui::IFilterFactoryCreator> logic::getFilterFactoryCreator()
{
    return gui::make_unique<CFilterFactoryCreator>();
}
