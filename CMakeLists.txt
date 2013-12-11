cmake_minimum_required(VERSION 2.6)
project(GAL_MaximalIndependetSets)

# find boost
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS graph program_options REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -Wno-deprecated")
if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif (APPLE)

link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

FILE(GLOB Sources src/*.cpp)

add_executable(gal ${Sources})
target_link_libraries(gal ${Boost_LIBRARIES})

