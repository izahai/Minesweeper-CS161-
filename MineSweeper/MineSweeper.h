#pragma once
#include <iostream>
#include <ctime>
#include <fstream>
#include <chrono>
#include <thread>
#include <string>
#include "Console.h"
#include "functions.h"


#define CW 80
#define CH 35

#define clickLeft 0x5A
#define clickRight 0x58
#define buttonS 0x53

struct board {
	short row = 0;
	short column = 0;
	short numBomb = 0;
	short numOpened = 0;
	short numFlag = 0;
};

struct hole {
	bool haveBomb = false;
	bool haveOpened = false;
	bool getFlag = false;
	short numNearBomb = 0;
};


struct GameClock {
    std::chrono::time_point<std::chrono::high_resolution_clock> start_timepoint;
    bool is_running = false;
    long long easy;
    long long normal;
    long long hard;
    long long tmp;
    long long bonus = 0;
    void update() {
        if (is_running) {
            auto current_timepoint = std::chrono::high_resolution_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_timepoint - start_timepoint).count();
            displayTime(elapsed_time);
        }
    }

    void displayTime(long long milliseconds) {
        tmp = milliseconds / 1000 + bonus;
        string s = "Time: " + to_string(tmp) + "s";
        if (milliseconds % 1000 == 0) Tao_mau_o(CW - 12, 3, 15, 0, s);
    }
};

