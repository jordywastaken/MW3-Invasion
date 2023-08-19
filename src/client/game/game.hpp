
#pragma once

#include <sys/sys_types.h>

#include "enums.hpp"
#include "structs.hpp"

#include "common/utils/hooking.hpp"

#pragma diag_suppress 1070
template<typename T>
class symbol
{
public:
    constexpr symbol(uintptr_t obj)
        : sp_object_(obj)
    { }

    operator T* ()
    {
        volatile uintptr_t object_fptr[]{ this->sp_object_, hook::toc() };

        T* type = *reinterpret_cast<T*>(this->sp_object_);
        return (decltype(type))object_fptr;
    }

    T* get()
    {
        return this->sp_object_;
    }

    uintptr_t sp_object_;
};

template<typename T>
static T clamp(T value, T min, T max)
{
    return value < min ? min : value > max ? max : value;
}

#include "symbols.hpp"

#ifdef _DEBUG
#define debug_print(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define debug_print(fmt, ...)
#endif