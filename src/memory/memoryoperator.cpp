#include "memorymanager.h"
#include <cstddef>
using namespace std;

using namespace _462;

//new section
//only turn on if using windows, visual c++ compiler
#ifdef _MSC_VER
void * operator new(size_t noBytes)
{
	return MemoryManager::GetInstance()->Alloc(noBytes, __FILE__, __LINE__);
}

void * operator new[](size_t noBytes)
{
	return MemoryManager::GetInstance()->Alloc(noBytes, __FILE__, __LINE__);
}

void operator delete (void * p)
{
	return MemoryManager::GetInstance()->Free(p);
}

void operator delete[](void * p)
{
	return MemoryManager::GetInstance()->Free(p);
}
#endif

void * operator new(size_t noBytes, char * fileName, unsigned long line)
{
	return MemoryManager::GetInstance()->Alloc(noBytes, fileName, line);
}

void * operator new[](size_t noBytes, char * fileName, unsigned long line)
{
	return MemoryManager::GetInstance()->Alloc(noBytes, fileName, line);
}

void operator delete (void * p, char * fileName, unsigned long line)
{
	return MemoryManager::GetInstance()->Free(p);
}

void operator delete[](void * p, char * fileName, unsigned long line)
{
	return MemoryManager::GetInstance()->Free(p);
}

//malloc section
void * CustomAlloc(size_t noBytes)
{
	return MemoryManager::GetInstance()->Alloc(noBytes, __FILE__, __LINE__);
}

void * CustomAllocDbg(size_t noBytes, char * fileName, unsigned long line)
{
	return MemoryManager::GetInstance()->Alloc(noBytes, fileName, line);
}

void CustomFree(void * p)
{
	return MemoryManager::GetInstance()->Free(p);
}
