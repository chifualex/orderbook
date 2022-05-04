#include "CommUdpClientImpl.h"
#include "FileStreamerImpl.h"
#include "Queue.h"
#include "ThreadManager.h"
#include "OrderBookManager.h"
#include "Factory.h"
#include "Publisher.h"
#include "UnitTests.h"
#include <atomic>
#include <csignal>

#define DEFAULT_PORT 5000

static volatile std::atomic<bool> keepRunning = true;

void intHandler(int dummy) {
	keepRunning = false;
}

int validateInputParameters(int argc, char *argv[], IStreamClient::StreamClientType& clientType, 
							 unsigned short& port, std::string& filePath)
{
	if (argc == 1)
	{
		std::cout << "Default mode : UDP communication on default port : " << DEFAULT_PORT << std::endl;
	}
	else if (argc == 3)
	{
		std::string mode = argv[1];

		std::string filePath = argv[2];

		if (mode == "UDP")
		{
			port = std::atoi(argv[2]);

			if (port == 0)
			{
				port = DEFAULT_PORT;
				std::cout << "Invalid port, proceed with default port " << port << std::endl;
			}

			std::cout << "UDP communication on port " << port << std::endl;
			clientType = IStreamClient::StreamClientType::UDP;
		}
		else if (mode == "File")
		{
			std::cout << "File mode communication" << std::endl;
			std::cout << "Proceed to parse input file : " << argv[2] << std::endl;
			clientType = IStreamClient::StreamClientType::FILE;
		}
		else
		{
			std::cout << "Invalid parameters" << std::endl;
			std::cout << "Start in default mode : UDP communication on default port : " << port << std::endl;
		}
	}
	else
	{
		std::cout << "Invalid parameters" << std::endl;
		std::cout << "Start in default mode : UDP communication on default port : " << port << std::endl;
	}

	return true;
}

int main(int argc, char *argv[])
{	
	signal(SIGABRT, intHandler);

	std::string filePath = "";
	unsigned short port = DEFAULT_PORT;
	IStreamClient::StreamClientType clientType = IStreamClient::StreamClientType::UDP;

	if (!validateInputParameters(argc, argv, clientType, port, filePath))
	{
		return 1;
	}
	
	/* Create processing and publishing queues */
	Queue<std::string> orderBookProcessingQueue;
	Queue<std::string> publishingQueue;

	/* Create main class objects - OrderBookManager, ThreadManager and Publisher */
	OrderBookManager* orderBookManager = new OrderBookManager(orderBookProcessingQueue, publishingQueue);
	ThreadManager* threadManager = new ThreadManager();
	Publisher* publisher = new Publisher(false, true, publishingQueue);
	//Logger::getLogger().setFilePath();

	IStreamClient* client;
	
	if (clientType == IStreamClient::StreamClientType::FILE)
	{
		/* Create File reader */
		client = new FileStreamerImpl(filePath, orderBookProcessingQueue);
	}
	else
	{
		/* Create UDP client */
		client = new CommUdpClientImpl(port, orderBookProcessingQueue);
	}

	while (keepRunning)
	{
		try
		{
			/* Open client connection or File reader */
			client->open();

			/* Create Input streamer and publisher threads */
			threadManager->createThread(client, &IStreamClient::read);
			threadManager->createThread(publisher, &Publisher::processPublishingMessages);
			
			/* Process streams from client reader */
			orderBookManager->processStreamingTask();
		}
		catch (std::exception& e)
		{
			std::cout << e.what();
		}
	}

	publisher->setThreadStatus(false);
	client->setThreadStatus(false);
	client->close();
	threadManager->joinAll();

	delete orderBookManager;
	delete client;
	delete threadManager;
	delete publisher;

	std::cout << "Exiting .." << std::endl;
}
