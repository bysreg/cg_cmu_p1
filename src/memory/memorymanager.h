#pragma once

namespace _462 {
	struct AllocBuffers
	{
		void* pAddress;
		unsigned int length;
		char* fileName;
		unsigned long line;
	};

	// TODO : prevent from copyable
	class MemoryManager
	{
	private:
		MemoryManager();
		~MemoryManager();

		static const int MAX_ALLOC = 1000;
		
		AllocBuffers m_dataBuffers[MAX_ALLOC];
		unsigned int m_noBuffers;

	public:
		static MemoryManager * GetInstance();
		void* Alloc(unsigned int noBytes, char* fileName, unsigned int line);
		void Free(void * pAddress);
		void Dump();
		void SanityCheck(bool bShowStats = false);
		void Error(char* szMessage);
	};
}