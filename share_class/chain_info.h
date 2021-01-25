#pragma once
#include "param_info.h"
#include "next.h"
#include "board.h"
#include "check_board.h"
#include "size.h"

constexpr int max_num_of_chain = 16;

struct Info_Per_Chain {
	int num_r;
	int num_b;
	int num_g;
	int num_y;
	int num_p;
	int num_ojama;
	int num_kata;
	int num_heart;
	int num_prism;
	int sep_r;
	int sep_b;
	int sep_g;
	int sep_y;
	int sep_p;
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

	int getElementCount(const int, const int) const;
	int getSeparateCount(const int, const int) const;

public:
	Chain_Info(const Param_Info*, Next*, Board*, const Board*);
	~Chain_Info();
};

