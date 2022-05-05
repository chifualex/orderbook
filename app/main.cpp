#include "FileStreamerImpl.h"
#include "Queue.h"
#include "ThreadManager.h"
#include "OrderBookManager.h"
#include "Publisher.h"
#include "UnitTests.h"
#include <atomic>

bool validateInputParameters(int argc, char *argv[], std::string& filePath)
{
	if (argc == 2)
	{
		filePath = argv[1];
	}
	else
	{
		std::cout << "Invalid parameters, please provide the input file path !" << std::endl;
		return false;
	}

	return true;
}

int main(int argc, char *argv[])
{	
	std::string filePath = "";

	if (!validateInputParameters(argc, argv, filePath))
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
	
	std::cout << filePath << std::endl;

	/* Create File reader */
	client = new FileStreamerImpl(filePath, orderBookProcessingQueue);

	/* Open client connection or File reader */
	client->open();

	try
	{
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

	client->close();
	threadManager->joinAll();

	delete orderBookManager;
	delete client;
	delete threadManager;
	delete publisher;

	std::cout << "Exiting .." << std::endl;
}
