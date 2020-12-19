cmake_minimum_required(VERSION 3.5)
project(Prog_pl)

set(CMAKE_CXX_STANDARD 20)
find_package(Boost REQUIRED) 
target_link_libraries(Prog_pl ${Boost_LIBRARIES})
add_executable(Prog_pl main.cpp)






