#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <Shlwapi.h>
#include <conio.h>
using namespace std;

void resizeConsole(SHORT width, SHORT height);
void clrscr(void);
void gotoXY(SHORT x, SHORT y);
SHORT wherex();
SHORT wherey();
void setColor(WORD color);
void setBackgroundColor(WORD color);
void setColorBGTextXY(SHORT x, SHORT y, WORD color, WORD background, LPSTR str, ...);
void Cursor(BOOL bVisible = TRUE, DWORD dwSize = 25);
void deleteRow(SHORT SStartPos, SHORT SNumberRow);
void Tao_mau_o(int x, int y, int t_color, int b_color, string name);

