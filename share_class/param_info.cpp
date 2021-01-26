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
    process_print_flag      = param.get<bool>("process_print_flag");
    chain_coefficient_list = chain_coefficient_list_entity;
    setChainCoefficientList();
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

double Param_Info::getChainMagnification(const int chain_count) const
{
    return chain_coefficient_list[chain_count];
}

double Param_Info::getChainMagnification(const int chain_count, const double chain_coefficient) const
{
    return 1 + basic_chain_coefficient[chain_count] * chain_coefficient;
}

void Param_Info::setChainCoefficientList()
{
    int i;
    for (i = 0; i < max_num_of_chain; ++i) {
        chain_coefficient_list_entity[i] = 1 + basic_chain_coefficient[i] * getChainCoefficient();
    }
}

bool Param_Info::isProcessPrint() const
{
    return process_print_flag;
}

void Param_Info::print() const
{
    std::cout << "�l�N�X�g�̐F       : " << getNextColor() << std::endl;
    std::cout << "�Ֆʃp�^�[��       : " << getBoardPattern() << std::endl;
    std::cout << "�ő�Ȃ��������   : " << getMaxTrace() << std::endl;
    std::cout << "���������W��       : " << getEliminationCoefficient() << std::endl;
    std::cout << "�A���W��           : " << getChainCoefficient() << std::endl;
    std::cout << "�����鎞�̌�����   : " << getMaxConnection() << std::endl;
    std::cout << "�A���ߒ��̕\��     : " << isProcessPrint() << std::endl;
}