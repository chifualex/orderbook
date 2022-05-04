#ifndef BOOK_ENTRY_HPP
#define BOOK_ENTRY_HPP

#include <string>

enum BookSide
{
	BOOK_BID_SIDE = 'B',
	BOOK_ASK_SIDE = 'S'
};

enum OrderStatus
{
	NEW_ORDER = 1,
	CANCEL_ORDER = 2
};

// Book Entry class -> Contains the Order fields
class BookEntry {
public:

	/* Constructor */
	BookEntry();

	/* Copy constructor */
	BookEntry(const BookEntry& other);

	/* Operator == overload */
	BookEntry& operator=(const BookEntry& other);

	/* Operator == overload */
	bool operator==(const BookEntry& other);

	/* Parameterized constructor */
	BookEntry(std::string symbol, int price, int quantity, int userOrderId, int userId, BookSide side, OrderStatus status);

	/* Destructor */
	~BookEntry();

	void update(std::string symbol, int price, int quantity, int userOrderId, int userId, BookSide side, OrderStatus status);

	/* Method used to set the symbol */
	void setSymbol(std::string symbol);

	/* Method used to set the price */
	void setPrice(int price);

	/* Method used to set the quantity */
	void setQuantity(int qty);

	/* Method used to set the userOrderId */
	void setUserOrderId(int userOrderId);

	/* Method used to set the userId */
	void setUserId(int userId);

	/* Method used to set the side */
	void setSide(BookSide side);

	/* Method used to set the orderStatus */
	void setOrderBookStatus(OrderStatus status);

	/* Method used to get the symbol */
	std::string getSymbol() const;

	/* Method used to get the price */
	double getPrice() const;

	/* Method used to get the quantity */
	int getQuantity() const;

	/* Method used to get the userOrderId */
	int getUserOrderId() const;

	/* Method used to get the userId */
	int getUserId() const;

	/* Method used to get the side */
	BookSide getSide() const;

	/* Method used to get the orderStatus */
	OrderStatus getOrderStatus() const;

private:
	std::string m_symbol;
	int m_price;
	int m_quantity;
	int m_userId;
	int m_userOrderId;
	BookSide m_side;
	OrderStatus  m_orderStatus;
};

#endif
