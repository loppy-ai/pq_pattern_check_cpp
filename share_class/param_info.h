#pragma once
#include <iostream>
#include <string>
#include "size.h"

constexpr double basic_chain_coefficient[max_num_of_chain] = { 0, 0.4, 0.7, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4 };

class Param_Info
{
private:
	int next_color;
	int board_pattern;
	int max_trace;
	double elimination_coefficient;
	double chain_coefficient;
	int max_connection;
	bool process_print_flag;
	double chain_coefficient_list_entity[max_num_of_chain] = { 0.0 };
	double *chain_coefficient_list;
	void setChainCoefficientList();

public:
	Param_Info(const std::string);
	~Param_Info();
	int getNextColor() const;
	int getBoardPattern() const;
	int getMaxTrace() const;
	double getEliminationCoefficient() const;
	double getChainCoefficient() const;
	int getMaxConnection() const;
	double getChainMagnification(const int) const;
	double getChainMagnification(const int, const double) const;
	bool isProcessPrint() const;
	void print() const;
};