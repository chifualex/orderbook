#include "Queue.h"
#include "Factory.h"
#include "FileStreamerImpl.h"
#include "CommUdpImpl.h"
#include "CommTcpImpl.h"
#include "ThreadManager.h"
#include "OrderBookManager.h"
#include "Publisher.h"
#include "UnitTests.h"
#include <atomic>

IStreamClient::StreamClientType m_type = IStreamClient::StreamClientType::TCP;
std::string m_filePath = "";

bool validateInputParameters(int argc, char *argv[])
{
	if (argc == 2)
	{
		if (argv[1] == "UDP")
		{
			std::cout << "UDP mode" << "\n";
			m_type = IStreamClient::StreamClientType::UDP;
		}
		else if (argv[1] == "TCP")
		{
			std::cout << "TCP mode" << "\n";
			m_type = IStreamClient::StreamClientType::TCP;
		}
	}
	else if (argc == 3)
	{
		if (argv[1] == "FILE")
		{
			m_type = IStreamClient::StreamClientType::FILE;
			m_filePath = argv[2];
		}
		else
		{
			std::cout << "Invalid parameters, please provide the stream type !" << "\n";
		}
	}
	else
	{
		std::cout << "Invalid parameters, please provide the input file path !" << "\n";
		return false;
	}

	return true;
}

int main(int argc, char *argv[])
{	
	if (!validateInputParameters(argc, argv))
	{
		return 1;
	}
	
	/* Create processing and publishing queues */
	Queue<std::string> orderBookProcessingQueue;
	Queue<std::string> publishingQueue;
	
	/* Create main class objects - OrderBookManager, ThreadManager, Publisher and IStreamClient */
	std::shared_ptr<OrderBookManager> p_orderBookManager(new OrderBookManager(orderBookProcessingQueue, publishingQueue));
	std::shared_ptr<Publisher> p_publisher(new Publisher(false, true, publishingQueue));
	std::shared_ptr<ThreadManager> p_threadManager(new ThreadManager());
	std::shared_ptr<IStreamClient> p_streamer(Factory::createStreamObject(m_type, orderBookProcessingQueue, m_filePath));

	/* Open client connection or File reader */
	p_streamer->Open();

	try
	{
		/* Create Input streamer and publisher threads */
		p_threadManager->createThread(p_streamer, &IStreamClient::Read);
		p_threadManager->createThread(p_publisher, &Publisher::processPublishingMessages);
		
		/* Process streams from client reader */
		p_orderBookManager->processStreamingTask();
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}

	p_streamer->Close();
	p_threadManager->joinAll();

	std::cout << "Exiting .." << "\n";
}
