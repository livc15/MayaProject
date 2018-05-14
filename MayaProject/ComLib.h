#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>

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
	bool send(const char* msg, const size_t length);
	/*
	returns: "true" if a message was received.
	"false" if there is nothing to read.
	"msg" is expected to have enough space, use "nextSize" to
	check this and allocate if needed, but outside ComLib.
	length should indicate the length of the data read.
	Should never return DUMMY messages.
	*/
	bool recv(char* msg, size_t& length);
	/* return the length of the next message */
	size_t nextSize();
	void calcFreeMem();
	void gen_random(char *s, const int len);

	HANDLE hFileMap;
	char* mData;
	bool exits = false;
	unsigned int mSize = 0;
	unsigned int freeMemSize = 0;

	Header h;
	size_t* head;
	size_t* tail;
};

