#include "interface/objectfactorycreator.h"
#include "cobjectfactory.h"

gui::unique_ptr<gui::IObjectFactory> datamodel::getObjectFactory()
{
    return gui::make_unique<datamodel::CObjectFactory>();
}
