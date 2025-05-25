#include "UnitTests.h"
#include "FileStreamerImpl.h"
#include "Queue.h"
#include "ThreadManager.h"
#include "OrderBookManager.h"
#include "Publisher.h"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <chrono>
#include <memory> 
#include <gtest/gtest.h>

std::string filePath;
std::stringstream outputBuffer, expectedOutputbuffer;

void RunTest(std::string& path, int indexFile, std::stringstream& outputBuffer, std::stringstream& expectedOutputBuffer)
{
	std::string expectedOutputFilePath = path + "/tests//outputs//expectedOutput" + std::to_string(indexFile + 1) + ".csv";
	std::string outputFilePath = path + "/tests/outputs/outputFile" + std::to_string(indexFile + 1) + ".csv";
	std::string inputFilePath = path + "/tests/inputs/inputFile" + std::to_string(indexFile + 1) + ".csv";

	remove(outputFilePath.c_str());

	/* Create processing and publishing queues */
	Queue<std::string> orderBookProcessingQueue;
	Queue<std::string> publishingQueue;

	/* Create main class objects - OrderBookManager, ThreadManager, Publisher and IStreamClient */
	std::shared_ptr<OrderBookManager> p_orderBookManager(new OrderBookManager(orderBookProcessingQueue, publishingQueue));
	std::shared_ptr<Publisher> p_publisher(new Publisher(true, true, publishingQueue));
	std::shared_ptr<ThreadManager> p_threadManager(new ThreadManager());
	std::shared_ptr<IStreamClient> p_streamer(new FileStreamerImpl(inputFilePath, orderBookProcessingQueue));
	
	Logger::getLogger().setFilePath(outputFilePath);

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
	Logger::getLogger().closeLogFile();

	std::ifstream outputFile(outputFilePath.c_str());
	std::ifstream expectedOutputFile;

	expectedOutputFile.open(expectedOutputFilePath.c_str(), std::ios::in);

	outputBuffer.str("");

	if (outputFile.is_open())
	{
		outputBuffer << outputFile.rdbuf();
		outputFile.close();
	}
    else
	{
		std::cout << "Unable to open file" << "\n";
	} 

	expectedOutputBuffer.str("");

	expectedOutputbuffer << expectedOutputFile.rdbuf();
	expectedOutputFile.close();

	p_threadManager->joinAll();
}

void RunPerformanceTest(std::string& path)
{
	std::string inputFilePath = path + "/tests/inputs/inputFile1.csv";
	std::string outputFilePath = path + "/tests/outputs/outputFilePerformance.csv";

	/* Create processing and publishing queues */
	Queue<std::string> orderBookProcessingQueue;
	Queue<std::string> publishingQueue;

	/* Create main class objects - OrderBookManager, ThreadManager, Publisher and IStreamClient */
	std::shared_ptr<OrderBookManager> p_orderBookManager(new OrderBookManager(orderBookProcessingQueue, publishingQueue));
	std::shared_ptr<Publisher> p_publisher(new Publisher(true, true, publishingQueue));
	std::shared_ptr<ThreadManager> p_threadManager(new ThreadManager());
	std::shared_ptr<IStreamClient> p_streamer(new FileStreamerImpl(inputFilePath, orderBookProcessingQueue));

	Logger::getLogger().setFilePath(outputFilePath);

	try
	{
		/* Create Input streamer and publisher threads */
		p_threadManager->createThread(p_streamer, &IStreamClient::TestPerformance);
		p_threadManager->createThread(p_publisher, &Publisher::processPublishingMessages);
		
		/* Process streams from client reader */
		p_orderBookManager->processStreamingTask();
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}

	p_streamer->Close();
	Logger::getLogger().closeLogFile();

	p_threadManager->joinAll();
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

TEST(PerformanceTests, PerformanceTestScenario1)
{
    RunPerformanceTest(filePath);
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "Please provide the path for input scenario files" << "\n";
		return -1;
	}

	filePath = argv[1];
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}