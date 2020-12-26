#ifndef CSORTERFACTORYCREATOR_H
#define CSORTERFACTORYCREATOR_H

#include "ilogic/isorterfactorycreator.h"

namespace logic
{
    class CSorterFactoryCreator : public gui::ISorterFactoryCreator
    {
    public:
        gui::unique_ptr<gui::ISorterFactory> create() const override;
    };
}


#endif // CSORTERFACTORYCREATOR_H
