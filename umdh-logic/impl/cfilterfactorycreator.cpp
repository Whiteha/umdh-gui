#include "cfilterfactorycreator.h"
#include "cfilterfactory.h"

using namespace logic;

gui::unique_ptr<gui::IFilterFactory> CFilterFactoryCreator::create() const
{
    return gui::make_unique<CFilterFactory>();
}
