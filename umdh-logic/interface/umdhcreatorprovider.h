#ifndef UMDHCREATORPROVIDER_H
#define UMDHCREATORPROVIDER_H

#include "umdh.logic.global.h"
#include "ilogic/iumdhcreator.h"

namespace logic
{
    UMDH_LOGIC_EXPORT gui::unique_ptr<gui::IUmdhCreator> getUmdhCreator();
}

#endif // UMDHCREATORPROVIDER_H
