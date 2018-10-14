cmake_minimum_required(VERSION 2.8)
project(MX)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
set(EXECUTABLE_OUTPUT_PATH "../bin")

add_executable(MX
        include/Callable.hh
        include/Decl.hh
        include/Env.hh
        include/Expr.hh
        include/Interpreter.hh
        include/Keywords.hh
        include/Lexer.hh
        include/Library.hh
        include/Parser.hh
        include/Return.hh
        include/SemanticException.hh
        include/Stmt.hh
        include/SyntaxException.hh
        include/Token.hh
        include/Type.hh
        include/TypeException.hh
        include/Var.hh
        include/Visitor.hh
        src/Callable.cc
        src/Interpreter.cc
        src/Lexer.cc
        src/main.cc
        src/Parser.cc
        src/Var.cc)
