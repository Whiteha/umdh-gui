#ifndef CLOADEDMODULE_H
#define CLOADEDMODULE_H

#include "idatamodel/idataobject.h"
#include "idatamodel/iclonable.h"
#include "idatamodel/iloadedmodule.h"
#include "idatamodel/iobjectfactory.h"

namespace datamodel
{
    class CLoadedModule :
            public gui::IDataObject,
            public gui::IClonable,
            public gui::ILoadedModule
    {
    public:
        CLoadedModule(const gui::IObjectFactory*, std::wstring typeId);

        // gui::IDataObject
        std::wstring type() const override;

        // gui::IClonable,
        gui::unique_ptr<IClonable> clone() const override;

        // gui::ILoadedModule
        void setModuleName(std::wstring) override;
        std::wstring getModuleName() const override;

        void setAddrBegin(size_t) override;
        size_t getAddrBegin() const override;

        void setAddrEnd(size_t) override;
        size_t getAddrEnd() const override;

        void setIsSymbolsLoaded(bool) override;
        bool getIsSymbolsLoaded() const override;

        void setSymbolsPath(std::wstring) override;
        std::wstring getSymbolsPath() const override;

    private:
        const gui::IObjectFactory *m_pObjectFactory = nullptr;
        const std::wstring m_typeId;
        struct CopyableState
        {
            std::wstring moduleName;
            size_t addrBegin = 0;
            size_t addrEnd = 0;
            bool isSymbolsLoaded = false;
            std::wstring symbolsPath;
        } m_copyableState;
    };
}

#endif // CLOADEDMODULE_H
