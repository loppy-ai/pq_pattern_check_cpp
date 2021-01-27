#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <time.h>
#include "param_info.h"
#include "chain_info.h"


void printResult(const Param_Info*, const Chain_Info*);
std::string getTimeStamp();