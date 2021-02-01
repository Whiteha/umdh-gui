#ifndef ISTACKTRACEFRAME_H
#define ISTACKTRACEFRAME_H

#include "iserializable.h"

#pragma warning(push, 0)
#include <string>
#pragma warning(pop)

namespace gui
{    
    struct IStackTraceFrame
    {
        virtual ~IStackTraceFrame() = default;

        virtual void setModule(std::wstring) = 0;
        virtual std::wstring getModule() const = 0;

        virtual void setFunction(std::wstring) = 0;
        virtual std::wstring getFunction() const = 0;

        virtual void setOffset(size_t) = 0;
        virtual size_t getOffset() const = 0;

        virtual void setSource(std::wstring) = 0;
        virtual std::wstring getSource() const = 0;

        virtual void setSourceLine(size_t) = 0;
        virtual size_t getSourceLine() const = 0;

        virtual void setUnknownAddress(std::wstring) = 0;
        virtual std::wstring getUnknownAddress() const = 0;
    };
}

#endif // ISTACKTRACEFRAME_H
