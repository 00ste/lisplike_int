cmake_minimum_required(VERSION 3.0)

project(simpleinterpreter2)

add_executable(simpleinterpreter2 SimpleInterpreter.cpp)

install(TARGETS simpleinterpreter2 RUNTIME DESTINATION bin)
