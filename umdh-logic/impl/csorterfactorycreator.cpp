#include "csorterfactorycreator.h"
#include "csorterfactory.h"

using namespace logic;

gui::unique_ptr<gui::ISorterFactory> CSorterFactoryCreator::create() const
{
    return gui::make_unique<CSorterFactory>();
}
