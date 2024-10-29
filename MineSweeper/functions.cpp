#include "MineSweeper.h"
#include "functions.h"
#include "Console.h"

board b;
hole** h;
COORD cursor; //Current cursor
bool keyboardOn = false;
bool legit = true;

//Coordinate x, y for drawing board
short xB;
short yB;

bool stateGame = false;

short page, position, sumCatelog;

fstream fin;
fstream fout;

GameClock timer;

void create2dArr()
{
	h = new hole * [b.row];
	for (int i = 0; i < b.row; i++) {
		h[i] = new hole[b.column];
	}
}
void delete2dArr() 
{
	for (int i = 0; i < b.row; i++) {
		delete[] h[i];
	}
	delete[] h;
}

void saveCoordinate() {
	xB = CW / 2 - b.column; //Column
	yB = 6 + (CH - 6) / 2 - b.row / 2; //Row
}

void initalize(short row, short column, short numBomb)
{
	b.row = row;
	b.column = column;
	b.numBomb = numBomb;
	b.numOpened = 0;
	b.numFlag = 0;
	timer.bonus = 0;

	create2dArr();
	saveCoordinate();
	cursor = { 0,0 };
	stateGame = true;
	drawBoard();
	createRandomBomb();
}

short coordinateX(short x) { //Find coordinate of x to draw board in middle
	return ((x * 2) + xB);
}

short coordinateY(short y) { //Find coordinate of y to draw board in middle
	return (y + yB);
}
void drawHole(short x, short y, short type)
{
	switch (type) {
	case 0: //Ô rỗng có màu trắng sáng
		Tao_mau_o(coordinateX(x), coordinateY(y), 0, 15, "  ");
		break;
	case 1: //So 1 chu mau xanh duong. Tu 1 -> 8  có nền màu trắng
		Tao_mau_o(coordinateX(x), coordinateY(y), 9, 15, "1 ");
		break;
	case 2:
		Tao_mau_o(coordinateX(x), coordinateY(y), 2, 15, "2 ");
		break;
	case 3:
		Tao_mau_o(coordinateX(x), coordinateY(y), 12, 15, "3 ");
		break;
	case 4:
		Tao_mau_o(coordinateX(x), coordinateY(y), 1, 15, "4 ");
		break;
	case 5:
		Tao_mau_o(coordinateX(x), coordinateY(y), 4, 15, "5 ");
		break;
	case 6:
		Tao_mau_o(coordinateX(x), coordinateY(y), 3, 15, "6 ");
		break;
	case 7:
		Tao_mau_o(coordinateX(x), coordinateY(y), 0, 15, "7 ");
		break;
	case 8:
		Tao_mau_o(coordinateX(x), coordinateY(y), 13, 15, "8 ");
		break;
	case 9: //Red background + black text
		Tao_mau_o(coordinateX(x), coordinateY(y), 0, 12, "B ");
		break;
	case 10:// ô chẵn màu đen
		Tao_mau_o(coordinateX(x), coordinateY(y), 0, 8, "  ");
		break;
	case 11:// ô lẻ màu xám
		Tao_mau_o(coordinateX(x), coordinateY(y), 0, 7, "  ");
		break;
	case 12:
		Tao_mau_o(coordinateX(x) + 1, coordinateY(y), 0, 13, " ");
		break;
	case 13:
		Tao_mau_o(coordinateX(x), coordinateY(y), 12, 14, "F ");
		break;
	case 14:
		Tao_mau_o(coordinateX(x), coordinateY(y), 15, 6, "Fx");
		break;
	case 15:
		Tao_mau_o(coordinateX(x), coordinateY(y), 12, 14, "B ");
		break;
	}
}
void drawBoard()
{
	for (int i = 0; i < b.row; ++i) {
		for (int j = 0; j < b.column; ++j) {
			if (h[i][j].getFlag)
				drawHole(j, i, 13);
			else if (h[i][j].numNearBomb)
				drawHole(j, i, h[i][j].numNearBomb);
			else if (h[i][j].haveOpened)
				drawHole(j, i, 0);
			else if ((i+j) % 2)
				drawHole(j, i, 11);
			else 
				drawHole(j, i, 10);
			if (keyboardOn)
			{
				drawHole(cursor.X, cursor.Y, 12);
			}
		}
	}
}


bool numOfFlag() {
	if (((b.numFlag + b.numOpened) == (b.column * b.row)) || (b.numBomb == (b.column * b.row - b.numOpened)))
		return 1;
	return 0;
}

