cmake_minimum_required(VERSION 3.0)

project(interpretervisitor)

add_executable(interpretervisitor InterpreterVisitor.cpp)

install(TARGETS interpretervisitor RUNTIME DESTINATION bin)
