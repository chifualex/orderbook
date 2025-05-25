#ifndef FACTORY_H
#define FACTORY_H

#include "CommUdpImpl.h"
#include "CommTcpImpl.h"
#include "FileStreamerImpl.h"
#include "Queue.h"

class Factory
{
public:

	static IStreamClient* createStreamObject(IStreamClient::StreamClientType type, 
											 Queue<std::string>& processingQueue, std::string filePath)
	{
		if (type == IStreamClient::StreamClientType::FILE)
		{
			return new FileStreamerImpl(filePath, processingQueue);
		}
		else if(type == IStreamClient::StreamClientType::TCP)
		{
			return new CommTcpImpl(processingQueue);
		}
		else if(type == IStreamClient::StreamClientType::UDP)
		{
			return new CommUdpImpl(processingQueue);
		}
		else
		{
			return new CommTcpImpl(processingQueue);
		}
	}
};

#endif