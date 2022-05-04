#include "OrderBook.h"
#include <iostream>
typedef std::multimap<int, BookEntry>::iterator mapIterator;

/* Reference the publishing queue to OrderBook*/
OrderBook::OrderBook(Queue<std::string>& publishingQueue) : m_publishingQueue(publishingQueue)
{
}

OrderBook::OrderBook(const OrderBook & other) : m_publishingQueue(other.m_publishingQueue)
{
}

OrderBook::~OrderBook()
{
}

/* Get best bid price - Get from map end for bids as it's ordered by price */
int OrderBook::getBestBidPrice()
{
	if (m_bids.size() > 0)
	{
		auto it = m_bids.rbegin();
		return it->second.getPrice();
	}
	else
	{
		return -1;
	}
}

/* Get best ask price - Get from map begin for asks as it's ordered by price */
int OrderBook::getBestAskPrice()
{
	if (m_asks.size() > 0)
	{
		auto it = m_asks.begin();
		return it->second.getPrice();
	}
	else
	{
		return -1;
	}
}

/* Cancel orders with userOrderId and userId*/
void OrderBook::cancelOrders(int userOrderId, int userId)
{
	for (auto it = m_bids.begin(); it != m_bids.end(); ) 
	{
		/* Check through map and cancel match orders*/
		if (it->second.getUserOrderId() == userOrderId &&
			it->second.getUserId() == userId)
		{
			/* Erase from map and publish ACK*/
			it = m_bids.erase(it);
			m_publishingQueue.push(ACKNOWLEDGEMENT_FORMAT(userId, userOrderId));
		}
		else
		{
			++it;
		}	
	}

	for (auto it = m_asks.begin(); it != m_asks.end(); )
	{
		/* Check through map and cancel match orders*/
		if (it->second.getUserOrderId() == userOrderId &&
			it->second.getUserId() == userId)
		{
			/* Erase from map and publish ACK*/
			it = m_asks.erase(it);
			m_publishingQueue.push(ACKNOWLEDGEMENT_FORMAT(userId, userOrderId));
		}
		else
		{
			++it;
		}
	}
}

/* Update book method */
void OrderBook::updateBook(std::string symbol, int price, int quantity, int userOrderId, int userId, BookSide side)
{
	if (side == BookSide::BOOK_BID_SIDE)
	{
		updateBookBids(symbol, price, quantity, userOrderId, userId);
	}
	else
	{
		updateBookAsks(symbol, price, quantity, userOrderId, userId);
	}
}

/* Publish top of book changes*/
void OrderBook::publishTopOfBookChanges(std::multimap<int, BookEntry>& map, BookSide side)
{
	int totalQuantity = 0;
	int newPrice = 0;
	std::string sideStr = (side == BookSide::BOOK_ASK_SIDE) ? "S" : "B";
	
	/* Check if bids or asks are not empty*/
	if (!map.empty())
	{
		/* Get bestAsk or bestBid price*/
		if (side == BookSide::BOOK_ASK_SIDE)
		{
			newPrice = map.begin()->second.getPrice();
		}
		else
		{
			newPrice = map.rbegin()->second.getPrice();
		}

		std::pair<mapIterator, mapIterator> result = map.equal_range(newPrice);

		/* Calculated total quantity at bestPrice */
		for (mapIterator it = result.first; it != result.second; it++)
		{
			totalQuantity += it->second.getQuantity();
		}

		/* Publish TOB with bestPrice and totalQuantity*/
		m_publishingQueue.push(TOP_OF_BOOK_FORMAT(sideStr, newPrice, totalQuantity));
	}
	else
	{
		/* Publish empty TOB*/
		m_publishingQueue.push(EMPTY_TOP_OF_BOOK_FORMAT(sideStr));
	}
}

