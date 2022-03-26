#pragma once
#include <Windows.h>
#include <cstdio>


#define SYM_SEG ".symtab"
#define STR_SEG ".strtab"
#define REL_SEG ".rel"
#define START "_start"
#define TEXT_SEG ".text"
#define DATA_SEG ".data"
#define BSS_SEG ".bss"
#define SH_STR_SEG ".shstrtab"


#define BASE_ADDR 0x08040000
#define MEM_ALIGN 0x1000
#define DISC_ALIGN 4
#define TEXT_ALIGN 16


//
// aux routines for colorful printf. And you should not pay more attention to them
//
#define xINFO(_X_,...)            color_printf(FG_WHITE,_X_,__VA_ARGS__)

#define xPANIC(_X_,...)           color_printf(FG_RED,_X_,__VA_ARGS__)

#define xSUCC(_X_,...)            color_printf(FG_GREEN,_X_,__VA_ARGS__)

#define xWARN(_X_,...)            color_printf(FG_YELLOW,_X_,__VA_ARGS__)


#define FG_BLACK               0x00
#define FG_BLUE                0x01
#define FG_GREEN               0x02
#define FG_BLUE_2              0x03
#define FG_RED                 0x04
#define FG_PURPLE              0x05
#define FG_YELLOW              0x06
#define FG_WHITE               0x0F

inline void setColor(int color, const bool bIntensity = true) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color | (bIntensity ? FOREGROUND_INTENSITY : 0));
}

inline void resetColor() {
	setColor(FG_WHITE);
}

inline int color_printf(const int color, const char* pszFmt, ...) {
	if (color != -1) {
		setColor(color);
	}
	va_list  marker;
	va_start(marker, pszFmt);
	auto ri = vprintf(pszFmt, marker);
	va_end(marker);
	resetColor();
	return ri;
}
