cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

project(mse)

if(WIN32)
	find_library(WSOCK32_LIBRARY wsock32)
	find_library(Ws2_32_LIBRARY ws2_32)
endif()

add_subdirectory(src)
