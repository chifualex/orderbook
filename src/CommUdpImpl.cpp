
#include "CommUdpImpl.h"
#include <chrono>

#define MAX_BUFFER_SIZE 				65536
#define DEFAULT_PORT					5000
#define TEST_PERFORAMNCE_INTERATIONS 	100000

#define NEW_BUY_ORDER(bidUserId, symbol, price, quantity, userOrderId) std::string("N, " + std::to_string(bidUserId) + ", " + std::to_string(symbol) \
														+ ", " + std::to_string(price) + ", " + std::to_string(quantity) \
														+ ", B," + std::to_string(userOrderId))

#define NEW_SELL_ORDER(askUserId, symbol, price, quantity, userOrderId) std::string("N, " + std::to_string(askUserId) + ", " + std::to_string(symbol) \
														+ ", " + std::to_string(price) + ", " + std::to_string(quantity) \
														+ ", S,"  + std::to_string(userOrderId))

CommUdpImpl::CommUdpImpl(Queue<std::string>& processingQueue) : m_processingQueue(processingQueue)
{
	m_isRunning = true;
	
    // create a socket
    if((m_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("socket creation error...\n");
        exit(-1);
    }
}

void CommUdpImpl::sendInputForProcessing(char *buffer)
{
	std::stringstream ss(buffer);
	std::string line;

	/* check input buffer */
	if (buffer != NULL)
	{
		/* send line by line in the queue to avoid delays*/
		while (std::getline(ss, line, '\n')) {
			m_processingQueue.push(line);
		}
	}
}

void CommUdpImpl::Read()
{
	char* buffer = new char[MAX_BUFFER_SIZE];
	int flags = 0;
	struct sockaddr_in from;
	socklen_t size = sizeof(from);

	/* Read function task -> read from server with recvfrom function and send it for processing to queue*/
	while (m_isRunning)
	{
        int ret = recvfrom(m_sock, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)& from, &size);
		if (ret < 0)
        {
            perror("Receive from function error error...\n");
            exit(-1);
        }

		/* make the buffer zero terminated */
		buffer[ret] = 0;

		/* Send buffer to processing queue*/
		sendInputForProcessing(buffer);
	}
}

void CommUdpImpl::TestPerformance()
{
	/* To be implemented */
}

void CommUdpImpl::Open()
{
	sockaddr_in add;
	add.sin_family = AF_INET;
	add.sin_addr.s_addr = htonl(INADDR_ANY);
	add.sin_port = htons(DEFAULT_PORT);

	/* sock bind */
    int ret = bind(m_sock, (struct sockaddr*)&add, sizeof(add));
	if (ret < 0)
    {
        perror("Bind function error...\n");
        exit(-1);
    }

	std::cout << "UDP channel open, waiting for data..." << "\n";
}

void CommUdpImpl::Close()
{
	/* close connection */
	close(m_sock);
}

CommUdpImpl::~CommUdpImpl()
{
	Close();
}
