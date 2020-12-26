#ifndef SORTERFACTORYCREATORPROVIDER_H
#define SORTERFACTORYCREATORPROVIDER_H

#include "umdh.logic.global.h"
#include "ilogic/isorterfactorycreator.h"

namespace logic
{
    UMDH_LOGIC_EXPORT gui::unique_ptr<gui::ISorterFactoryCreator> getSorterFactoryCreator();
}

#endif // SORTERFACTORYCREATORPROVIDER_H
