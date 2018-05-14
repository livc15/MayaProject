#include "ComLib.h"

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

bool ComLib::send(const void* msg, const size_t length)
{
	this->calcFreeMem();

	int msgSize = sizeof(Header) + length;
	int blockCount = ceil(msgSize / 64.f);
	int pad = (blockCount * 64) - msgSize;
	int totalBlockSize = msgSize + pad;

	if (this->freeMemSize > totalBlockSize)
	{
		if (this->mSize - msgSize <= *this->head)
		{
			if (*this->tail == 0)
			{
				return send(msg, length);
			}
			h.msgId = DUMMY;
			h.msgSeq = 0;
			h.msgLength = mSize - *this->head;

			memcpy(this->mData + *this->head, &h, sizeof(Header));
			*this->head = 0;

			return send(msg, length);
		}
		else
		{

			/*h.msgId = NORMAL;
			h.msgSeq = 0;
			h.msgLength = length;

			memcpy(this->mData + *this->head, &h, sizeof(Header));
			*this->head += sizeof(Header);

			memcpy(this->mData + *this->head, msg, length);
			*this->head += length + pad;

			if (*this->head == mSize)
			{
				*this->head = 0;
			}
			return true;*/
		}
	}
	else
	{
		return false;
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



