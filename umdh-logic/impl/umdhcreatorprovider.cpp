#include "interface/umdhcreatorprovider.h"
#include "impl/cumdhcreator.h"

gui::unique_ptr<gui::IUmdhCreator> logic::getUmdhCreator()
{
    return gui::make_unique<CUmdhCreator>();
}
