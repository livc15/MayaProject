#include "ComLib.h"
#include <mutex>

ComLib::ComLib(const std::string& filemap, const size_t& buffSize) : mSize(buffSize)
{
	this->hFileMap = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		(DWORD)0,
		mSize + sizeof(size_t) * 2,
		(LPCSTR) "sharedFileMap");
	if (hFileMap == nullptr)
	{
		//Fatal error
		std::cout << "Fatal error: couldn't create filemap" << std::endl;
		exit(-1);
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		//handle sharing
		char* data = nullptr;
		data = (char*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);

		if (data == nullptr)
		{
			std::cout << "View map couldn't be created!" << std::endl;
			exit(-1);
		}

		this->head = ((size_t*)data);
		this->tail = ((size_t*)data) + 1;
		this->mData = ((char*)this->tail) + sizeof(size_t);
	}
	else
	{
		char* data = nullptr;
		data = (char*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);

		if (data == nullptr)
		{
			std::cout << "View map couldn't be created!" << std::endl;
			exit(-1);
		}
		this->head = ((size_t*)data);
		this->tail = ((size_t*)data) + 1;
		*this->head = 0;
		*this->tail = 0;
		this->mData = ((char*)this->tail) + sizeof(size_t);
	}
}
ComLib::~ComLib()
{
	UnmapViewOfFile((LPCVOID)mData);
	CloseHandle(hFileMap);
}

bool ComLib::connect()
{
	return false;
}
bool ComLib::isConnected()
{
	return false;
}

bool ComLib::recv(char* msg, size_t& length)
{
	calcFreeMem();
	if (*this->tail == *this->head || length == 0)
	{
		return false;
	}
	else
	{
		int msgSize = length + sizeof(Header);
		int blockCount = ceil(msgSize / 64.f);
		int pad = (blockCount * 64.f) - msgSize;
		int totalBlockSize = msgSize + pad;

		memcpy(&h, this->mData + *this->tail, sizeof(Header));


		if (h.msgId == DUMMY || *this->tail == mSize)
		{
			*this->tail = 0;
			return false;
		}
		else
		{

			memcpy(msg, this->mData + *this->tail + sizeof(Header), length);
			*this->tail = (*this->tail + totalBlockSize) % mSize;
			return true;
		}
	}
}

size_t ComLib::nextSize()
{
	if (*this->head == *this->tail)
	{
		return 0;
	}
	else
	{
		if (((Header*)(this->mData + *this->tail))->msgId == DUMMY)
		{
			return ((Header*)(this->mData))->msgLength;
		}
		else
		{
			return ((Header*)(this->mData + *this->tail))->msgLength;
		}
	}
}

void ComLib::calcFreeMem()
{
	if (*this->head == *this->tail)
	{
		this->freeMemSize = mSize;
	}
	else if (*this->head > *this->tail)
	{
		int temp1 = mSize - *this->head;

		this->freeMemSize = temp1 + *this->tail;
	}
	else
	{
		this->freeMemSize = *this->tail - *this->head;
	}
}

void ComLib::gen_random(char *s, const int len)
{
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (auto i = 0; i < len; ++i) {
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	s[len - 1] = 0;
}



