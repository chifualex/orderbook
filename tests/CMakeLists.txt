include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

set(TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp")

add_executable(OrderBookTests UnitTests.cpp)

target_include_directories(OrderBookTests PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../includes
)

set(ENABLE_EXPORTS ON)

target_link_libraries(OrderBookTests
 PRIVATE
  GTest::GTest
  OrderBookLib)

add_test(OrderBook_gtests OrderBookTests)