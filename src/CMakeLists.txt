add_library(OrderBookLib "${CMAKE_CURRENT_SOURCE_DIR}/BookEntry.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/CommUdpImpl.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/CommTcpImpl.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/FileStreamerImpl.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/OrderBook.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/OrderBookManager.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/Publisher.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/StreamDeserializer.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/ThreadManager.cpp")

target_include_directories(OrderBookLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../includes
)

target_include_directories(OrderBookLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../tests
)