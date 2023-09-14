cmake_minimum_required(VERSION 3.8)
project(jsonpb)
include(FetchContent)
include(cmake/debug_func.cmake)

set(CMAKE_CXX_STANDARD 14)

set(protobuf_BUILD_TESTS OFF)
FetchContent_Declare(
  protobuf
  URL https://assets.grgreye.com/libs/protobuf-3.19.2.zip
)
FetchContent_MakeAvailable(protobuf)
add_subdirectory(${protobuf_SOURCE_DIR}/cmake)
include_directories(${protobuf_SOURCE_DIR}/src)

include_directories(${PROJECT_SOURCE_DIR}/proto)
file(GLOB_RECURSE all_tests_files FOLLOW_SYMLINKS tests/*.cpp)
file(GLOB_RECURSE all_benches_files FOLLOW_SYMLINKS bench/*.cpp)
file(GLOB_RECURSE all_proto_files FOLLOW_SYMLINKS proto/*.cc)

FetchContent_Declare(
  gtest
  URL https://assets.grgreye.com/googletest-release
)
FetchContent_MakeAvailable(gtest)

include_directories(${gtest_SOURCE_DIR}/googlemock/include)
include_directories(${gtest_SOURCE_DIR}/googletest/include)

FetchContent_Declare(
  rapidjson
  URL https://assets.grgreye.com/libs/rapidjson-1.1.0.zip
)
FetchContent_MakeAvailable(rapidjson)
include_directories(${rapidjson_SOURCE_DIR}/include)

add_executable(tests ${all_tests_files} ${common_test_files} ${all_proto_files})
add_dependencies(tests libprotobuf)
target_link_libraries(tests gmock_main)
target_link_libraries(tests libprotobuf)
