file(GLOB_RECURSE cppSources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
set(OBJECT_LIST ${cppSources})

add_library(OrderBookLib ${OBJECT_LIST})
target_compile_options(OrderBookLib PUBLIC "SHELL: -m64 -D BIG_ENDIAN=1 -std=c++11  -D -Wall -D LITTLE_ENDIAN=0 -fno-strict-aliasing -D _REENTRANT -include -fPIC -D")

set(CMAKE_EXE_LINKER_FLAGS "-rdynamic")
target_include_directories(OrderBookLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../includes
)

target_include_directories(OrderBookLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../tests
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  ${CMAKE_BINARY_DIR}/orderbooklibConfigVersion.cmake
  VERSION 1.1
  COMPATIBILITY AnyNewerVersion)

export(TARGETS OrderBookLib FILE ${CMAKE_BINARY_DIR}/ORDERBOOKLIBConfig.cmake)
export(PACKAGE OrderBookLib)