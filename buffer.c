#include <stdlib.h>
#include <ctype.h>
#include "b64.h"

#ifdef b64_USE_CUSTOM_MALLOC
extern void* b64_malloc(size_t);
#endif

#ifdef b64_USE_CUSTOM_REALLOC
extern void* b64_realloc(void*, size_t);
#endif

// The number of buffers we need
size_t bufc = 0;

void* b64_buf_malloc()
{
	void* buf = b64_malloc(B64_BUFFER_SIZE);
	bufc = 1;
	return buf;
}

void* b64_buf_realloc(void* ptr, size_t size)
{
	if (size > bufc * B64_BUFFER_SIZE)
	{
		while (size > bufc * B64_BUFFER_SIZE) bufc++;
		void* buf = b64_realloc(ptr, B64_BUFFER_SIZE * bufc);
		if (!buf) return NULL;
		return buf;
	}

	return ptr;
}
