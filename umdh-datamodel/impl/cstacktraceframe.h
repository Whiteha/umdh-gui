#ifndef CSTACKTRACEFRAME_H
#define CSTACKTRACEFRAME_H

#include "idatamodel/idataobject.h"
#include "idatamodel/iclonable.h"
#include "idatamodel/istacktraceframe.h"
#include "idatamodel/iobjectfactory.h"

namespace datamodel
{
    class CStackTraceFrame :
            public gui::IDataObject,
            public gui::IClonable,
            public gui::IStackTraceFrame
    {
    public:
        CStackTraceFrame(const gui::IObjectFactory*, std::wstring typeId);

        // gui::IDataObject
        std::wstring type() const override;

        // gui::IClonable,
        gui::unique_ptr<IClonable> clone() const override;

        // gui::IStackTraceFrame
        void setModule(std::wstring) override;
        std::wstring getModule() const override;

        void setFunction(std::wstring) override;
        std::wstring getFunction() const override;

        void setOffset(size_t) override;
        size_t getOffset() const override;

        void setSource(std::wstring) override;
        std::wstring getSource() const override;

        void setSourceLine(size_t) override;
        size_t getSourceLine() const override;

        void setUnknownAddress(std::wstring) override;
        std::wstring getUnknownAddress() const override;

    private:
        const gui::IObjectFactory *m_pObjectFactory = nullptr;
        const std::wstring m_typeId;
        struct CopyableState
        {
            std::wstring module;
            std::wstring function;
            size_t offset = 0;
            std::wstring source;
            size_t sourceLine = 0;
            std::wstring unknownAddress;
        } m_copyableState;
    };
}

#endif // CSTACKTRACEFRAME_H
