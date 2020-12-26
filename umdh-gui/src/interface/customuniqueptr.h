#ifndef CUSTOMUNIQUEPTR_H
#define CUSTOMUNIQUEPTR_H

#include <memory>
#include <functional>

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
    unique_ptr<T> make_unique(Args&&... args)
    {
        return {new T {args...}, get_erased_deleter<T>() };
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
        return unique_ptr<To>(nullptr);
    }
}

#endif // CUSTOMUNIQUEPTR_H
