#ifndef ILOADEDMODULE_H
#define ILOADEDMODULE_H

#pragma warning(push, 0)
#include <string>
#pragma warning(pop)

namespace gui
{
    struct ILoadedModule
    {
        virtual ~ILoadedModule() = default;

        virtual void setModuleName(std::wstring) = 0;
        virtual std::wstring getModuleName() const = 0;

        virtual void setAddrBegin(size_t) = 0;
        virtual size_t getAddrBegin() const = 0;

        virtual void setAddrEnd(size_t) = 0;
        virtual size_t getAddrEnd() const = 0;

        virtual void setIsSymbolsLoaded(bool) = 0;
        virtual bool getIsSymbolsLoaded() const = 0;

        virtual void setSymbolsPath(std::wstring) = 0;
        virtual std::wstring getSymbolsPath() const = 0;
    };
}
#endif // ILOADEDMODULE_H
