#include "print.h"

// ���ʕ\��
void printResult(const Param_Info* pi, const Chain_Info* ci)
{
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "|   �F   |������|  �{��  |�y�A�{��|" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "|   ��   |" << std::right << std::setw(6) << ci->getElementCountPerColor(Red) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, Red) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, Red) * 5.5 << "|" << std::endl;
    std::cout << "|   ��   |" << std::right << std::setw(6) << ci->getElementCountPerColor(Blue) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, Blue) << "| ------ |" << std::endl;
    std::cout << "|   ��   |" << std::right << std::setw(6) << ci->getElementCountPerColor(Green) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, Green) << "| ------ |" << std::endl;
    std::cout << "|   ��   |" << std::right << std::setw(6) << ci->getElementCountPerColor(Yellow) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, Yellow) << "| ------ |" << std::endl;
    std::cout << "|   ��   |" << std::right << std::setw(6) << ci->getElementCountPerColor(Purple) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, Purple) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, Purple) * 5.5 << "|" << std::endl;
    std::cout << "|���C���h| ---- |" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, None) << "| ------ |" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
}