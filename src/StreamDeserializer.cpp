#include "StreamDeserializer.h"
#include <algorithm>
#include <string>
#include <iostream>

StreamDeserializer::StreamDeserializer()
{

}

StreamDeserializer::~StreamDeserializer()
{

}

void StreamDeserializer::setBookEntry(int& index, std::string& param, BookEntry& bookEntry)
{
	switch (index)
	{
	/* Set bookEntry status */
	case 0:
		if (param == "N")
			bookEntry.setOrderBookStatus(OrderStatus::NEW_ORDER);
		else if (param == "C")
			bookEntry.setOrderBookStatus(OrderStatus::CANCEL_ORDER);
		break;
	/* Set userId */
	case 1:
		bookEntry.setUserId(std::atoi(param.c_str()));
		break;
	/* Set symbol or userOrderId*/
	case 2:
		if (bookEntry.getOrderStatus() == OrderStatus::NEW_ORDER)
		{
			bookEntry.setSymbol(param);
		}
		if (bookEntry.getOrderStatus() == OrderStatus::CANCEL_ORDER)
		{
			bookEntry.setUserOrderId(std::atoi(param.c_str()));
		}
		break;
	/* Set price */
	case 3:
		bookEntry.setPrice(std::atoi(param.c_str()));
		break;
	/* Set quantity */
	case 4:
		bookEntry.setQuantity(std::atoi(param.c_str()));
		break;
	/* Set side */
	case 5:
		bookEntry.setSide((param == "B") ? BookSide::BOOK_BID_SIDE : BookSide::BOOK_ASK_SIDE);
		break;
	/* Set userOrderId*/
	case 6:
		bookEntry.setUserOrderId(std::atoi(param.c_str()));
		break;
	default:
		break;
	}
}

char StreamDeserializer::deserializeStream(std::string& stream, BookEntry& bookEntry)
{
	/* Check for flush command */
	if (stream == "F" || stream == "F\r")
	{
		return 'F';
	}

	/* Validate new order*/
	if (validateStreamAndExtractParameters(stream, bookEntry, NEW_ORDER_FORMAT))
	{
		return 'N';
	}
	
	/* Validate cancel orders */
	if (validateStreamAndExtractParameters(stream, bookEntry, CANCEL_ORDER_FORMAT))
	{
		return 'C';
	}

	//std::cout << "Invalid stream request" << std::endl;
	return 'F';
}

bool StreamDeserializer::validateStreamAndExtractParameters(std::string& stream, BookEntry& bookEntry, std::regex validator)
{
	/* Check for a match */
	if (std::regex_match(stream, validator))
	{
		/* Remove whitespaces */
		std::string parameter;
		removeWhitespaces(stream);

		int i = 0;
		std::stringstream sstream(stream);

		/* Extract parameters separated by ',' */
		while (std::getline(sstream, parameter, ','))
		{
			/* Populate book Entry*/
			setBookEntry(i, parameter, bookEntry);
			++i;
		}

		return true;
	}
	else
	{
		return false;
	}
}

void StreamDeserializer::removeWhitespaces(std::string& stream)
{
	/* Remove whitespaces from string */
	stream.erase(std::remove_if(stream.begin(),
		stream.end(),
		[](unsigned char x) {return (x == ' '); }),
		stream.end());
}
