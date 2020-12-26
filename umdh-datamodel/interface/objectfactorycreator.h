#ifndef REPORTCREATORPROVIDER_H
#define REPORTCREATORPROVIDER_H

#include "umdh.datamodel.global.h"
#include "idatamodel/iobjectfactory.h"
#include "customuniqueptr.h"

namespace datamodel
{
    UMDH_DATAMODEL_EXPORT gui::unique_ptr<gui::IObjectFactory> getObjectFactory();
}

#endif // REPORTCREATORPROVIDER_H
