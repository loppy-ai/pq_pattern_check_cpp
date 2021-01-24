#pragma once
#include <iostream>
#include "size.h"

class Board
{
private:
	int board_entity[board_size] = {0};

public:
	int* board;
	Board();
	~Board();
	int* getBoard() const;
	int getBoardElement(const int) const;
	void setBoardElement(const int, const int);
	void print() const;
	void print(const std::string) const;
};

