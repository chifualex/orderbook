# orderbook
Build and run the project : 

mkdir build && cd build
cmake ..
cmake --build .

For release build :

cmake --build . --target ALL_BUILD --config Release

Dependencies : g++ (Rev10, Built by MSYS2 project) 11.2.0 or higher on Windows
			   g++ 6.3.1 20170216 (Red Hat 6.3.1-3) on Linux

Run the exe :

./OrderBookApp.exe <inputFilePath>

Run the UnitTests :

./OrderBookUnitTests.exe <projectPath>

Future Improvements :

Short term improvements if I had more time 

- More unit tests
- Improve communication and synchronization using boost asio library
- Improve Streamer module and make it more generic, add protobuf
- Improve performance speed on critical paths
- Improve/Change data structure to support large number of orders 
- Consider switching to lists or vectors as they are more hardware/cache friendly and more efficient for topOfBook changes (in case of lists)

Longer term improvements :

- Add interfaces, extend and implement more OrderBook, BookEntry, Communication types - UDP/TCP/gRPC, Stop-limit, stop-loss orders, quotes, etc.
- Encrypt communication with SSL certificates
- Add a Gateway module to support multiple clients
- Order storage module
- Audit trail module to store transactions

Tests :

- Google test unit testing for the available scenarios

