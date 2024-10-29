#pragma once
void create2dArr();
void delete2dArr();
void initalize(short row, short column, short numBomb);
void drawHole();
void drawBoard();
void createRandomBomb();
void handleEvent();
void open(short x, short y);
void clickL(short x, short y);
short countNearBomb(short x, short y);
void clickR(short x, short y);
void win();
void lose();
void reInitalize();

//Design 
void drawTitleGame();
void drawStateOfPlay(short state);
void drawMainMenu(short index);
void drawLevelMenu(short index);
void drawHighScore();
void drawPlayAgain(short index);

//Load & Save
void loadFile();
void saveFile();
void loadScore();
void saveScore();
void updateScore(short index);