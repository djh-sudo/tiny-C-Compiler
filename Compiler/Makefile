
# make
TOOL_PATH = ../make_tool/

MAKE      = $(TOOL_PATH) make.exe -r
SRC       = main.cpp Generator.cpp Lexer.cpp Parser.cpp Semantic.cpp
SRC_H     = common.h Generator.h Lexer.h Parser.h Semantic.h
FLAGS     = -g -Wno-deprecated -std=c++11
# -g 编译产生调试信息
# -Wno-deprecated 忽略头文件依赖的警告
# -std=c++11 使用C++11
# compile
cc: $(SRC) $(SRC_H) Makefile
	g++ $(SRC) -o compile.exe $(FLAGS)



