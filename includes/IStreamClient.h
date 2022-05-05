#ifndef ISTREAMCLIENT_HPP
#define ISTREAMCLIENT_HPP

#include <string>
#include <iostream>

class IStreamClient
{
public:
	typedef enum
	{
		FILE,
		UDP,
		TCP
	}StreamClientType;

	/* Method used to read data from a communication channel */
	virtual void read() = 0;
	
	/* Method used to open a communication channel */
	virtual void open() = 0;
	
	/* Method used to close the communication channel */
	virtual void close() = 0;

	/* Method used to get the typeof a communication channel */
	virtual StreamClientType getCommType() = 0;

	virtual ~IStreamClient() {}
};

#endif
