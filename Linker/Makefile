
# make
TOOL_PATH = ../make_tool/

MAKE      = $(TOOL_PATH) make.exe -r
SRC       = main.cpp Link.cpp Elf_File.cpp
SRC_H     = Common.h Link.h Elf_File.h elf.h
FLAGS     = -g -Wno-deprecated -std=c++11
# -g 编译产生调试信息
# -Wno-deprecated 忽略头文件依赖的警告
# -std=c++11 使用C++11
# compile
link: $(SRC) $(SRC_H) Makefile
	g++ $(SRC) -o linker.exe $(FLAGS)



