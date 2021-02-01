#ifndef CREPORTMANAGERCREATOR_H
#define CREPORTMANAGERCREATOR_H

#include "ilogic/ireportmanagercreator.h"

namespace logic
{
    class CReportManagerCreator : public gui::IReportManagerCreator
    {
    public:
        gui::unique_ptr<gui::IReportManager> create(const gui::IObjectFactory*,
                                                    const gui::ISerializerFactory*) const override;
    };
}

#endif // CREPORTMANAGERCREATOR_H
