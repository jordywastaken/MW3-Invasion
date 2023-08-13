
#pragma once

#include <stdarg.h>
#include <stdint.h>

// sysPrxForUser library imports
extern "C"
{
    int _sys_printf(const char* format, ...);
    int _sys_sprintf(char* s, const char* format, ...);
    int _sys_snprintf(char* buffer, size_t len, const char* fmt, ...);
    int _sys_vsprintf(char* s, const char* format, va_list arg);
    int _sys_vsnprintf(char* s, size_t n, const char* fmt, va_list arg);

    void* _sys_malloc(size_t n);
    void _sys_free(void* ptr);
    void* _sys_memalign(size_t boundary, size_t size);;

    void* _sys_memchr(const void* s, int c, size_t n);
    int _sys_memcmp(const void* s1, const void* s2, size_t n);
    void* _sys_memcpy(void* dest, const void* src, size_t n);
    void* _sys_memset(void* m, int c, size_t n);
    void* _sys_memmove(void* str1, const void* str2, size_t n);

    char* _sys_strcat(char* dest, const char* src);
    char* _sys_strchr(const char* s, int c);
    int _sys_strcmp(const char* s1, const char* s2);
    char* _sys_strcpy(char* dest, const char* src);
    size_t _sys_strlen(const char* str);

    char* _sys_strncat(char* dest, const char* src, size_t n);
    char* _sys_strrchr(const char* s, int c);
    int _sys_strncmp(const char* s1, const char* s2, size_t n);
    int _sys_strncasecmp(const char* s1, const char* s2, size_t n);
    char* _sys_strncpy(char* dest, const char* src, size_t n);

    char _sys_tolower(char c);
    char _sys_toupper(char c);
}

static int atoi(const char* str)
{
    int result = 0;
    while (*str)
        result = (result * 10) + (*str++ - '0');
    return result;
}