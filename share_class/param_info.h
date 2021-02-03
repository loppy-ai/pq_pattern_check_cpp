#pragma once
#include <iostream>
#include <string>
#include "size.h"

constexpr double basic_chain_coefficient[max_num_of_chain] = { 0, 0.4, 0.7, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4 };

class Param_Info
{
private:
	int next_color;						// ネクストの色
	int board_pattern;					// 盤面パターン
	int max_trace;						// 最大なぞり消し数
	double elimination_coefficient;		// 同時消し係数
	double chain_coefficient;			// 連鎖係数倍率
	int max_color;						// 最大値を求めたい色
	int max_connection;					// 最大結合数
	bool max_trace_print_flag;			// 最大なぞり消し数表示フラグ
	bool process_print_flag;			// 連鎖過程表示フラグ
	double chain_coefficient_list_entity[max_num_of_chain] = { 0.0 };	// 連鎖係数リストの実体
	double *chain_coefficient_list;										// 連鎖係数リストのアドレス
	void setChainCoefficientList();

public:
	Param_Info(char**);
	Param_Info(const std::string);
	~Param_Info();
	int getNextColor() const;
	int getBoardPattern() const;
	int getMaxTrace() const;
	double getEliminationCoefficient() const;
	double getChainCoefficient() const;
	int getMaxColor() const;
	int getMaxConnection() const;
	double getChainMagnification(const int) const;
	double getChainMagnification(const int, const double) const;
	bool isProcessPrint() const;
	void print() const;
};