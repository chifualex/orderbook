#ifndef STREAMDESERIALIZER_HPP
#define STREAMDESERIALIZER_HPP

#include "BookEntry.h"
#include <regex>
#include <sstream>

/* Regex defines */
#define NEW_ORDER_FORMAT std::regex("(N, )([0-9]+)(, )([a-zA-Z]+)(, )([0-9]+)(, )([0-9]+)(, )(B|S)(, )([0-9]+)(\r*)")
#define CANCEL_ORDER_FORMAT std::regex("(C,)( )([0-9]+),( )([0-9]+)(\r*)")

/* ACK, Trade, Top of book string format defines */
#define ACKNOWLEDGEMENT_FORMAT(userId, userOrderId) std::string("A, " + std::to_string(userId) + ", " + std::to_string(userOrderId))
#define ASK_TRADE_FORMAT(userId, userOrderId, askUserId, askUserOrderId, price, quantity) std::string("T, " + std::to_string(userId) + ", " + std::to_string(userOrderId) \
														+ ", " + std::to_string(askUserId) + ", " + std::to_string(askUserOrderId) \
														+ ", " + std::to_string(price) + ", " + std::to_string(quantity))
#define BIDS_TRADE_FORMAT(bidUserId, bidUserOrderId, userId, userOrderId, price, quantity) std::string("T, " + std::to_string(bidUserId) + ", " + std::to_string(bidUserOrderId) \
														+ ", " + std::to_string(userId) + ", " + std::to_string(userOrderId) \
														+ ", " + std::to_string(price) + ", " + std::to_string(quantity))
#define EMPTY_TOP_OF_BOOK_FORMAT(sideStr) std::string("B, " + sideStr + ", -, -")
#define TOP_OF_BOOK_FORMAT(sideStr, newPrice, totalQuantity) std::string("B, " + sideStr + ", " + std::to_string(newPrice) + ", " + std::to_string(totalQuantity))

class StreamDeserializer
{
public:

	/* Constructor and Destructor */
	StreamDeserializer();
	~StreamDeserializer();

	/* Remove whitespaces from string */
	static void removeWhitespaces(std::string& stream);

	/* Validate stream by regex validator and extract BookEntry parameters */
	static bool validateStreamAndExtractParameters(std::string& stream, BookEntry& bookEntry, std::regex validator);
	
	/* Set BookEntry parameters*/
	static void setBookEntry(int& index, std::string& param, BookEntry& bookEntry);
	
	/* Deserialize Stream method */
	static char deserializeStream(std::string& stream, BookEntry& bookEntry);

private:
};

#endif
