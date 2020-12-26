#ifndef COBJECTFACTORY_H
#define COBJECTFACTORY_H

#include "idatamodel/iobjectfactory.h"

namespace datamodel
{
    class CObjectFactory : public gui::IObjectFactory
    {
    public:
        gui::unique_ptr<gui::IDataObject> get(std::wstring typeId) const override;
    };
}

#endif // COBJECTFACTORY_H
