#ifndef CFILTERFACTORYCREATOR_H
#define CFILTERFACTORYCREATOR_H

#include "ilogic/ifilterfactorycreator.h"

namespace logic
{
    class CFilterFactoryCreator : public gui::IFilterFactoryCreator
    {
    public:
        gui::unique_ptr<gui::IFilterFactory> create() const override;
    };
}


#endif // CFILTERFACTORYCREATOR_H