void createRandomBomb() {
	short numBomb = b.numBomb;
	short x, y;//Coordinate of random bomb
	srand(time(NULL));//Reset time
	while (numBomb != 0) { //First numBomb have n value
		x = rand() % b.row; //Generate coordinate 
		y = rand() % b.column;
		
		if (h[x][y].haveBomb) continue; //Pass and continue new loop because already have bomb
			
		h[x][y].haveBomb = true;
		--numBomb; //Update new number of bombs
	}
}


void clickR(short x, short y) //Get flag
{
	if (!h[x][y].haveOpened) {
		if (h[x][y].getFlag) {
			h[x][y].getFlag = false;
			b.numFlag--;
		}
		else {
			h[x][y].getFlag = true;
			b.numFlag++;
		}
		if (stateGame) {
			drawBoard();
			if (numOfFlag()) win();
		}
	}
}

short countNearBomb(short x, short y) {
	short count = 0;
	for (int i = x - 1; i <= x + 1; ++i) {
		for (int j = y - 1; j <= y + 1; ++j) {
			if (i < 0 || j < 0 || j >= b.column || i >= b.row || (i == x && j == y) )
				continue;
			if (h[i][j].haveBomb)
				count++;
		}
	}
	return count;
}
void open(short x, short y)
{
	if (!h[x][y].haveOpened && !h[x][y].getFlag) {
		h[x][y].haveOpened = true;
		if (h[x][y].haveBomb) {
			lose();
		}
		else
		{
			b.numOpened++;
			short nearBomb = countNearBomb(x, y);
			if (nearBomb) h[x][y].numNearBomb = nearBomb;
			else {
				for (int i = x - 1; i <= x + 1; ++i) {
					for (int j = y - 1; j <= y + 1; ++j) {
						if (i < 0 || j < 0 || j >= b.column || i >= b.row || (i == x && j == y))
							continue;
						open(i, j);
					}
				}
			}
			
		}
	}
}

void clickL(short x, short y) {
	if (!h[x][y].haveOpened && !h[x][y].getFlag) {
		open(x, y);
		if (stateGame) {
			drawBoard();
			if (numOfFlag()) win();
		}
	}
}

void win() {
	stateGame = false;
	delete2dArr();
	deleteRow(4, 5);
	drawStateOfPlay(2);
	if(legit) updateScore(position);
	
	position = 0;
	drawPlayAgain(0);
}

