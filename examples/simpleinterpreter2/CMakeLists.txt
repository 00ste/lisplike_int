cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 17)

project(simpleinterpreter2)

add_executable(simpleinterpreter2 SimpleInterpreter.cpp)

install(TARGETS simpleinterpreter2 RUNTIME DESTINATION bin)
