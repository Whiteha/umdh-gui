#ifndef FILTERFACTORYCREATORPROVIDER_H
#define FILTERFACTORYCREATORPROVIDER_H

#include "umdh.logic.global.h"
#include "ilogic/ifilterfactorycreator.h"

namespace logic
{
    UMDH_LOGIC_EXPORT gui::unique_ptr<gui::IFilterFactoryCreator> getFilterFactoryCreator();
}

#endif // FILTERFACTORYCREATORPROVIDER_H
