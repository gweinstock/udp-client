cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add the executable
add_library(udp-client STATIC udp-client.cpp udp-client-test/UdpSvc.cpp)

message(STATUS "BOOST_INCLUDE: ${BOOST_INCLUDE} BOOST_LIB: ${BOOST_LIB}")
message(STATUS "RAPIDJSON_INCLUDE: ${RAPIDJSON_INCLUDE}")

# set the project name
project(udp-client)

#find_library(BOOST_DATE_TIME NAME boost_date_time-vc142-mt-gd-x32-1_71 PATHS ${BOOST_LIB})
#find_library(BOOST_SYSTEM NAME boost_system-vc142-mt-gd-x32-1_71 PATHS ${BOOST_LIB})

message(STATUS "BOOST: [${BOOST_DATE_TIME}, ${BOOST_SYSTEM}]")

target_include_directories(udp-client PUBLIC ${BOOST_INCLUDE} ${RAPIDJSON_INCLUDE})

target_link_libraries(udp-client PUBLIC ${BOOST_DATE_TIME} ${BOOST_SYSTEM} pthread)