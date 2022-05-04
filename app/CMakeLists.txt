file(GLOB cppSources "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
set(OBJECT_LIST ${cppSources})

add_executable(OrderBookApp ${OBJECT_LIST})

target_link_libraries(OrderBookApp OrderBookLib) 