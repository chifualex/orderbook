# orderbook
Build and run the project : 

Simply run the make command : make

Dependencies : g++ (Rev10, Built by MSYS2 project) 11.2.0 or higher on Windows
			   g++ 6.3.1 20170216 (Red Hat 6.3.1-3) on Linux

The build should be supported on Linux also but I didn't have a proper environment to test the functionality. Only the build.
I am not sure if the UDP module works as I couldn't run netcat commands on my work environment.

Run the exe :

./OrderBookApplication.exe 
-> This will be default run on UDP port 5000

./OrderBookApplication.exe UDP <port>
./OrderBookApplication.exe File <fullFilePath>
./OrderBookApplication.exe RunUnitTests <projectPath>
-> Input and output folders with the .csv files are present in the directory
-> The test compares the expectedOutput files content with the new generated output and see if they match
./OrderBookApplication.exe RunPerformanceTest <projectPath>
-> PerformanceFile will be created with 40000 order operations (10000 bids and asks and then trades to empty the orderbook)
-> 

For the UDP type the application listen for data, on the File type in executes right away


Future Improvements :

Short term improvements if I had more time 

- CMake project type
- More unit tests
- Improve communication and synchronization using boost asio library
- Improve Streamer module and make it more generic, add protobuf
- Improve performance speed on critical paths
- Improve/Change data structure to support large number of orders 
- Consider switching to lists or vectors as they are more hardware/cache friendly and more efficient for topOfBook changes (in case of lists)

Longer term improvements :
- Add interfaces, extend and implement more OrderBook, BookEntry, Communication types - TCP/gRPC, Stop-limit, stop-loss orders, quotes, etc.
- Encrypt communication with SSL certificates
- Add a Gateway module to support multiple clients
- Order storage module
- Audit trail module to store transactions

Tests :

- I managed to test the application on Windows quite well using the https://packetsender.com/ tool
- Unfortunately I could not automate the unit tests with google tests or some other unit testing tool in time

Performance Tests :

- I had setup a test with 40000 orders transactions from a file which took around 4-5 seconds to finish (whole program)
- Some minor performance tests were performed in term of measurements of the critical path :
- From UDP/File receive function to orderbook updates. Typically the speed was between 15 - 100 us for any particular updates.
- The tests were run on a Intel(R) Core(TM) i5-4300M CPU @ 2.60GHz, so a pretty patetic hardware setup :)

Important NOTE here : 

The above values where achieved using Visual Studio Release build
I turned on the optimizations in the Makefile as well but the performance for some reason is really slow compared to Visual Studio (10-20x slower)
I might have forgot some optimization flags but still it's strange, maybe the difference in g++ versions and libs.

Autor's thoughts :

- Happy about the communication and stability between threads, everything runs smooth and is published correctly. I am really happy about that.
- Sad about the testing side, too much time was spent on development as I wanted to have something done and not enough time on tests.
- Kind of happy about the datastructure choice and performances (with proper optimizations), could be a lot better obviously.

