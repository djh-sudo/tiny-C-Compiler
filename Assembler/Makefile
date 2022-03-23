
# make
TOOL_PATH = ../make_tool/

MAKE      = $(TOOL_PATH) make.exe -r
SRC       = main.cpp Generate.cpp Lexer.cpp Parser.cpp Semantic.cpp Elf_File.cpp
SRC_H     = Common.h Generate.h Lexer.h Parser.h Semantic.h elf.h Elf_File.h
FLAGS     = -g -Wno-deprecated -std=c++11
# -g 编译产生调试信息
# -Wno-deprecated 忽略头文件依赖的警告
# -std=c++11 使用C++11
# compile
ass: $(SRC) $(SRC_H) Makefile
	g++ $(SRC) -o assembly.exe $(FLAGS)



