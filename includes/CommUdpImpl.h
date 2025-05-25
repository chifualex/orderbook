#ifndef COMMUDPIMPL_H
#define COMMUDPIMPL_H

#include "IStreamClient.h"
#include "Queue.h"
#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <iostream>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

/* Udp Class derived from Stream interface*/
class CommUdpImpl : public IStreamClient
{
public:
	/* Constructor */
	CommUdpImpl(Queue<std::string>& processingQueue);

	/* Method used to read data from a communication channel */
	void Read() override;
	
	/* Method used to test performance data from a communication channel */
	void TestPerformance() override;

	/* Method used to open a communication channel */
	void Open() override;
	
	/* Method used to close the communication channel */
	void Close() override;
	
	/* Method used to get the typeof a communication channel */
	virtual StreamClientType getCommType() { return UDP; }

	/* Method used to send received stream to processing queue */
	void sendInputForProcessing(char *buffer);

	/* Destructor */
	~CommUdpImpl() override;

private:
	int m_sock;
	Queue<std::string>& m_processingQueue;
	int m_lenght;
	bool m_isRunning;
};

#endif