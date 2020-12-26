#ifndef CTOTALINCREASE_H
#define CTOTALINCREASE_H

#include "idatamodel/idataobject.h"
#include "idatamodel/iclonable.h"
#include "idatamodel/itotalincrease.h"
#include "idatamodel/iobjectfactory.h"

namespace datamodel
{
    class CTotalIncrease :
            public gui::IDataObject,
            public gui::IClonable,
            public gui::ITotalIncrease
    {
    public:
        CTotalIncrease(const gui::IObjectFactory*, std::wstring typeId);

        // gui::IDataObject
        std::wstring type() const override;

        // gui::IClonable,
        gui::unique_ptr<IClonable> clone() const override;

        // gui::ITotalIncrease
        void setRequested(size_t) override;
        size_t getRequested() const override;

        void setOverhead(size_t) override;
        size_t getOverhead() const override;

    private:
        const gui::IObjectFactory *m_pObjectFactory = nullptr;
        const std::wstring m_typeId;
        struct CopyableState
        {
            size_t requested = 0;
            size_t overhead = 0;
        } m_copyableState;
    };
}

#endif // CTOTALINCREASE_H
