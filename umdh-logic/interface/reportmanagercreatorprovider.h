#ifndef REPORTMANAGERCREATORPROVIDER_H
#define REPORTMANAGERCREATORPROVIDER_H

#include "umdh.logic.global.h"
#include "ilogic/ireportmanagercreator.h"

namespace logic
{
    UMDH_LOGIC_EXPORT gui::unique_ptr<gui::IReportManagerCreator> getReportManagerCreator();
}

#endif // REPORTMANAGERCREATORPROVIDER_H
