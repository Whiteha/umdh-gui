#ifndef CLOGENTRY_H
#define CLOGENTRY_H

#include "idatamodel/idataobject.h"
#include "idatamodel/iclonable.h"
#include "idatamodel/ilogentry.h"
#include "idatamodel/iobjectfactory.h"

namespace datamodel
{
    class CLogEntry :
            public gui::IDataObject,
            public gui::IClonable,
            public gui::ILogEntry
    {
    public:
        CLogEntry(const gui::IObjectFactory*, std::wstring typeId);

        // gui::IDataObject
        std::wstring type() const override;

        // gui::IClonable,
        gui::unique_ptr<IClonable> clone() const override;

        // gui::ILogEntry
        void setTraceId(std::wstring) override;
        std::wstring getTraceId() const override;

        void setNewCount(size_t) override;
        size_t getNewCount() const override;

        void setOldCount(size_t) override;
        size_t getOldCount() const override;

        void setNewBytes(size_t) override;
        size_t getNewBytes() const override;

        void setOldBytes(size_t) override;
        size_t getOldBytes() const override;

        void setStackTrace(std::vector<gui::unique_ptr<gui::IStackTraceFrame>>) override;
        std::vector<gui::unique_ptr<gui::IStackTraceFrame>> getStackTrace() const override;

    private:
        const gui::IObjectFactory *m_pObjectFactory = nullptr;
        const std::wstring m_typeId;
        struct CopyableState
        {
            std::wstring trace_id;
            size_t new_count = 0;
            size_t old_count = 0;
            size_t new_bytes = 0;
            size_t old_bytes = 0;
        } m_copyableState;
        std::vector<gui::unique_ptr<gui::IStackTraceFrame>> m_pStackTrace;
    };
}

#endif // CLOGENTRY_H
