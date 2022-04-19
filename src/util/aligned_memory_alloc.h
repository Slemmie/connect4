// malloc but paged...

#pragma once

#ifndef __linux__
#warning "aligned memory allocation is unoptimized on the detected operating system"
#endif

#include <cstdlib>
#include <cstddef>

inline void* _aligned_allocate(size_t alignment, size_t size) {
#if defined(POSIXALIGNEDALLOC)
	void* result = nullptr;
	if (posix_memalign(&result, alignment, size)) {
		return nullptr;
	}
	return result;
#elif defined(_WIN32)
	return _mm_alloc(size, alignment);
#else
	return std::aligned_alloc(alignment, size);
#endif
}

inline void* aligned_large_allocate(size_t size) {
	constexpr const size_t alignment = static_cast <size_t> (2 * 1024 * 1024);
	size = alignment * ((size + alignment - 1) / alignment);
	return _aligned_allocate(alignment, size);
}

inline void _aligned_large_free(void* ptr) {
#if defined(POSIXALIGNEDALLOC)
	free(ptr);
#elif defined(_WIN32)
	_mm_free(ptr);
#else
	free(ptr);
#endif
}

inline void aligned_large_free(void* ptr) {
	_aligned_large_free(ptr);
}
