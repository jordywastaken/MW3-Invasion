
#include "imports.hpp"
#include <new>

extern "C"
{
    void* memcpy(void* dest, const void* src, size_t size)
    {
        return _sys_memcpy(dest, src, size);
    }
}

void* operator new(size_t size) throw(std::bad_alloc)
{
    return _sys_malloc(size);
}

void* operator new(size_t size, const std::nothrow_t&) throw()
{
    return _sys_malloc(size);
}

void* operator new(size_t size, size_t align)
{
    return _sys_memalign(align, size);
}

void* operator new(size_t size, size_t align, const std::nothrow_t&) throw()
{
    return _sys_memalign(align, size);
}

void* operator new[](std::size_t size) throw(std::bad_alloc)
{
    return _sys_malloc(size);
}

void* operator new[](std::size_t size, const std::nothrow_t&) throw()
{
    return _sys_malloc(size);
}

void* operator new[](size_t size, size_t align)
{
    return operator new(size, align);
}

void* operator new[](size_t size, size_t align, const std::nothrow_t&) throw()
{
    return operator new(size, align);
}

void operator delete(void* mem) throw()
{
    _sys_free(mem);
}

void operator delete(void* mem, const std::nothrow_t&) throw()
{
    _sys_free(mem);
}

void operator delete(void* ptr, void* prt2)
{
    return operator delete(ptr);
}

void operator delete(void* ptr, size_t align)
{
    return operator delete(ptr);
}

void operator delete(void* ptr, size_t align, const std::nothrow_t&) throw()
{
    return operator delete(ptr);
}

void operator delete(void* ptr, size_t align, void* prt2)
{
    return operator delete(ptr);
}

void operator delete[](void* mem) throw()
{
    _sys_free(mem);
}

void operator delete[](void* mem, const std::nothrow_t&) throw()
{
    _sys_free(mem);
}

void operator delete[](void* ptr, void* prt2)
{
    return operator delete(ptr, prt2);
}

void operator delete[](void* ptr, size_t align)
{
    return operator delete(ptr, align);
}

void operator delete[](void* ptr, size_t align, const std::nothrow_t&) throw()
{
    return operator delete(ptr, align);
}

void operator delete[](void* ptr, size_t align, void* prt2)
{
    return operator delete(ptr, align);
}