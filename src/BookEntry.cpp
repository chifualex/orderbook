#include "BookEntry.h"

BookEntry::BookEntry()
	: m_symbol("")
	, m_price(0.0)
	, m_quantity(0)
	, m_userId(0)
	, m_userOrderId(0)
	, m_side(BookSide::BOOK_BID_SIDE)
	, m_orderStatus(OrderStatus::NEW_ORDER)
{

}

BookEntry::BookEntry(const BookEntry& other)
	: m_symbol(other.m_symbol)
	, m_price(other.m_price)
	, m_quantity(other.m_quantity)
	, m_userId(other.m_userId)
	, m_userOrderId(other.m_userOrderId)
	, m_side(other.m_side)
	, m_orderStatus(other.m_orderStatus)
{

}

BookEntry& BookEntry::operator=(const BookEntry& other)
{
	m_symbol = other.m_symbol;
	m_price = other.m_price;
	m_quantity = other.m_quantity;
	m_userOrderId = other.m_userOrderId;
	m_userId = other.m_userId;
	m_side = other.m_side;
	m_orderStatus = other.m_orderStatus;

	return *this;
}

bool BookEntry::operator==(const BookEntry& other)
{
	return (m_symbol == other.m_symbol &&
		m_quantity == other.m_quantity &&
		m_userOrderId == other.m_userOrderId &&
		m_userId == other.m_userId &&
		m_side == other.m_side &&
		m_orderStatus == other.m_orderStatus);
}

BookEntry::BookEntry(std::string symbol, int price, int quantity, int userOrderId, int userId, BookSide side, OrderStatus status)
	: m_symbol(symbol), m_price(price), m_quantity(quantity), m_userOrderId(userOrderId), m_userId(userId), m_side(side), m_orderStatus(status)
{
	
}

BookEntry::~BookEntry()
{
}

void BookEntry::update(std::string symbol, int price, int quantity, int userOrderId, int userId, BookSide side, OrderStatus status)
{
	m_symbol = symbol;
	m_price = price;
	m_quantity = quantity;
	m_userOrderId = userOrderId;
	m_userId = userId;
	m_side = side;
	m_orderStatus = status;
}

void BookEntry::setSymbol(std::string symbol)
{
	m_symbol = symbol;
}

void BookEntry::setPrice(int price)
{
	m_price = price;
}

void BookEntry::setQuantity(int qty)
{
	m_quantity = qty;
}

void BookEntry::setUserOrderId(int userOrderId)
{
	m_userOrderId = userOrderId;
}

void BookEntry::setUserId(int userId)
{
	m_userId = userId;
}

void BookEntry::setSide(BookSide side)
{
	m_side = side;
}

void BookEntry::setOrderBookStatus(OrderStatus status)
{
	m_orderStatus = status;
}

std::string BookEntry::getSymbol() const
{
	return m_symbol;
}

double BookEntry::getPrice() const
{
	return m_price;
}

int BookEntry::getQuantity() const
{
	return m_quantity;
}

int BookEntry::getUserOrderId() const
{
	return m_userOrderId;
}

int BookEntry::getUserId() const
{
	return m_userId;
}

BookSide BookEntry::getSide() const
{
	return m_side;
}

OrderStatus BookEntry::getOrderStatus() const
{
	return m_orderStatus;
}
