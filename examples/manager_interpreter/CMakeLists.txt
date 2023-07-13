cmake_minimum_required(VERSION 3.0)

project(manager_interpreter)

add_executable(manager_interpreter ExpressionInterpreter.cpp)

install(TARGETS manager_interpreter RUNTIME DESTINATION bin)
