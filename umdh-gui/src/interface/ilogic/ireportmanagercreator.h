#ifndef IREPORTMANAGERCREATOR_H
#define IREPORTMANAGERCREATOR_H

#include "ilogic/ireportmanager.h"
#include "idatamodel/iobjectfactory.h"
#include "idatamodelserializer/iserializerfactory.h"

namespace gui
{

struct IReportManagerCreator
{
    virtual ~IReportManagerCreator() = default;

    virtual gui::unique_ptr<IReportManager> create(const gui::IObjectFactory*,
                                                   const gui::ISerializerFactory*) const = 0;
};

}

#endif // IREPORTMANAGERCREATOR_H
