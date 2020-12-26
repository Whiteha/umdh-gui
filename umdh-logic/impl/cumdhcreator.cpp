#include "cumdhcreator.h"
#include "cumdh.h"

using namespace logic;

gui::unique_ptr<gui::IUmdh> CUmdhCreator::create(gui::ISettings *pSettings) const
{
    return gui::make_unique<CUmdh>(pSettings);
}
