#include "interface/reportmanagercreatorprovider.h"
#include "impl/creportmanagercreator.h"

gui::unique_ptr<gui::IReportManagerCreator> logic::getReportManagerCreator()
{
    return gui::make_unique<CReportManagerCreator>();
}
