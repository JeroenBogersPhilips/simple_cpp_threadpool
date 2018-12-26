cmake_minimum_required(VERSION 3.10)
project(asio)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.66.0 REQUIRED system)

include_directories(/Users/jeroen/googletest/googletest/include)
include_directories(/Users/jeroen/googletest/googlemock/include)

link_libraries(/Users/jeroen/googletest/build/googlemock/libgmock_main.a)
link_libraries(/Users/jeroen/googletest/build/googlemock/libgmock.a)
link_libraries(/Users/jeroen/googletest/build/googlemock/gtest/libgtest.a)
link_libraries(/Users/jeroen/googletest/build/googlemock/gtest/libgtest_main.a)

add_executable(asio TestThreadSafeQueue.cpp TestThreadPool.cpp)

target_include_directories(asio PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(asio ${Boost_LIBRARIES})