void lose() {
	for (int i = 0; i < b.row; i++) 
		for (int j = 0; j < b.column; j++) 
			if (!h[i][j].getFlag && !h[i][j].haveBomb && !h[i][j].haveOpened) {
				h[i][j].haveOpened = true;
				short nearBomb = countNearBomb(i, j);
				if (nearBomb) h[i][j].numNearBomb = nearBomb;
			}
	drawBoard();

	for (int i = 0; i < b.row; i++) {
		for (int j = 0; j < b.column; j++) {
			if (h[i][j].getFlag) {
				if (h[i][j].haveBomb)
					drawHole(j, i, 15); // True flag
				else
					drawHole(j, i, 14); // Wrong flag
			}
			else {
				if (h[i][j].haveBomb) // Illustrate bomb
					drawHole(j, i, 9);
			}
		}
	}

	stateGame = false;
	delete2dArr();
	deleteRow(4, 4);
	drawStateOfPlay(3);
	//if(legit) updateScore(position);

	position = 0;
	drawPlayAgain(0);

}
void handleKeyboard(KEY_EVENT_RECORD key) {
	if (key.bKeyDown) //User have pressed keyboard
	{
		switch (key.wVirtualKeyCode)
		{
		case VK_UP: //Arrow up
			switch (page) {
			case 1:
				if (position == 0) position = sumCatelog - 1;
				else position--;
				drawMainMenu(position);
				break;
			case 2: //Menu level
				if (position == 0) position = sumCatelog - 1;
				else position--;
				drawLevelMenu(position);
				break;
			case 3: //Playing
				if (stateGame) {
					keyboardOn = true;
					cursor.Y = ((cursor.Y == 0) ? b.row - 1 : cursor.Y - 1);
					drawBoard();
				}
				else {
					if (position == 0) position = sumCatelog - 1;
					else position--;
					drawPlayAgain(position);
				}
				break;
			}
			break;

		case VK_DOWN: //Arrow down
			switch (page) {
			case 1: //Page 1 - Main menu
				if (position == sumCatelog - 1) position = 0;
				else position++;
				drawMainMenu(position);
				break;
			case 2: //Page 2 - Level menu
				if (position == sumCatelog - 1) position = 0;
				else position++;
				drawLevelMenu(position);
				break;
			case 3: //Page 3 - Playing
				if (stateGame) {
					keyboardOn = true;
					cursor.Y = ((cursor.Y == b.row - 1) ? 0 : cursor.Y + 1);
					drawBoard();
				}
				else {
					if (position == sumCatelog - 1) position = 0;
					else position++;
					drawPlayAgain(position);
				}
				break;
			}
			break;
		case VK_LEFT: //Arrow left
			if (page == 3) {
				if (stateGame == true) {
					keyboardOn = true;
					cursor.X = ((cursor.X == 0) ? b.column - 1 : cursor.X - 1);
					drawBoard();
				}
			}
			break;
		case VK_RIGHT://Arrow right2
			if (page == 3) {
				if (stateGame == true) {
					keyboardOn = true;
					cursor.X = ((cursor.X == b.column - 1) ? 0 : cursor.X + 1);
					drawBoard();
					break;
				}
			}
			break;
		case VK_RETURN://Enter button
			switch (page) {
			case 1: //Page 1 - Main menu
				switch (position) {
				case 0: //New game
					legit = true;
					position = 0;
					deleteRow(4, 10);
					drawLevelMenu(position);
					break;
				case 1: {//Continue 
					legit = false;
					deleteRow(3, 20);
					string s7 = "Don't record high score in continue mode!!";
					Tao_mau_o(CW /2 - s7.length() / 2, 5, 12, 0, s7);
					loadFile();
					break;
				}
				case 2: //Highscore
					deleteRow(3, 20);
					loadScore();
					drawHighScore();
					break;
				case 3: 
					exit(0);
					break;
				}
				break;
			case 2: //Page 2 - Level Menu
				switch (position) {
				case 0: //Easy
					deleteRow(3, 20);
					page = 3;
					initalize(9, 9, 8);
					drawStateOfPlay(1);
					break;
				case 1: //Normal
					deleteRow(3, 20);
					page = 3;
					initalize(16, 16, 20);
					drawStateOfPlay(1);
					break;
				case 2: //Hard
					deleteRow(3, 10);
					page = 3;
					initalize(25, 25, 90);
					drawStateOfPlay(1);
					break;
				case 3: // Custom
					legit = false;
					int a, b, c;
					deleteRow(3, 20);
					page = 3;
					position = 0;
					cout << "Enter row (9 to 25): ";
					cin >> a;
					cout << "Enter column (9 to 25): ";
					cin >> b;
					cout << "Enter num of bombs(1 to 90): ";
					cin >> c;
					deleteRow(4, 10);
					initalize(a, b, c);
					drawStateOfPlay(1);
					break;
				case 4: //Back
					page = 1;
					position = 0;
					deleteRow(4, 10);
					drawMainMenu(position);
					break;
				}
				break;
			case 3: //Page 3 - Playing 
				if (stateGame == false) {
					switch (position) {
					case 0:
						deleteRow(3, 100);
						initalize(b.row, b.column, b.numBomb);
						drawStateOfPlay(1);
						break;
					case 1:
						deleteRow(3, 100);
						position = 0;
						drawMainMenu(position);
						break;

					default:
						break;
					}
				}	
				break;
			case 4: //Page 4 - Highscore
				position = 0;
				deleteRow(3, 20);
				drawMainMenu(position);
				break;
			case 5:
				break;
			}
			break;
		case VK_ESCAPE: //Esc button
			break;
		case clickLeft: //z button - open
			clickL(cursor.Y, cursor.X);
			break;
		case clickRight: //x button - getFlag
			clickR(cursor.Y, cursor.X);
			break;
		case buttonS: //s button - save
			if (page == 3 && stateGame) {
				string s10 = " Saved  ";
				Tao_mau_o(26, 6, 14, 0, s10);
				saveFile();
			}
			break;
		default:
			break;
		}
	}
}

