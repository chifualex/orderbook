# orderbook
Build and run the project : 

extract the zip file - you should get the orderbook folder
run the following commands :

docker pull ubuntu:23.10
docker run -ti -v $PWD:/TestOrderbookAssignment ubuntu:23.10
cd orderbook
apt-get update
apt-get install build-essential cmake libssl-dev python3 git-all

mkdir build && cd build
cmake ..
cmake --build .

Dependencies : CMake version 3.23 or higher - 3.29.1 used
			   gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0 used

Run the exe :

./OrderBookApp UDP
-> send data with : cat inputfile.csv | netcat -u 127.0.0.1 5000

./OrderBookApp FILE inputFilePath

Run the UnitTests :

./OrderBookUnitTests projectPath

Tests :

- Google test unit testing for the available scenarios

Future Improvements :

Short term improvements given more time

- More unit tests
- Improve communication and synchronization using boost asio library
- Improve Streamer module and make it more generic, add protobuf
- Improve performance speed on critical paths - remove regex validations (already attempted and improved 10x the speed on some paths - however some tests were affected and I had to abort due to time constrain)
- Improve/Change data structure to support large number of orders 
- Consider switching to lists or vectors as they are more hardware/cache friendly and more efficient for topOfBook changes (in case of lists)

Longer term improvements :

- Add interfaces, extend and implement more OrderBook, BookEntry, Communication types - TCP/gRPC, Stop-limit, stop-loss orders, quotes, etc.
- Encrypt communication with SSL certificates
- Add a Gateway module to support multiple clients
- Order storage module
- Audit trail module to store transactions
- Think of ways to protect against "attacks" - for exampple : mass order influx targeted to meant to crash the application



