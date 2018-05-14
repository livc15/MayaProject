#pragma once

#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

#define THREADCOUNT 2

class Mutex
{
private:
	HANDLE handle;

public:
	Mutex()
	{
		handle = CreateMutex(nullptr, false, "Global\\sharedMutex");

		if (handle == NULL)
		{
			printf("CreateMutex error: %d\n", GetLastError());
			getchar();
			exit(0);
		}
	}
	~Mutex()
	{
		ReleaseMutex(handle);
	}
	void Lock(DWORD milliseconds = INFINITE)
	{
		WaitForSingleObject(handle, milliseconds);
	}
	void Unlock()
	{
		ReleaseMutex(handle);
	}
};

class ComLib
{
public:
	enum MSG_TYPE { MESSAGE, MESH, CAMERA, LIGHT, DUMMY };
	struct Header
	{
		MSG_TYPE msgId;
		size_t msgSeq;
		size_t msgLength;
	};

	// create a ComLib
	ComLib(const std::string& secret, const size_t& buffSize);
	/* disconnect and destroy all resources */
	~ComLib();

	// init and check status
	bool connect();
	bool isConnected();

	// returns "true" if data was sent successfully.
	// false if for any reason the data could not be sent.
	bool send(const void* msg, const size_t length);

	/* return the length of the next message */
	size_t nextSize();
	void calcFreeMem();

	HANDLE hFileMap;
	char* mData;
	bool exits = false;
	unsigned int mSize = 0;
	unsigned int freeMemSize = 0;

	Header h;
	size_t* head;
	size_t* tail;
};

