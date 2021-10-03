#ifndef NEW_H
#define NEW_H

#ifdef __cplusplus

#include <stdlib.h>

void* operator new (size_t size) _GLIBCXX_THROW (std::bad_alloc);
void* operator new[] (size_t size) _GLIBCXX_THROW (std::bad_alloc);
void operator delete (void* ptr) _GLIBCXX_USE_NOEXCEPT;
void operator delete[] (void* ptr) _GLIBCXX_USE_NOEXCEPT;
void operator delete (void* ptr, size_t size) _GLIBCXX_USE_NOEXCEPT;
void operator delete[] (void* ptr, size_t size) _GLIBCXX_USE_NOEXCEPT;


#endif // __cplusplus

#endif
