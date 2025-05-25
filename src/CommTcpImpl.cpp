#include "CommTcpImpl.h"
#include <chrono>

#define MAX_BUFFER_SIZE             65536
#define DEFAULT_PORT                5000
#define MAX_PENDING_CONNECTIONS     10
#define TEST_PERFORAMNCE_INTERATIONS    100000

#define NEW_BUY_ORDER(bidUserId, symbol, price, quantity, userOrderId) std::string("N, " + std::to_string(bidUserId) + ", " + std::to_string(symbol) \
                                                        + ", " + std::to_string(price) + ", " + std::to_string(quantity) \
                                                        + ", B," + std::to_string(userOrderId))

#define NEW_SELL_ORDER(askUserId, symbol, price, quantity, userOrderId) std::string("N, " + std::to_string(askUserId) + ", " + std::to_string(symbol) \
                                                        + ", " + std::to_string(price) + ", " + std::to_string(quantity) \
                                                        + ", S,"  + std::to_string(userOrderId))

CommTcpImpl::CommTcpImpl(Queue<std::string>& processingQueue) : m_processingQueue(processingQueue)
{
    m_isRunning = true;
    m_clientSock = -1;
    
    // Create a TCP socket
    if((m_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("socket creation error...\n");
        exit(-1);
    }

    // Set socket options to allow reuse of address
    int opt = 1;
    if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt error...\n");
        exit(-1);
    }
}

void CommTcpImpl::sendInputForProcessing(char *buffer)
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

void CommTcpImpl::Read()
{
    char* buffer = new char[MAX_BUFFER_SIZE];

    /* Read function task -> read from client with recv function and send it for processing to queue*/
    while (m_isRunning)
    {
        // Accept a client connection if we don't have one
        if (m_clientSock < 0)
        {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            m_clientSock = accept(m_sock, (struct sockaddr*)&client_addr, &client_len);
            if (m_clientSock < 0)
            {
                perror("Accept error...\n");
                continue;
            }
            std::cout << "New client connected" << std::endl;
        }

        // Read from the client
        int ret = recv(m_clientSock, buffer, MAX_BUFFER_SIZE - 1, 0);
        if (ret < 0)
        {
            perror("Receive function error...\n");
            close(m_clientSock);
            m_clientSock = -1;
            continue;
        }
        else if (ret == 0)
        {
            // Client disconnected
            std::cout << "Client disconnected" << std::endl;
            close(m_clientSock);
            m_clientSock = -1;
            continue;
        }

        /* make the buffer zero terminated */
        buffer[ret] = 0;

        /* Send buffer to processing queue*/
        sendInputForProcessing(buffer);
    }

    delete[] buffer;
}

void CommTcpImpl::TestPerformance()
{
    /* To be implemented */
}

void CommTcpImpl::Open()
{
    struct sockaddr_in add;
    add.sin_family = AF_INET;
    add.sin_addr.s_addr = htonl(INADDR_ANY);
    add.sin_port = htons(DEFAULT_PORT);

    /* sock bind */
    if (bind(m_sock, (struct sockaddr*)&add, sizeof(add)) < 0)
    {
        perror("Bind function error...\n");
        exit(-1);
    }

    /* start listening */
    if (listen(m_sock, MAX_PENDING_CONNECTIONS) < 0)
    {
        perror("Listen function error...\n");
        exit(-1);
    }

    std::cout << "TCP server listening on port " << DEFAULT_PORT << "..." << std::endl;
}

void CommTcpImpl::Close()
{
    /* close client connection if active */
    if (m_clientSock >= 0)
    {
        close(m_clientSock);
    }
    
    /* close server socket */
    close(m_sock);
}

CommTcpImpl::~CommTcpImpl()
{
    Close();
} 