#include "UnitTests.h"
#include "CommUdpClientImpl.h"
#include "FileStreamerImpl.h"
#include "Queue.h"
#include "ThreadManager.h"
#include "OrderBookManager.h"
#include "Factory.h"
#include "Publisher.h"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <gtest/gtest.h>

std::string filePath;
std::stringstream outputBuffer, expectedOutputbuffer;

void run_performance_test(std::string& path)
{
	std::string performanceFilePath = path + "\\performanceFile.csv";

	remove(performanceFilePath.c_str());

	std::ofstream performanceFile;
  	performanceFile.open(performanceFilePath);

	for (int i = 0; i < 10000; i++)
	{
		performanceFile << "N, " << i << ", IBM, 10, 100, B, " << i << "\n";
	}

	for (int i = 10000; i < 20000; i++)
	{
		performanceFile << "N, " << i << ", IBM, 11, 100, S, " << i << "\n";
	}

	for (int i = 20000; i < 30000; i++)
	{
		performanceFile << "N, " << i << ", IBM, 11, 100, B, " << i << "\n";
	}

	for (int i = 30000; i < 40000; i++)
	{
		performanceFile << "N, " << i << ", IBM, 10, 100, S, " << i << "\n";
	}

	performanceFile.close();

	/* Create processing and publishing queues */
	Queue<std::string> orderBookProcessingQueue;
	Queue<std::string> publishingQueue;

	/* Create main class objects - OrderBookManager, ThreadManager and Publisher */
	OrderBookManager* orderBookManager = new OrderBookManager(orderBookProcessingQueue, publishingQueue);
	ThreadManager* threadManager = new ThreadManager();
	Publisher* publisher = new Publisher(false, false, publishingQueue);
	
	BookEntry bookEntry;

	IStreamClient* client = new FileStreamerImpl(performanceFilePath, orderBookProcessingQueue);
	threadManager->createThread(publisher, &Publisher::processPublishingMessages);

	client->open();
	client->read();

	std::string stream;

	std::chrono::steady_clock::time_point start, end;
	start = std::chrono::steady_clock::now();

	while (true)
	{
		std::string stream = orderBookProcessingQueue.pop();

		if (stream == "Exit")
		{
			break;
		}

		std::string result = StreamDeserializer::deserializeStream(stream, bookEntry);

		orderBookManager->addOrUpdateOrderBook(result, bookEntry);
	}

	end = std::chrono::steady_clock::now();

	std::cout << "Elapsed orderBook update time in seconds: "
		<< std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
		<< " s" << std::endl;

	client->close();
	Logger::getLogger().closeLogFile();

	publishingQueue.push("Exit");
	publisher->setThreadStatus(false);
	threadManager->joinAll();

	delete orderBookManager;
	delete client;
	delete threadManager;
	delete publisher;
}

void RunTest(std::string& path, int indexFile, std::stringstream& outputBuffer, std::stringstream& expectedOutputBuffer)
{
	std::string expectedOutputFilePath = path + "\\outputs\\expectedOutput" + std::to_string(indexFile + 1) + ".csv";
	std::string outputFilePath = path + "\\outputs\\outputFile" + std::to_string(indexFile + 1) + ".csv";
	std::string inputFilePath = path + "\\inputs\\inputFile" + std::to_string(indexFile + 1) + ".csv";

	remove(outputFilePath.c_str());

	/* Create processing and publishing queues */
	Queue<std::string> orderBookProcessingQueue;
	Queue<std::string> publishingQueue;

	/* Create main class objects - OrderBookManager, ThreadManager and Publisher */
	OrderBookManager* orderBookManager = new OrderBookManager(orderBookProcessingQueue, publishingQueue);
	ThreadManager* threadManager = new ThreadManager();
	Publisher* publisher = new Publisher(true, true, publishingQueue);
	
	Logger::getLogger().setFilePath(outputFilePath);

	IStreamClient* client = new FileStreamerImpl(inputFilePath, orderBookProcessingQueue);
	threadManager->createThread(publisher, &Publisher::processPublishingMessages);

	client->open();
	client->read();

	BookEntry bookEntry;
	std::string stream;

	while (true)
	{
		stream = orderBookProcessingQueue.pop();

		if (stream == "Exit")
		{
			break;
		}

		std::string result = StreamDeserializer::deserializeStream(stream, bookEntry);

		orderBookManager->addOrUpdateOrderBook(result, bookEntry);
	}

	client->close();
	Logger::getLogger().closeLogFile();

	std::ifstream outputFile, expectedOutputFile;

	expectedOutputFile.open(expectedOutputFilePath.c_str(), std::ios::in);
	outputFile.open(outputFilePath.c_str(), std::ios::in);

	outputBuffer.str("");
	expectedOutputBuffer.str("");
	outputBuffer << outputFile.rdbuf();
	expectedOutputbuffer << expectedOutputFile.rdbuf();

	publishingQueue.push("Exit");
	publisher->setThreadStatus(false);
	threadManager->joinAll();

	delete orderBookManager;
	delete client;
	delete threadManager;
	delete publisher;
}

