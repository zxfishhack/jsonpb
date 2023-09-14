cmake_minimum_required(VERSION 3.8)
project(jsonpb)
include(FetchContent)
include(cmake/debug_func.cmake)

set(CMAKE_CXX_STANDARD 14)

FetchContent_Declare(
  protobuf
  URL https://assets.grgreye.com/libs/protobuf-3.19.2.zip
)
FetchContent_MakeAvailable(protobuf)
option(protobuf_BUILD_TESTS "Build tests" OFF)
set(protobuf_BUILD_TESTS OFF)
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
option(RAPIDJSON_BUILD_DOC "Build rapidjson documentation." OFF)
option(RAPIDJSON_BUILD_EXAMPLES "Build rapidjson examples." OFF)
option(RAPIDJSON_BUILD_TESTS "Build rapidjson perftests and unittests." OFF)
set(RAPIDJSON_BUILD_DOC OFF)
set(RAPIDJSON_BUILD_EXAMPLES OFF)
set(RAPIDJSON_BUILD_TESTS OFF)
FetchContent_MakeAvailable(rapidjson)
include_directories(${rapidjson_SOURCE_DIR}/include)

add_executable(tests ${all_tests_files} ${common_test_files} ${all_proto_files})
add_dependencies(tests libprotobuf)
target_link_libraries(tests gmock_main)
target_link_libraries(tests libprotobuf)

add_custom_target(check
  COMMAND tests --gtest_brief=1
  DEPENDS tests
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
