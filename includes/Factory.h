#ifndef FACTORY_H
#define FACTORY_H

#include "CommUdpClientImpl.h"
#include "FileStreamerImpl.h"

class Factory
{
public:

	static IStreamClient* createStreamObject(IStreamClient::StreamClientType type, 
											 Queue<std::string>& processingQueue, std::string filePath, unsigned short port)
	{
		if (type == IStreamClient::StreamClientType::FILE)
		{
			return new FileStreamerImpl(filePath, processingQueue);
		}
		else if(type == IStreamClient::StreamClientType::UDP)
		{
			return new CommUdpClientImpl(port, processingQueue);
		}
		else
		{
			return new CommUdpClientImpl(port, processingQueue);
		}
	}
};

#endif
