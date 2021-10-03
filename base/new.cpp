#include "new.h"
#include "helpers.h"

void* operator new (size_t size)
{
	if (void *ptr = malloc (size))
	{
		return (ptr);
	}

	throw std::bad_alloc{};
}

void* operator new[] (size_t size)
{
	if (void *ptr = malloc (size))
	{
		return (ptr);
	}

	throw std::bad_alloc{};
}

void operator delete (void* ptr) _GLIBCXX_USE_NOEXCEPT
{
	if (ptr)
	{
		free (ptr);
	}
}

void operator delete[] (void* ptr) _GLIBCXX_USE_NOEXCEPT
{
	if (ptr)
	{
		free (ptr);
	}
}

void operator delete (void* ptr, size_t size) _GLIBCXX_USE_NOEXCEPT
{
	if (ptr)
	{
		free (ptr);
	}
}

void operator delete[] (void* ptr, size_t size) _GLIBCXX_USE_NOEXCEPT
{
	if (ptr)
	{
		free (ptr);
	}
}
