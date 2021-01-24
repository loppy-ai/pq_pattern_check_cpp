#pragma once
#include <iostream>
#include <string>

class Param_Info
{
private:
	int next_color;
	int board_pattern;
	int max_trace;
	double elimination_coefficient;
	double chain_coefficient;
	int max_connection;
	bool is_process_print;

public:
	Param_Info(const std::string);
	~Param_Info();
	int getNextColor() const;
	int getBoardPattern() const;
	int getMaxTrace() const;
	double getEliminationCoefficient() const;
	double getChainCoefficient() const;
	int getMaxConnection() const;
	bool isProcessPrint() const;
	void print() const;
};