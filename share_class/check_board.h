#pragma once
#include "board.h"
#include "size.h"

enum CheckBoardElement
{
	NoCheck,
	Checking,
	Combined,
	Uncombined
};

class Check_Board : public Board
{
public:
	Check_Board();
	~Check_Board();
	bool isConfirmed(const int) const;
	bool isChecking(const int) const;
	bool isCombined(const int) const;
	void setBoardElementChecking(const int);
	void setBoardElementCombined(const int);
	void setBoardElementUncombined(const int);

	bool checkUpper(const int, const int) const;
	bool checkRight(const int, const int) const;
	bool checkLower(const int, const int) const;
	bool checkLeft(const int, const int) const;
};