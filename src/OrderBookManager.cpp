#include <chrono>
#include "OrderBookManager.h"
#include <iostream>
#include <chrono>

OrderBookManager::OrderBookManager(Queue<std::string>& processingQueue, Queue<std::string>& publishingQueue) : m_processingQueue(processingQueue), m_publishingQueue(publishingQueue)
{
}

OrderBookManager::OrderBookManager(const OrderBookManager & other) : m_processingQueue(other.m_processingQueue), m_publishingQueue(other.m_publishingQueue)
{
}

void OrderBookManager::processStreamingTask()
{
	BookEntry bookEntry;

	/* Processing method */
	while (true)
	{
		/* Wait for available stream to process */
		std::string stream = m_processingQueue.pop();

		/* Added for testing */
		if (stream == "Exit")
		{
			m_publishingQueue.push("Exit");
			break;
		}

		/* Deserialize stream into the bookEntry object*/
		char result = StreamDeserializer::deserializeStream(stream, bookEntry);

		/* add or update orderBooks*/
		addOrUpdateOrderBook(result, bookEntry);
	}
}

void OrderBookManager::addOrUpdateOrderBook(char result, BookEntry& bookEntry)
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

void OrderBookManager::handleOrderBookOperations(char result, BookEntry& bookEntry)
{
	/* Get valid orderBook ptr */
	OrderBook* ptr = m_orderBookMap[bookEntry.getSymbol()];

	/* If valid handle the book commands */
	if (ptr)
	{
		if (result == 'F')
		{
			/* Reset book */
			ptr->resetBook();
		}

		if (result == 'N')
		{
			/* Update book */
			ptr->updateBook(bookEntry.getSymbol(),
				bookEntry.getPrice(),
				bookEntry.getQuantity(),
				bookEntry.getUserOrderId(),
				bookEntry.getUserId(),
				bookEntry.getSide());
		}

		if (result == 'C')
		{
			/* Cancel orders */
			ptr->cancelOrders(bookEntry.getUserOrderId(),
				bookEntry.getUserId());
		}

		if (result == 0)
		{
			//m_publishingQueue.push("Invalid stream request");
		}
	}
	else
	{
		std::cout << "Invalid OrderBook" << std::endl;
	}
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
