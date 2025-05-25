#ifndef COMMTCPIMPL_H
#define COMMTCPIMPL_H

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

class CommTcpImpl : public IStreamClient
{
private:
    int m_sock;
    int m_clientSock;
    bool m_isRunning;
    Queue<std::string>& m_processingQueue;

public:
    CommTcpImpl(Queue<std::string>& processingQueue);
    ~CommTcpImpl() override;
    
    	/* Method used to read data from a communication channel */
	void Read() override;
	
	/* Method used to test performance data from a communication channel */
	void TestPerformance() override;

	/* Method used to open a communication channel */
	void Open() override;
	
	/* Method used to close the communication channel */
	void Close() override;
	
	/* Method used to get the typeof a communication channel */
	virtual StreamClientType getCommType() { return TCP; }

	/* Method used to send received stream to processing queue */
	void sendInputForProcessing(char *buffer);
}; 

#endif