void handleEvent() {
	while (1)
	{
		DWORD DWnumOfEvents = 0;// Save current event, DWORD is integer positive num (32 bit)
		DWORD DWnumOfEventsRead = 0; //Save event have been read

		HANDLE HconsoleInput = GetStdHandle(STD_INPUT_HANDLE); //Device Input
		GetNumberOfConsoleInputEvents(HconsoleInput, &DWnumOfEvents); //Connect input event to variable DWnumOfEvents

		if (DWnumOfEvents != 0) {
			INPUT_RECORD* IReventBuffer = new INPUT_RECORD[DWnumOfEvents]; //Pointer for eventBuffer
			ReadConsoleInput(HconsoleInput, IReventBuffer, DWnumOfEvents, &DWnumOfEventsRead); //Connect saved events to pointer eventBuffer

			//Run loops to read events
			for (DWORD i = 0; i < DWnumOfEvents; i++) {
				if (IReventBuffer[i].EventType == KEY_EVENT) //If there are keyboard events
				{
					handleKeyboard(IReventBuffer[i].Event.KeyEvent);
				}
			}
		}
		if (!timer.is_running && page == 3 && stateGame) {
			timer.start_timepoint = std::chrono::high_resolution_clock::now();
			timer.is_running = true;
		}

		if (timer.is_running && page == 3 && stateGame) {
			timer.update();
		}

		if (!stateGame && page == 3 && timer.is_running) timer.is_running = false;
	}
}

void drawTitleGame()
{
	short i;
	for (int i = 0; i < CW; i++) {
		cout << "-";
	}
	cout << endl;
	string s = "    *\\(^o^)/* MINESWEEPER *\\(^o^)/*   ";
	Tao_mau_o(CW / 2 - s.length() / 2, 1, 9, 15, s);
	setColor(7);
	setBackgroundColor(0);
	cout << endl;
	
	for (int i = 0; i < CW; i++) {
		cout << "-";
	}
}

void drawStateOfPlay(short state) {
	string save = "Press 's' button to save: Unsaved";
	string s = "State: ";
	string w = "YOU WIN!";
	string l = "YOU LOSE!";
	string c = "Playing";
	string s2 = "Num of bomb: " + to_string(b.numBomb);
	string s1 = "Size: " + to_string(b.row) + " x " + to_string(b.column);

	
	Tao_mau_o(1, 3, 15, 0, s1);
	Tao_mau_o(1, 4, 15, 0, s2);
	Tao_mau_o(1, 6, 15, 0, save);
	Tao_mau_o(1, 5, 15, 0, s);

	//Draw status
	if (state == 1) //Current playing
		Tao_mau_o(8, 5, 10, 0, c);
	if (state == 2) //Win
		Tao_mau_o(8, 5, 14, 0, w);
	if (state == 3) //Lose
		Tao_mau_o(8, 5, 12, 0, l);
}

void drawMainMenu(short index) {
	// Update
	page = 1;
	position = index;
	sumCatelog = 4;

	string s2 = "Press 'Enter' button to choose";
	Tao_mau_o(1, 3, 15, 0, s2);
	// Draw menu
	string s = "  New game  ";
	Tao_mau_o((CW / 2) - s.length() / 2, 7, 15, (index == 0) ? 5 : 0, s);
	s = "  Continue  ";
	Tao_mau_o((CW / 2) - s.length() / 2, 8, 15, (index == 1) ? 5 : 0, s);
	s = "  Score board  ";
	Tao_mau_o((CW / 2) - s.length() / 2, 9, 15, (index == 2) ? 5 : 0, s);
	s = "  Exit  ";
	Tao_mau_o((CW / 2) - s.length() / 2, 10, 15, (index == 3) ? 5 : 0, s);
}

void drawLevelMenu(short index) {
	// Update
	page = 2;
	position = index;
	sumCatelog = 5;
	string s1 = "Choose level to play";
	Tao_mau_o((CW / 2) - (s1.length() / 2), 5, 10, 0, s1);

	// Draw menu
	string s = "  Beginner (9 x 9 + 8 bombs)  ";
	Tao_mau_o((CW / 2) - s.length() / 2, 7, 15, (index == 0) ? 5 : 0, s);
	s = "  Intermediate (16 x 16 + 40 bombs)  ";
	Tao_mau_o((CW / 2) - s.length() / 2, 8, 15, (index == 1) ? 5 : 0, s);
	s = "  Expert (25 x 25 + 90 bombs)  ";
	Tao_mau_o((CW / 2) - s.length() / 2, 9, 15, (index == 2) ? 5 : 0, s);
	s = "  Custom  ";
	Tao_mau_o((CW / 2) - s.length() / 2, 10, 15, (index == 3) ? 5 : 0, s);
	s = "  Back  ";
	Tao_mau_o((CW / 2) - s.length() / 2, 11, 15, (index == 4) ? 5 : 0, s);
}

