#ifndef CUMDHCREATOR_H
#define CUMDHCREATOR_H

#include "ilogic/iumdhcreator.h"

namespace logic
{ 
    class CUmdhCreator : public gui::IUmdhCreator
    {
    public:
        gui::unique_ptr<gui::IUmdh> create(gui::ISettings*) const override;
    };
}


#endif // CUMDHCREATOR_H
