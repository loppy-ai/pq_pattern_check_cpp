#include "param_info.h"
#include "param.h"

Param_Info::Param_Info(char** argv)
{
    next_color              = atoi(argv[1]);
    board_pattern           = atoi(argv[2]);
    max_trace               = atoi(argv[3]);
    elimination_coefficient = atof(argv[4]);
    chain_coefficient       = atof(argv[5]);
    max_color               = atoi(argv[6]);
    max_connection          = (board_pattern < 100) ? 3 : 4;
    process_print_flag      = false;
    max_trace_print_flag    = true;
    chain_coefficient_list  = chain_coefficient_list_entity;
    setChainCoefficientList();
}

Param_Info::Param_Info(const std::string file_name)
{
    param::parameter param(file_name);
    next_color              = param.get<int>("next_color");
    board_pattern           = param.get<int>("board_pattern");
    max_trace               = 0;
    elimination_coefficient = param.get<double>("elimination_coefficient");
    chain_coefficient       = param.get<double>("chain_coefficient");
    max_color               = 0;
    max_connection          = (board_pattern < 100) ? 3 : 4;
    process_print_flag      = param.get<bool>("process_print_flag");
    max_trace_print_flag    = false;
    chain_coefficient_list  = chain_coefficient_list_entity;
    setChainCoefficientList();
}

Param_Info::~Param_Info()
{

}

// ネクストの色を取得
int Param_Info::getNextColor() const
{
    return next_color;
}

// 盤面パターンを取得
int Param_Info::getBoardPattern() const
{
    return board_pattern;
}

// 最大なぞり消し数を取得
int Param_Info::getMaxTrace() const
{
    return max_trace;
}

// 同時消し係数を取得
double Param_Info::getEliminationCoefficient() const
{
    return elimination_coefficient;
}

// 連鎖係数倍率を取得
double Param_Info::getChainCoefficient() const
{
    return chain_coefficient;
}

// 求めたい色を取得
int Param_Info::getMaxColor() const
{
    return max_color;
}

// ぷよが消える最大結合数を取得
int Param_Info::getMaxConnection() const
{
    return max_connection;
}

// 連鎖倍率を取得
double Param_Info::getChainMagnification(const int chain_count) const
{
    return chain_coefficient_list[chain_count];
}

// 連鎖倍率を取得
double Param_Info::getChainMagnification(const int chain_count, const double chain_coefficient) const
{
    return 1 + basic_chain_coefficient[chain_count] * chain_coefficient;
}

// 連鎖係数をリストとして設定
void Param_Info::setChainCoefficientList()
{
    for (int i = 0; i < max_num_of_chain; ++i) {
        chain_coefficient_list_entity[i] = 1 + basic_chain_coefficient[i] * getChainCoefficient();
    }
}

// 連鎖過程を表示するか判定
bool Param_Info::isProcessPrint() const
{
    return process_print_flag;
}

// 情報表示
void Param_Info::print() const
{
    std::string ipp = isProcessPrint() ? "表示する" : "表示しない";
    std::cout << "---------------------設定情報---------------------" << std::endl;
    std::cout << "ネクストの色       : " << getNextColor() << std::endl;
    std::cout << "盤面パターン       : " << getBoardPattern() << std::endl;
    if (max_trace_print_flag) {
        std::cout << "最大なぞり消し数   : " << getMaxTrace() << std::endl;
    }
    std::cout << "同時消し係数       : " << getEliminationCoefficient() << std::endl;
    std::cout << "連鎖係数           : " << getChainCoefficient() << std::endl;
    if (max_trace_print_flag) {
        std::cout << "求めたい色         : " << getMaxColor() << std::endl;
    }
    std::cout << "消える時の結合数   : " << getMaxConnection() << std::endl;
    std::cout << "連鎖過程の表示     : " << ipp << std::endl;
}