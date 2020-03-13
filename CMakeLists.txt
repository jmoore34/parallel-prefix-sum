cmake_minimum_required(VERSION 3.15)
project(parallel_prefix_sum)

FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(my_app Threads::Threads)

set(CMAKE_CXX_STANDARD 14)

add_executable(parallel_prefix_sum main.cpp)