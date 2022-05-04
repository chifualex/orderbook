#ifndef COMMUDPCLIENTIMPL_H
#define COMMUDPCLIENTIMPL_H

#include "IStreamClient.h"
#include <system_error>
#include "Queue.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>

#ifdef __linux
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <netinet/in.h>
#else
  #include <WinSock2.h>
  #include <Ws2tcpip.h>
  #include <Windows.h>
  #pragma comment(lib, "Ws2_32.lib")
#endif

#ifdef _WIN32
class WSASession
{
public:
	WSASession()
	{
		int ret = WSAStartup(MAKEWORD(2, 2), &m_data);
		if (ret != 0)
			throw std::system_error(WSAGetLastError(), std::system_category(), "WSAStartup Failed");
	}
	~WSASession()
	{
		WSACleanup();
	}
private:
	WSAData m_data;
};
#endif

/* Udp Class derived from Stream interface*/
class CommUdpClientImpl : public IStreamClient
{
public:
	/* Constructor */
	CommUdpClientImpl(unsigned short port, Queue<std::string>& processingQueue);

	/* Method used to read data from a communication channel */
	void read() override;
	
	/* Method used to open a communication channel */
	void open() override;
	
	/* Method used to close the communication channel */
	void close() override;

	/* Method used to set thread running status */
	void setThreadStatus(bool isRunning) override;
	
	/* Method used to get the typeof a communication channel */
	virtual StreamClientType getCommType() { return UDP; }

	/* Method used to send received stream to processing queue */
	void sendInputForProcessing(char *buffer);

	/* Destructor */
	~CommUdpClientImpl() override;

private:
	int m_sockfd;
	#ifdef _WIN32
	WSASession* m_wsaSession;
	SOCKET m_sock;
	#endif
	Queue<std::string>& m_processingQueue;
	unsigned short m_port;
	int m_lenght;
	bool m_isRunning;
};

#endif