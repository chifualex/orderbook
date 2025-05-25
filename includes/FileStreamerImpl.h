#ifndef FILESTREAMERIMPL_H
#define FILESTREAMERIMPL_H

#include "IStreamClient.h"
#include "Queue.h"
#include <fstream>

class FileStreamerImpl : public IStreamClient
{
public:
	FileStreamerImpl(std::string const& filePath, Queue<std::string>& processingQueue);

	/* Method used to read data from a communication channel */
	void Read() override;
	
	/* Method used to test performance data from a communication channel */
	void TestPerformance() override;

	/* Method used to open a communication channel */
	void Open() override;
	
	/* Method used to close the communication channel */
	void Close() override;
	
	/* Method used to get the typeof a communication channel */
	virtual StreamClientType getCommType() { return FILE; }

	~FileStreamerImpl() override;

private:
	Queue<std::string>& m_processingQueue;
	std::fstream m_file;
	std::string m_filePath;
};

#endif
