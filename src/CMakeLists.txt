file(GLOB_RECURSE cppSources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
set(OBJECT_LIST ${cppSources})

add_library(OrderBookLib ${OBJECT_LIST})

target_include_directories(OrderBookLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../includes
)

target_include_directories(OrderBookLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../tests
)