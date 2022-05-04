#include <chrono>
#include "OrderBookManager.h"
#include <iostream>

OrderBookManager::OrderBookManager(Queue<std::string>& processingQueue, Queue<std::string>& publishingQueue) : m_processingQueue(processingQueue), m_publishingQueue(publishingQueue)
{
	m_isRunning = true;
}

OrderBookManager::OrderBookManager(const OrderBookManager & other) : m_processingQueue(other.m_processingQueue), m_publishingQueue(other.m_publishingQueue)
{
}

void OrderBookManager::processStreamingTask()
{
	BookEntry bookEntry;

	//std::chrono::steady_clock::time_point start, end;

	/* Processing method */
	while (m_isRunning)
	{
		/* Wait for available stream to process */
		std::string stream = m_processingQueue.pop();

		//start = std::chrono::steady_clock::now();

		/* Added for testing */
		if (stream == "Exit")
		{
			m_isRunning = false;
		}

		/* Deserialize stream into the bookEntry object*/
		std::string result = StreamDeserializer::deserializeStream(stream, bookEntry);

		/* add or update orderBooks*/
		addOrUpdateOrderBook(result, bookEntry);

		//end = std::chrono::steady_clock::now();

		//std::cout << "Elapsed orderBook update time in microseconds: "
			//<< std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
			//<< " us" << std::endl;
	}
}

void OrderBookManager::addOrUpdateOrderBook(std::string& result, BookEntry& bookEntry)
{
	/* Check if OrderBook exists already */
	if (m_orderBookMap.find(bookEntry.getSymbol()) != m_orderBookMap.end()) 
	{
		/* Handle operations*/
		handleOrderBookOperations(result, bookEntry);
	}
	else
	{
		/* Create new OrderBook and handle operations*/
		m_orderBookMap[bookEntry.getSymbol()] = new OrderBook(m_publishingQueue);
		handleOrderBookOperations(result, bookEntry);
	}
}

void OrderBookManager::handleOrderBookOperations(std::string& result, BookEntry& bookEntry)
{
	/* Get valid orderBook ptr */
	OrderBook* ptr = m_orderBookMap[bookEntry.getSymbol()];

	/* If valid handle the book commands */
	if (ptr)
	{
		if (result == "F")
		{
			/* Reset book */
			ptr->resetBook();
		}

		if (result == "N")
		{
			/* Update book */
			ptr->updateBook(bookEntry.getSymbol(),
				bookEntry.getPrice(),
				bookEntry.getQuantity(),
				bookEntry.getUserOrderId(),
				bookEntry.getUserId(),
				bookEntry.getSide());
		}

		if (result == "C")
		{
			/* Cancel orders */
			ptr->cancelOrders(bookEntry.getUserOrderId(),
				bookEntry.getUserId());
		}
	}
	else
	{
		std::cout << "Invalid OrderBook" << std::endl;
	}
}

void OrderBookManager::setThreadStatus(bool isRunning)
{
	m_isRunning = isRunning;
}

OrderBookManager::~OrderBookManager()
{
	OrderBook* tmp = nullptr;
	for (std::unordered_map<std::string, OrderBook*>::iterator it = m_orderBookMap.begin(); it != m_orderBookMap.end(); it++)
	{
		tmp = it->second;
		delete tmp;
		tmp = nullptr;
	}

	m_orderBookMap.clear();
}