TEST(FileTests, TestScenario1)
{
    RunTest(filePath, 0, outputBuffer, expectedOutputbuffer);
	ASSERT_EQ(outputBuffer.str(), expectedOutputbuffer.str());
}

TEST(FileTests, TestScenario2)
{
    RunTest(filePath, 1, outputBuffer, expectedOutputbuffer);
	ASSERT_EQ(outputBuffer.str(), expectedOutputbuffer.str());
}

TEST(FileTests, TestScenario3)
{
    RunTest(filePath, 2, outputBuffer, expectedOutputbuffer);
	ASSERT_EQ(outputBuffer.str(), expectedOutputbuffer.str());
}

TEST(FileTests, TestScenario4)
{
    RunTest(filePath, 3, outputBuffer, expectedOutputbuffer);
	ASSERT_EQ(outputBuffer.str(), expectedOutputbuffer.str());
}

TEST(FileTests, TestScenario5)
{
    RunTest(filePath, 4, outputBuffer, expectedOutputbuffer);
	ASSERT_EQ(outputBuffer.str(), expectedOutputbuffer.str());
}

TEST(FileTests, TestScenario6)
{
    RunTest(filePath, 5, outputBuffer, expectedOutputbuffer);
	ASSERT_EQ(outputBuffer.str(), expectedOutputbuffer.str());
}

TEST(FileTests, TestScenario7)
{
    RunTest(filePath, 6, outputBuffer, expectedOutputbuffer);
	ASSERT_EQ(outputBuffer.str(), expectedOutputbuffer.str());
}

TEST(FileTests, TestScenario8)
{
    RunTest(filePath, 7, outputBuffer, expectedOutputbuffer);
	ASSERT_EQ(outputBuffer.str(), expectedOutputbuffer.str());
}

TEST(FileTests, TestScenario9)
{
    RunTest(filePath, 8, outputBuffer, expectedOutputbuffer);
	ASSERT_EQ(outputBuffer.str(), expectedOutputbuffer.str());
}

TEST(FileTests, TestScenario10)
{
    RunTest(filePath, 9, outputBuffer, expectedOutputbuffer);
	ASSERT_EQ(outputBuffer.str(), expectedOutputbuffer.str());
}

TEST(FileTests, TestScenario11)
{
    RunTest(filePath, 10, outputBuffer, expectedOutputbuffer);
	ASSERT_EQ(outputBuffer.str(), expectedOutputbuffer.str());
}

TEST(FileTests, TestScenario12)
{
    RunTest(filePath, 11, outputBuffer, expectedOutputbuffer);
	ASSERT_EQ(outputBuffer.str(), expectedOutputbuffer.str());
}

TEST(FileTests, TestScenario13)
{
    RunTest(filePath, 12, outputBuffer, expectedOutputbuffer);
	ASSERT_EQ(outputBuffer.str(), expectedOutputbuffer.str());
}

TEST(FileTests, TestScenario14)
{
    RunTest(filePath, 13, outputBuffer, expectedOutputbuffer);
	ASSERT_EQ(outputBuffer.str(), expectedOutputbuffer.str());
}

TEST(FileTests, TestScenario15)
{
    RunTest(filePath, 14, outputBuffer, expectedOutputbuffer);
	ASSERT_EQ(outputBuffer.str(), expectedOutputbuffer.str());
}

TEST(FileTests, TestScenario16)
{
    RunTest(filePath, 15, outputBuffer, expectedOutputbuffer);
	ASSERT_EQ(outputBuffer.str(), expectedOutputbuffer.str());
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "Please provide the path for input scenario files" << std::endl;		
	}

	filePath = argv[1];
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}