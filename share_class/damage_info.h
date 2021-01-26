#pragma once
#include "param_info.h"
#include "chain_info.h"

class Damage_Info
{
private:
	int red_elimi;
	int blue_elimi;
	int green_elimi;
	int yellow_elimi;
	int purple_elimi;
	double red_magni;
	double blue_magni;
	double green_magni;
	double yellow_magni;
	double purple_magni;
	double wild_magni;

public:
	Damage_Info(Param_Info*, Chain_Info*);
	~Damage_Info();
};