void OrderBook::updateBookAsks(std::string symbol, int price, int quantity, int userOrderId, int userId)
{
	/* Check for matching orders or market orders*/
	if (!m_bids.empty() && ((getBestBidPrice() >= price) || (price == 0)))
	{
		/* Get Top of book */
		price = m_bids.rbegin()->second.getPrice();
		auto it = m_bids.find(price);

		/* Get Top of book params */
		int bidUserId = it->second.getUserId();
		int bidUserOrderId = it->second.getUserOrderId();

		/* Check if Top of book will be completely filled */
		if (it->second.getQuantity() <= quantity)
		{
			/* Left quantity*/
			int leftQty = quantity - it->second.getQuantity();

			it = m_bids.erase(it);

			/* Send ACK and Trade */
			m_publishingQueue.push(ACKNOWLEDGEMENT_FORMAT(userId, userOrderId));
			m_publishingQueue.push(BIDS_TRADE_FORMAT(bidUserId, bidUserOrderId, userId, userOrderId, price, quantity));

			/* Publish Top of book changes */
			publishTopOfBookChanges(m_bids, BookSide::BOOK_BID_SIDE);

			/* If partially fill proceed to further processing*/
			if (leftQty)
			{
				updateBookAsks(symbol, price, leftQty, userOrderId, userId);
			}
		}
		else
		{
			/* Publish Trade */
			m_publishingQueue.push(ACKNOWLEDGEMENT_FORMAT(userId, userOrderId));
			m_publishingQueue.push(BIDS_TRADE_FORMAT(bidUserId, bidUserOrderId, userId, userOrderId, price, quantity));

			/* Adjust Top of book quantity */
			it->second.setQuantity(it->second.getQuantity() - quantity);

			/* Publish Top of book changes in quantity */
			publishTopOfBookChanges(m_bids, BookSide::BOOK_BID_SIDE);
		}
	}
	else if (m_bids.empty() && (price == 0))
	{
		/* Market order Kill ACK*/
		m_publishingQueue.push(ACKNOWLEDGEMENT_FORMAT(userId, userOrderId));
	}
	else
	{
		/* ACK of new order*/
		m_publishingQueue.push(ACKNOWLEDGEMENT_FORMAT(userId, userOrderId));
		
		if (!m_asks.empty() && (price <= getBestAskPrice()))
		{
			/* Insert order and update Top of book */
			m_asks.insert({ price, BookEntry(symbol, price, quantity, userOrderId, userId, BookSide::BOOK_ASK_SIDE, OrderStatus::NEW_ORDER) });
			publishTopOfBookChanges(m_asks, BookSide::BOOK_ASK_SIDE);
		}
		else if (m_asks.empty())
		{
			/* Insert order and update Top of book */
			m_asks.insert({ price, BookEntry(symbol, price, quantity, userOrderId, userId, BookSide::BOOK_ASK_SIDE, OrderStatus::NEW_ORDER) });
			publishTopOfBookChanges(m_asks, BookSide::BOOK_ASK_SIDE);
		}
		else
		{
			/* Update Top of book */
			m_asks.insert({ price, BookEntry(symbol, price, quantity, userOrderId, userId, BookSide::BOOK_ASK_SIDE, OrderStatus::NEW_ORDER) });
		}
	}
}

void OrderBook::updateBookBids(std::string symbol, int price, int quantity, int userOrderId, int userId)
{
	/* Check for matching orders or market orders*/
	if (!m_asks.empty() && ((getBestAskPrice() <= price) || (price == 0)))
	{
		/* Get top of book */
		auto it = m_asks.begin();

		/* Get top of book params*/
		int askUserId = it->second.getUserId();
		int askUserOrderId = it->second.getUserOrderId();
		price = it->second.getPrice();

		/* Check if Top of book will be completely filled */
		if (it->second.getQuantity() <= quantity)
		{
			/* Left quantity*/
			int leftQty = quantity - it->second.getQuantity();

			/* Erase Top of book at the beginning of map*/
			it = m_asks.erase(it);

			/* Send ACK and Trade */
			m_publishingQueue.push(ACKNOWLEDGEMENT_FORMAT(userId, userOrderId));
			m_publishingQueue.push(ASK_TRADE_FORMAT(userId, userOrderId, askUserId, askUserOrderId, price, quantity));

			publishTopOfBookChanges(m_asks, BookSide::BOOK_ASK_SIDE);

			/* If partially fill proceed to further processing*/
			if (leftQty)
			{
				updateBookBids(symbol, price, leftQty, userOrderId, userId);
			}
		}
		else
		{
			/* Publish Trade */
			m_publishingQueue.push(ACKNOWLEDGEMENT_FORMAT(userId, userOrderId));
			m_publishingQueue.push(ASK_TRADE_FORMAT(userId, userOrderId, askUserId, askUserOrderId, price, quantity));

			/* Adjust Top of book quantity */
			it->second.setQuantity(it->second.getQuantity() - quantity);

			/* Publish Top of book changes in quantity */
			publishTopOfBookChanges(m_asks, BookSide::BOOK_ASK_SIDE);
		}
	}
	else if (m_asks.empty() && (price == 0))
	{
		/* Market order Kill ACK*/
		m_publishingQueue.push(ACKNOWLEDGEMENT_FORMAT(userId, userOrderId));
	}
	else
	{
		/* ACK of new order*/
		m_publishingQueue.push(ACKNOWLEDGEMENT_FORMAT(userId, userOrderId));
		
		if (!m_bids.empty() && (price >= getBestBidPrice()))
		{
			/* Insert order and update Top of book */
			m_bids.insert({ price, BookEntry(symbol, price, quantity, userOrderId, userId, BookSide::BOOK_BID_SIDE, OrderStatus::NEW_ORDER) });
			publishTopOfBookChanges(m_bids, BookSide::BOOK_BID_SIDE);
		}
		else if (m_bids.empty())
		{
			/* Insert order and update Top of book */
			m_bids.insert({ price, BookEntry(symbol, price, quantity, userOrderId, userId, BookSide::BOOK_BID_SIDE, OrderStatus::NEW_ORDER) });
			publishTopOfBookChanges(m_bids, BookSide::BOOK_BID_SIDE);
		}
		else
		{
			/* Update Top of book */
			m_bids.insert({ price, BookEntry(symbol, price, quantity, userOrderId, userId, BookSide::BOOK_BID_SIDE, OrderStatus::NEW_ORDER) });
		}
	}
}

void OrderBook::resetBook()
{
	/* Clear the maps */
	m_bids.clear();
	m_asks.clear();
}
