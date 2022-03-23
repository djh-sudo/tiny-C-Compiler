
# make
TOOL_PATH = ./make_tool/

MAKE      = $(TOOL_PATH) make.exe -r
SRC       = main.cpp
FLAGS     = -g -Wno-deprecated -std=c++11 -w
# -g 编译产生调试信息
# -Wno-deprecated 忽略头文件依赖的警告
# -std=c++11 使用C++11
# -w 忽略警告
# compile
jcc: $(SRC) Makefile
	g++ $(SRC) -o jcc.exe $(FLAGS)
