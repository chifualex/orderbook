#ifndef FILESTREAMERCLIENTIMPL_H
#define FILESTREAMERCLIENTIMPL_H

#include "IStreamClient.h"
#include "Queue.h"
#include <fstream>

class FileStreamerImpl : public IStreamClient
{
public:
	FileStreamerImpl(std::string const& filePath, Queue<std::string>& processingQueue);

	/* Method used to read data from a communication channel */
	void read() override;
	
	/* Method used to open a communication channel */
	void open() override;
	
	/* Method used to close the communication channel */
	void close() override;
	
	/* Method used to get the typeof a communication channel */
	virtual StreamClientType getCommType() { return FILE; }

	~FileStreamerImpl() override;

private:
	Queue<std::string>& m_processingQueue;
	std::fstream m_file;
	std::string m_filePath;
};

#endif
