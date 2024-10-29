#include <iostream>
#include "functions.h"
#include "Console.h"
#include "MineSweeper.h"

int main()
{
	resizeConsole(CW, CH);
	SetConsoleTitle(TEXT("MINESWEEPER!! - Made by Nguyen Thanh Hai - 23125082"));
	loadScore();
	drawTitleGame();
	drawMainMenu(0);	
	Cursor(false); // Hidden blinking cursor
	handleEvent(); // Read input from user's keyboard
    return 0;
}

//Hướng dẫn điều khiển game
//Em chào thầy / cô
//Em là Nguyễn Thanh Hải - 23125082
//
//Dạ để game đc đúng tỉ lệ nhất : thầy / cô vui lòng giúp em điểu chỉ màn hình console
//theo như sau :
//
//Screen buffer size and Window Size :
//Width: 80 - Height : 35
//
//Font : 16
//
//Cursor Size : Small
//
//Hướng dẫn điều khiển game :
//z: mở ô
//x : cấm cờ
//s : save game
//enter : chọn mục
//< -: qua phải
//	-> : qua trái
//	^ : lên trên
//	v : đi xuống
//
//	Chú ý mục : Highscore chỉ đc lưu khi chơi chế độ chuẩn easy / normal / hard ko áp
//	dụng với custom and continue mode
//
//	Dạ thầy cô chỉ nên bấm các nút cần thiết khi chơi game nếu bấm những nút ko
//	liên quan có thể dẫn để crash chương trình
//
//	Em cảm ơn thầy cô đã xem project của em ạ.Chúc thầy / cô 1 ngày tốt lành




