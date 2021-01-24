#include "param_info.h"
#include "param.h"

Param_Info::Param_Info(const std::string file_name)
{
    param::parameter param(file_name);
    next_color              = param.get<int>("next_color");
    board_pattern           = param.get<int>("board_pattern");
    max_trace               = param.get<int>("max_trace");
    elimination_coefficient = param.get<double>("elimination_coefficient");
    chain_coefficient       = param.get<double>("chain_coefficient");
    max_connection          = param.get<int>("max_connection");
    is_process_print        = param.get<bool>("is_process_print");
}

Param_Info::~Param_Info()
{

}

int Param_Info::getNextColor() const
{
    return next_color;
}

int Param_Info::getBoardPattern() const
{
    return board_pattern;
}

int Param_Info::getMaxTrace() const
{
    return max_trace;
}

double Param_Info::getEliminationCoefficient() const
{
    return elimination_coefficient;
}

double Param_Info::getChainCoefficient() const
{
    return chain_coefficient;
}

int Param_Info::getMaxConnection() const
{
    return max_connection;
}

bool Param_Info::isProcessPrint() const
{
    return is_process_print;
}

void Param_Info::print() const
{
    std::cout << "ネクストの色       : " << getNextColor() << std::endl;
    std::cout << "盤面パターン       : " << getBoardPattern() << std::endl;
    std::cout << "最大なぞり消し数   : " << getMaxTrace() << std::endl;
    std::cout << "同時消し係数       : " << getEliminationCoefficient() << std::endl;
    std::cout << "連鎖係数           : " << getChainCoefficient() << std::endl;
    std::cout << "消える時の結合数   : " << getMaxConnection() << std::endl;
    std::cout << "連鎖過程の表示     : " << isProcessPrint() << std::endl;
}