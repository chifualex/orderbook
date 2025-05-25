
#include "FileStreamerImpl.h"

#define TEST_PERFORAMNCE_INTERATIONS 	1000

#define NEW_BUY_ORDER(bidUserId, symbol, price, quantity, userOrderId) std::string("N, " + std::to_string(bidUserId) + ", " + symbol \
														+ ", " + std::to_string(price) + ", " + std::to_string(quantity) \
														+ ", B, " + std::to_string(userOrderId))

#define NEW_SELL_ORDER(askUserId, symbol, price, quantity, userOrderId) std::string("N, " + std::to_string(askUserId) + ", " + symbol \
														+ ", " + std::to_string(price) + ", " + std::to_string(quantity) \
														+ ", S, "  + std::to_string(userOrderId))

FileStreamerImpl::FileStreamerImpl(std::string const& filePath, Queue<std::string>& processingQueue) : m_processingQueue(processingQueue)
{
	m_filePath = filePath;
}

void FileStreamerImpl::Read()
{
	/* Read function - Read from beginning of the file and send line by line to queue*/
	std::string line = "";
	m_file.seekg(0, std::ios::beg);
	while (!m_file.eof())
	{
		std::getline(m_file, line);
		m_processingQueue.push(line);
	}

	/* Added for testing */
	m_processingQueue.push("Exit");
}

void FileStreamerImpl::TestPerformance()
{
	int count = 0;

	/* Test latency performance */
	while (count < TEST_PERFORAMNCE_INTERATIONS)
	{
        count++;

		m_processingQueue.push(NEW_BUY_ORDER(2, "IBM", 9, 100, 101));
		m_processingQueue.push(NEW_BUY_ORDER(2, "IBM", 9, 100, 101));
		m_processingQueue.push(NEW_BUY_ORDER(2, "IBM", 9, 100, 101));
		m_processingQueue.push(NEW_SELL_ORDER(2, "IBM", 11, 100, 102));
		m_processingQueue.push(NEW_SELL_ORDER(2, "IBM", 11, 100, 102));
		m_processingQueue.push(NEW_SELL_ORDER(2, "IBM", 11, 100, 102));
		m_processingQueue.push(NEW_BUY_ORDER(1, "IBM", 11, 100, 3));
		m_processingQueue.push(NEW_SELL_ORDER(2, "IBM", 10, 100, 103));
		m_processingQueue.push(NEW_BUY_ORDER(1, "IBM", 10, 100, 4));
		m_processingQueue.push(NEW_BUY_ORDER(1, "IBM", 6, 100, 4));
		m_processingQueue.push(NEW_BUY_ORDER(1, "IBM", 5, 100, 4));
		m_processingQueue.push(NEW_BUY_ORDER(1, "IBM", 7, 100, 4));
		m_processingQueue.push(NEW_SELL_ORDER(2, "IBM", 9, 100, 104));
		m_processingQueue.push(NEW_SELL_ORDER(2, "IBM", 7, 100, 104));
		m_processingQueue.push(NEW_SELL_ORDER(2, "IBM", 6, 100, 104));
		m_processingQueue.push(NEW_SELL_ORDER(2, "IBM", 5, 100, 104));
	}

	/* Added for testing */
	m_processingQueue.push("Exit");
}

void FileStreamerImpl::Open()
{
	/* File open */
	m_file.open(m_filePath, std::ios::in);
	if (!m_file)
	{
		std::cout << "File does not exists in the specified path" << std::endl;
	}
}

FileStreamerImpl::~FileStreamerImpl()
{
	Close();
}

void FileStreamerImpl::Close()
{
	m_file.close();
}
