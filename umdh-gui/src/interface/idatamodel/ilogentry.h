#ifndef ILOGENTRY_H
#define ILOGENTRY_H

#include "istacktraceframe.h"
#include "iserializable.h"
#include "customuniqueptr.h"

#pragma warning(push, 0)
#include <string>
#include <vector>
#pragma warning(pop)

namespace gui
{
    struct ILogEntry
    {
        virtual ~ILogEntry() = default;

        virtual void setTraceId(std::wstring) = 0;
        virtual std::wstring getTraceId() const = 0;

        virtual void setNewCount(size_t) = 0;
        virtual size_t getNewCount() const = 0;

        virtual void setOldCount(size_t) = 0;
        virtual size_t getOldCount() const = 0;

        virtual void setNewBytes(size_t) = 0;
        virtual size_t getNewBytes() const = 0;

        virtual void setOldBytes(size_t) = 0;
        virtual size_t getOldBytes() const = 0;

        virtual void setStackTrace(std::vector<unique_ptr<IStackTraceFrame>>) = 0;
        virtual std::vector<unique_ptr<IStackTraceFrame>> getStackTrace() const = 0;
    };
}

#endif // ILOGENTRY_H
