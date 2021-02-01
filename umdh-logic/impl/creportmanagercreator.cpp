#include "creportmanagercreator.h"
#include "creportmanager.h"
#include "chotpathcalculator.h"

gui::unique_ptr<gui::IReportManager>
logic::CReportManagerCreator::create(const gui::IObjectFactory *pObjectFactory,
                                     const gui::ISerializerFactory *pSerializerFactory) const
{
    return gui::make_unique<CReportManager>(pObjectFactory, pSerializerFactory, gui::make_unique<logic::CHotPathCalculator>(pSerializerFactory));
}
