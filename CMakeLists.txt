# References used:
# https://stackoverflow.com/questions/1620918/cmake-and-libpthread
# https://stackoverflow.com/questions/3897839/how-to-link-c-program-with-boost-using-cmake

# Basic setup
cmake_minimum_required(VERSION 3.11)
project(parallel_prefix_sum)

# Packages
# Thread package
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
#Boost package
FIND_PACKAGE( Boost 1.53 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )


set(CMAKE_CXX_STANDARD 11)

add_executable(parallel_prefix_sum main.cpp)

# Linking
link_directories(${Boost_LIBRARY_DIRS})
TARGET_LINK_LIBRARIES( parallel_prefix_sum LINK_PUBLIC Threads::Threads ${Boost_LIBRARIES}  )

