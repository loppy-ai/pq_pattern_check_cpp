#pragma once
#include "param_info.h"
#include "next.h"
#include "board.h"
#include "check_board.h"
#include "size.h"

struct Info_Per_Chain {
	int num_r = 0;
	int num_b = 0;
	int num_g = 0;
	int num_y = 0;
	int num_p = 0;
	int num_ojama = 0;
	int num_kata = 0;
	int num_heart = 0;
	int num_prism = 0;
	int sep_r = 0;
	int sep_b = 0;
	int sep_g = 0;
	int sep_y = 0;
	int sep_p = 0;
};

class Chain_Info
{
private:
	int num_of_chain;
	Info_Per_Chain all_chain_info[max_num_of_chain] = { 0 };

	void chain(const Param_Info*, Next*, Board*, const Board*);
	void applyTracePattern(Board*, const Board*);
	void dropBoard(Board*);
	void checkConnection(const int, const Param_Info*, Board*, Check_Board*);
	int recursionCheckConnection(const int, Board*, Check_Board*, int);
	void checkConnectionOther(const int, const int, Board*);
	bool dropNext(Next*, Board*);

	void setElementCount(const int, const int, const int);
	void setSeparateCount(const int, const int);

	void debugPrint(const Param_Info*, const Next*, const Board*, const std::string) const;
	void debugPrint(const Param_Info*, const Board*, const std::string) const;
	void debugChain(const int) const;

	int getElementCountPerColorAndChain(const int, const int) const;
	int getSeparateCountPerColorAndChain(const int, const int) const;
	int getSeparateCountPerChain(const int) const;
	double getMagnificationPerColorAndChain(const Param_Info*, const int, const int) const;
	double getMagnificationPerColorAndChain(const Param_Info*, const double, const double, const int, const int) const;

public:
	Chain_Info(const Param_Info*, Next*, Board*, const Board*);
	~Chain_Info();
	int getNumOfChain() const;
	int getElementCountPerColor(const int) const;
	int getElementCountPerChain(const int) const;
	double getMagnificationPerColor(const Param_Info*, const int) const;
	double getMagnificationPerColor(const Param_Info*, const double, const double, const int) const;
};