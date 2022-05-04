#ifndef ORDERBOOK_MANAGER_HPP
#define ORDERBOOK_MANAGER_HPP

#include "BookEntry.h"
#include "OrderBook.h"
#include "Queue.h"
#include <unordered_map>

class OrderBookManager {

public:

	/* Constructor */
	OrderBookManager(Queue<std::string>& processingQueue, Queue<std::string>& publishingQueue);

	/* Copy constructor */
	OrderBookManager(const OrderBookManager& other);

	/* Processing task main method */
	void processStreamingTask();

	/* Update orderBook if existing, if not create a new orderBook */
	void addOrUpdateOrderBook(std::string& result, BookEntry& bookEntry);

	/* Handler book operations - add new order : N, cancel orders : C, reset book : F*/
	void handleOrderBookOperations(std::string& result, BookEntry& bookEntry);

	/* Method used to set thread running status */
	void setThreadStatus(bool isRunning);

	/* Destructor */
	~OrderBookManager();

private:
	/* Processing and publishing queues */
	Queue<std::string>& m_processingQueue;
	Queue<std::string>& m_publishingQueue;
	
	/* Map of <symbol, orderBook> - have a separate OrderBook for every symbol */
	std::unordered_map<std::string, OrderBook*> m_orderBookMap;
	bool m_isRunning;
};
#endif