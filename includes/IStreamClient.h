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
	virtual void Read() = 0;

	/* Method used to test performance data from a communication channel */
	virtual void TestPerformance() = 0;
	
	/* Method used to open a communication channel */
	virtual void Open() = 0;
	
	/* Method used to close the communication channel */
	virtual void Close() = 0;

	/* Method used to get the typeof a communication channel */
	virtual StreamClientType getCommType() = 0;

	virtual ~IStreamClient() {}
};

#endif
