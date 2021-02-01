#ifndef CHOTPATHCALCULATOR_H
#define CHOTPATHCALCULATOR_H

#include "ihotpathcalculator.h"
#include "idatamodelserializer/iserializerfactory.h"

namespace logic
{
    class CHotPathCalculator : public logic::IHotPathCalculator
    {
    public:
        CHotPathCalculator(const gui::ISerializerFactory*);
        gui::CallsByCountMap calculateCallsCount(gui::IReport*) const override;

    private:
        const gui::ISerializerFactory *m_pSerializerFactory;
    };
}

#endif // CHOTPATHCALCULATOR_H
