#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include "BookEntry.h"
#include "Queue.h"
#include "StreamDeserializer.h"
#include <map>

class OrderBook {

public:
	/* Constructor */
	OrderBook(Queue<std::string>& publishingQueue);

	/* Copy constructor */
	OrderBook(const OrderBook& other);

	/* Destructor */
	~OrderBook();

	/* Method to publish changes in TOB*/
	void publishTopOfBookChanges(std::multimap<int, BookEntry>& map, BookSide side);

	/* Update book */
	void updateBook(std::string symbol, int price, int quantity, int userOrderId, int userId, BookSide side);

	/* Update Book Asks	*/
	void updateBookAsks(std::string symbol, int price, int quantity, int userOrderId, int userId);

	/* Update Book Bids	*/
	void updateBookBids(std::string symbol, int price, int quantity, int userOrderId, int userId);

	/* Cancel orders */
	void cancelOrders(int userOrderId, int userId);

	/* Reset orderbook */
	void resetBook();

	/* Get best ask Price */
	int getBestAskPrice();

	/* Get best bid Price */
	int getBestBidPrice();

private:
	/* Bids and asks are each a multimap of <price, BookEntry>*/
	std::multimap<int, BookEntry> m_bids;
	std::multimap<int, BookEntry> m_asks;
	Queue<std::string>& m_publishingQueue;
}; // class OrderBook

#endif
