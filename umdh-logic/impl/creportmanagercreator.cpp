#include "creportmanagercreator.h"
#include "creportmanager.h"

using namespace logic;

gui::unique_ptr<gui::IReportManager> CReportManagerCreator::create(gui::IObjectFactory *pObjectFactory,
                                                                   gui::ISerializerFactory *pSerializerFactory) const
{
    return gui::make_unique<CReportManager>(pObjectFactory, pSerializerFactory);
}
