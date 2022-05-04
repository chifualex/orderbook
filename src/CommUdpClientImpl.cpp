#include "CommUdpClientImpl.h"

#define MAX_BUFFER_SIZE 65536

CommUdpClientImpl::CommUdpClientImpl(unsigned short port, Queue<std::string>& processingQueue) : m_processingQueue(processingQueue)
{
	m_isRunning = true;
	m_port = port;
	m_sockfd = 0;

	#ifdef __linux
	// Creating socket file descriptor
    if ( (m_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
	#else
	m_wsaSession = new WSASession();
	m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_sock == INVALID_SOCKET)
		throw std::system_error(WSAGetLastError(), std::system_category(), "Error opening socket");
	#endif
}

void CommUdpClientImpl::sendInputForProcessing(char *buffer)
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

void CommUdpClientImpl::read()
{
	char* buffer = new char[MAX_BUFFER_SIZE];
	int flags = 0;
	sockaddr_in from;
	#ifdef __linux
	unsigned int size = sizeof(from);
	#else
	int size = sizeof(from);
	#endif
	int ret = 0;

	/* Read function task -> read from server with recvfrom function and send it for processing to queue*/
	while (m_isRunning)
	{
		#ifdef __linux
		ret = recvfrom(m_sockfd, buffer, MAX_BUFFER_SIZE, 
					MSG_WAITALL, ( struct sockaddr *) &from,
					&size);
		if (ret < 0)
			throw std::system_error(0, std::system_category(), "recvfrom failed");
		#else
		ret = recvfrom(m_sock, buffer, MAX_BUFFER_SIZE, 0, reinterpret_cast<SOCKADDR *>(&from), &size);
		if (ret < 0)
			throw std::system_error(WSAGetLastError(), std::system_category(), "recvfrom failed");
		#endif

		/* make the buffer zero terminated */
		buffer[ret] = 0;

		/* Send buffer to processing queue*/
		sendInputForProcessing(buffer);
	}
}
    
void CommUdpClientImpl::open()
{
	#ifdef __linux
	struct sockaddr_in cliaddr, servaddr;
	memset(&cliaddr, 0, sizeof(cliaddr));
	memset(&servaddr, 0, sizeof(servaddr));
    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(m_port);
	// Bind the socket with the server address
    if ( bind(m_sockfd, (const struct sockaddr *)&servaddr, 
            sizeof(servaddr)) < 0 )
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
	#else
	sockaddr_in add;
	add.sin_family = AF_INET;
	add.sin_addr.s_addr = htonl(INADDR_ANY);
	add.sin_port = htons(m_port);
	/* sock bind */
	int ret = bind(m_sock, reinterpret_cast<SOCKADDR *>(&add), sizeof(add));
	if (ret < 0)
		throw std::system_error(WSAGetLastError(), std::system_category(), "Bind failed");
	#endif
}

/* Status of the task function */
void CommUdpClientImpl::setThreadStatus(bool isRunning)
{
	m_isRunning = isRunning;
}

CommUdpClientImpl::~CommUdpClientImpl()
{
	#ifdef _WIN32
	delete m_wsaSession;
	#endif
}

void CommUdpClientImpl::close()
{
	/* close connection */
	#ifdef _WIN32
	closesocket(m_sock);
	#endif
}