void saveFile() {
	fout.open("save.txt");
	if (fout.is_open()) {
		fout << timer.tmp << endl;
		fout << b.column << endl;;
		fout << b.row << endl;
		fout << b.numBomb << endl;
		fout << b.numFlag << endl;
		fout << b.numOpened << endl;

		for (int i = 0; i < b.row; ++i) {
			for (int j = 0; j < b.column; ++j) {
				fout << h[i][j].haveOpened << endl;
				fout << h[i][j].haveBomb << endl;
				fout << h[i][j].getFlag << endl;
				fout << h[i][j].numNearBomb << endl;
			}
		}
		
	}
	else {
		cout << "The file cann't open!\n";
	}
	fout.close();
}

void loadFile() {
	fin.open("save.txt");
	if (fin.is_open() && fin.peek() == std::ifstream::traits_type::eof()) {
		deleteRow(3, 50);
		cout << "The file is empty!!" << endl;
		exit(666);
	}
	if (fin.is_open()) {
		fin >> timer.bonus;
		fin >> b.column;
		fin >> b.row;
		fin >> b.numBomb;
		fin >> b.numFlag;
		fin >> b.numOpened;

		create2dArr();

		for (int i = 0; i < b.row; ++i) {
			for (int j = 0; j < b.column; ++j) {
				fin >> h[i][j].haveOpened;
				fin >> h[i][j].haveBomb;
				fin >> h[i][j].getFlag;
				fin >> h[i][j].numNearBomb;
			}
		}

		reInitalize();
		drawStateOfPlay(1);
	}
	else {
		cout << "The file cann't open!\n";
	}
	fin.close();
}

void reInitalize()
{
	page = 3;
	//create2dArr();
	saveCoordinate();
	cursor = { 0,0 };
	stateGame = true;
	drawBoard();
	//createRandomBomb();
}

void loadScore() {
	fin.open("highscore.txt");
	if (fin.is_open()) {
		fin >> timer.easy;
		fin >> timer.normal;
		fin >> timer.hard;
	}
	else {
		cout << "The file cann't open!\n";
	}
	fin.close();
}

void saveScore() {
	fout.open("highscore.txt");
	if (fout.is_open()) {
		fout << timer.easy << endl;
		fout << timer.normal << endl;
		fout << timer.hard << endl;
	}
	else {
		cout << "The file cann't open!\n";
	}
	fout.close();
}

void drawHighScore() {
	page = 4;
	string s1 = "  Highest score of easy mode: ";
	if (timer.easy == -1) s1 += "unknown!  ";
	else s1 += to_string(timer.easy) + "s  ";
	
	string s2 = "  Highest score of normal mode: ";
	if (timer.normal == -1) s2 += "unknown!  ";
	else s2 += to_string(timer.normal) + "s  ";

	string s3 = "  Highest score of hard mode: ";
	if (timer.hard == -1) s3 += "unknown!  ";
	else s3 += to_string(timer.hard) + "s  ";
	
	string s4 = "  Back  ";

	string s5 = "Press 'Enter' button to back";

	Tao_mau_o((CW / 2) - s1.length() / 2, 7, 14, 0, s1);
	Tao_mau_o((CW / 2) - s2.length() / 2, 9, 14, 0, s2);
	Tao_mau_o((CW / 2) - s3.length() / 2, 11, 14, 0, s3);
	Tao_mau_o((CW / 2) - s4.length() / 2, 13, 15, 5, s4);
	Tao_mau_o(1, 3, 15, 0, s5);
}

void drawPlayAgain(short index) {
	page = 3;
	sumCatelog = 2;

	string s2 = "Press 'Enter' button to choose   ";
	Tao_mau_o(1, 6, 15, 0, s2);
	
	string s1 = "  Play again  ";
	Tao_mau_o((CW / 2) - s1.length() / 2, 4, 15, (index == 0) ? 5 : 0, s1);
	s1 = "  Back  ";
	Tao_mau_o((CW / 2) - s1.length() / 2, 6, 15, (index == 1) ? 5 : 0, s1);
}

void updateScore(short index) {
	switch (index) {
	case 0:
		if (timer.easy == -1) timer.easy = timer.tmp;
		else if (timer.tmp < timer.easy) timer.easy = timer.tmp;
		break;
	case 1:
		if (timer.normal == -1) timer.normal = timer.tmp;
		else if (timer.tmp < timer.normal) timer.normal = timer.tmp;
		break;
	case 2:
		if (timer.hard == -1) timer.hard = timer.tmp;
		else if (timer.tmp < timer.hard) timer.hard = timer.tmp;
		break;
	default:
		break;
	}
	saveScore();
}

/*
Page 1: main menu 
Page 2: menu level
Page 3: playing game
Page 4: highscore
*/

/*
Index 1: easy
Index 2: normal
Index 3: hard
*/


