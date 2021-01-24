#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include "board.h"

class Trace_Pattern_Board : public Board
{
private:
	bool error_flag;
	void loadFromFile(const std::string);
	void readFromStream(std::istream&);
	bool isError();

public:
	Trace_Pattern_Board(const std::string);
	~Trace_Pattern_Board();
};

