#ifndef CUSTOMUNIQUEPTR_H
#define CUSTOMUNIQUEPTR_H

#pragma warning(push, 0)
#include <memory>
#include <functional>
#pragma warning(pop)

namespace gui
{
    template <class T>
    using unique_ptr = std::unique_ptr<T, std::function<void(void*)>>;

    template <class T>
    std::function<void(void*)> get_erased_deleter()
    {
        return [] (void *p) { delete static_cast<T*>(p); };
    }

    template <class T, class... Args>
    auto make_unique(Args&&... args)
    {
        return unique_ptr<T> {new T {std::forward<Args>(args)...}, get_erased_deleter<T>() };
    }

    template <class To, class From>
    unique_ptr<To> dynamic_unique_cast(unique_ptr<From> &&p)
    {
        if (To *cast = dynamic_cast<To*>(p.get()))
        {
            unique_ptr<To> result(cast, std::move(p.get_deleter()));
            p.release();
            return result;
        }
        throw std::bad_cast();
    }
}

#endif // CUSTOMUNIQUEPTR_H
